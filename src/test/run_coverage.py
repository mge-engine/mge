#!/usr/bin/env python3
# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
"""Run headless tests under OpenCppCoverage and report overall coverage."""

import argparse
import json
import os
import shutil
import subprocess
import sys
import xml.etree.ElementTree as ET


def discover_tests(ctest, binary_dir):
    """Discover headless tests via ctest --show-only=json-v1."""
    result = subprocess.run(
        [ctest, "--show-only=json-v1", "-LE", "capture|graphics",
         "--test-dir", binary_dir],
        capture_output=True, text=True
    )
    if result.returncode != 0:
        print(f"Error discovering tests: {result.stderr}", file=sys.stderr)
        sys.exit(1)
    data = json.loads(result.stdout)
    tests = []
    for test in data.get("tests", []):
        command = test.get("command", [])
        if command:
            name = test.get("name", "unknown")
            tests.append({"name": name, "command": command})
    return tests


def apply_gtest_exclude(command, gtest_exclude):
    """Add --gtest_filter to exclude death tests from a command."""
    if not gtest_exclude:
        return command
    exe = os.path.normpath(command[0])
    exe_name = os.path.splitext(os.path.basename(exe))[0]
    filters = gtest_exclude.get(exe_name, [])
    if not filters:
        return command
    negative = "-" + ":".join(filters)
    return command + [f"--gtest_filter={negative}"]


def run_coverage(opencppcoverage, tests, source_dir, output_dir,
                 gtest_exclude, html=False):
    """Run each test under OpenCppCoverage, accumulating coverage."""
    src_mge = os.path.normpath(os.path.join(source_dir, "src", "mge"))
    src_modules = os.path.normpath(
        os.path.join(source_dir, "src", "modules"))
    output_dir = os.path.normpath(output_dir)

    failed = []
    prev_cov = None

    for i, test in enumerate(tests):
        test_command = apply_gtest_exclude(test["command"], gtest_exclude)
        test_exe = test_command[0]
        test_args = test_command[1:]
        test_name = test["name"]
        is_last = (i == len(tests) - 1)
        step_cov = os.path.join(output_dir, f"step{i}.cov")

        cmd = [
            opencppcoverage,
            "--quiet",
            "--sources", src_mge,
            "--sources", src_modules,
            "--excluded_sources", "*\\test\\*",
            "--continue_after_cpp_exception",
        ]

        if prev_cov:
            cmd += ["--input_coverage", prev_cov]

        if is_last:
            cobertura_path = os.path.join(output_dir, "coverage.xml")
            cmd += [
                "--export_type", f"binary:{step_cov}",
                "--export_type", f"cobertura:{cobertura_path}",
            ]
            if html:
                html_dir = os.path.join(output_dir, "html")
                cmd += ["--export_type", f"html:{html_dir}"]
        else:
            cmd += ["--export_type", f"binary:{step_cov}"]

        cmd += ["--", test_exe] + test_args

        print(f"[{i+1}/{len(tests)}] {test_name}")
        result = subprocess.run(cmd)
        if result.returncode != 0:
            failed.append(test_name)
            print(f"  FAILED (exit code {result.returncode})")

        prev_cov = step_cov

    return failed


def report_coverage(output_dir):
    """Parse cobertura XML and print overall line coverage."""
    cobertura_path = os.path.join(output_dir, "coverage.xml")
    if not os.path.exists(cobertura_path):
        print("No coverage report generated.", file=sys.stderr)
        return None

    tree = ET.parse(cobertura_path)
    root = tree.getroot()
    line_rate = float(root.get("line-rate", "0"))
    percentage = line_rate * 100.0
    print(f"\nOverall line coverage: {percentage:.1f}%")
    return percentage


def main():
    parser = argparse.ArgumentParser(
        description="Run headless tests with OpenCppCoverage")
    parser.add_argument("--opencppcoverage", required=True)
    parser.add_argument("--ctest", required=True)
    parser.add_argument("--source-dir", required=True)
    parser.add_argument("--binary-dir", required=True)
    parser.add_argument("--output-dir", required=True)
    parser.add_argument("--gtest-exclude", action="append", default=[],
                        help="EXE:TestSuite.TestCase to exclude (repeatable)")
    parser.add_argument("--html", action="store_true",
                        help="Generate detailed HTML coverage report")
    parser.add_argument("--min-coverage", type=float, default=0.0,
                        help="Minimum coverage percentage, fail if below")
    args = parser.parse_args()

    gtest_exclude = {}
    for entry in args.gtest_exclude:
        exe, pattern = entry.split(":", 1)
        gtest_exclude.setdefault(exe, []).append(pattern)

    opencppcoverage = args.opencppcoverage
    if not os.path.isfile(opencppcoverage):
        resolved = shutil.which(opencppcoverage)
        if resolved:
            opencppcoverage = resolved
        else:
            print(f"OpenCppCoverage not found: {opencppcoverage}",
                  file=sys.stderr)
            sys.exit(1)
    opencppcoverage = os.path.normpath(opencppcoverage)

    tests = discover_tests(args.ctest, args.binary_dir)
    if not tests:
        print("No headless tests found.", file=sys.stderr)
        sys.exit(1)

    print(f"Found {len(tests)} headless test(s)")

    os.makedirs(args.output_dir, exist_ok=True)

    failed = run_coverage(
        opencppcoverage, tests,
        args.source_dir, args.output_dir,
        gtest_exclude, html=args.html
    )

    percentage = report_coverage(args.output_dir)

    if args.html:
        html_dir = os.path.normpath(
            os.path.join(args.output_dir, "html"))
        print(f"HTML report: {html_dir}")

    if failed:
        print(f"\n{len(failed)} test(s) failed: {', '.join(failed)}",
              file=sys.stderr)
        sys.exit(1)

    if args.min_coverage > 0.0:
        if percentage is None:
            print("Coverage check failed: no report.",
                  file=sys.stderr)
            sys.exit(1)
        if percentage < args.min_coverage:
            print(f"Coverage {percentage:.1f}% is below minimum "
                  f"{args.min_coverage:.1f}%", file=sys.stderr)
            sys.exit(1)
        print(f"Coverage check passed: {percentage:.1f}% >= "
              f"{args.min_coverage:.1f}%")


if __name__ == "__main__":
    main()
