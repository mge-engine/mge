import os
import sys
import subprocess
import time
import shutil
from pathlib import Path
import argparse 
import json
import xml.etree.ElementTree as ET

def write_config(test_dir, exe_name, render_system="opengl", stop_cycle=5):
    """Write configuration file for blackscreen test."""
    config_path = test_dir / f'test_{exe_name}.json'
    config_data = {
        "graphics": {
            "frame_debugger": "renderdoc",
            "record_frames": True,
            "render_system": render_system
        },
        "application": {
            "stop_at_cycle": stop_cycle
        }
    }
    
    with open(config_path, 'w') as f:
        json.dump(config_data, f, indent=4)
    print(f"Wrote configuration to {config_path}")


class Sanitizer:

    def __init__(self, xml_file, output_file):
        self.tree = ET.parse(xml_file)
        self.root = self.tree.getroot()
        self.output_lines = []
        self.frame = 0
        self.frame_call = 0
        self.output_file = output_file
        self.resource_ids = {}      # typename -> {raw_value -> sequence_number}
        self.resource_counters = {} # typename -> next sequence number
        self.current_chunk_name = "" # Track current chunk for conditional filtering

    def output(self, line=""):
        self.output_lines.append(line)

    def sanitize_header(self, header_elem):
        self.output("# Header")
        for child in header_elem:
            if child.tag == "driver":
                self.output(f"- Driver: {child.text}")

    def resource_id_value(self, typename, raw_value):
        if raw_value == "0":
            return "null"
        if typename not in self.resource_ids:
            self.resource_ids[typename] = {}
            self.resource_counters[typename] = 0
        if raw_value not in self.resource_ids[typename]:
            self.resource_counters[typename] += 1
            self.resource_ids[typename][raw_value] = self.resource_counters[typename]
        seq = self.resource_ids[typename][raw_value]
        return f"{typename} #{seq}"

    def struct_value(self, param):
        fields = []
        for child in param:
            child_hidden = child.attrib.get("hidden", "false")
            if child_hidden == "true":
                continue
            name = child.attrib.get("name", "unknown")
            # Skip size field in Vulkan memory writes (non-deterministic in RenderDoc)
            if (self.current_chunk_name == "Internal::Coherent Mapped Memory Write" and 
                name == "size"):
                continue
            value = self.parameter_value(child)
            fields.append(f"{name}: {value}")
        return "{" + ", ".join(fields) + "}"

    def array_value(self, param):
        elements = list(param)
        if not elements:
            return "[]"
        values = []
        for child in elements:
            child_hidden = child.attrib.get("hidden", "false")
            if child_hidden == "true":
                continue
            values.append(self.parameter_value(child))
        return "[" + ", ".join(values) + "]"

    def parameter_value(self, param):
        if param.tag == "float":
            return param.text
        elif param.tag == "null":
            return "null"
        elif param.tag == "int":
            return param.text
        elif param.tag == "uint":
            return param.text
        elif param.tag == "string":
            return param.text
        elif param.tag == "bool":
            return param.text
        elif param.tag == "ResourceId":
            typename = param.get("typename", "ResourceId")
            raw_value = param.text or "0"
            return self.resource_id_value(typename, raw_value)
        elif param.tag == "array":
            return self.array_value(param)
        elif param.tag == "struct":
            return self.struct_value(param)
        elif param.tag == "buffer":
            return "_ignored_"
        elif param.tag == "enum":
            return param.get("string", "_unknown_")
        else:
            raise Exception(f"Unknown parameter type: {param}")
        
    def parameter(self, param):
        param_name = param.attrib.get("name", "unknown")
        param_hidden = param.attrib.get("hidden", "false")
        if param_hidden == "true":
            return
        param_value = self.parameter_value(param)
        self.output(f"  - {param_name}: {param_value}")

    def frame_chunk(self, chunk):
        if self.frame_call == 0:
            self.output(f"## Frame {self.frame}")
        self.frame_call += 1
        chunk_name = chunk.attrib.get("name", "unknown")
        self.current_chunk_name = chunk_name  # Track for filtering
        self.output(f"- Call: {chunk_name}")
        for param in chunk:
            self.parameter(param)
        end_of_frame_chunks = ["IDXGISwapChain::Present", "SwapBuffers", "vkQueuePresentKHR"]
        if chunk_name in end_of_frame_chunks:
            self.frame += 1
            self.frame_call = 0

    def sanitize_chunks(self, chunks_elem):
        self.output("# Frames")
        for chunk in chunks_elem:
            chunk_name = chunk.attrib.get("name", "unknown")
            if chunk_name == "Internal::Beginning of Capture":
                self.frame = 1
                self.frame_call = 0 
            elif chunk_name == "Internal::End of Capture":
                self.frame = 0
            else:
                if self.frame > 0:
                    self.frame_chunk(chunk)

    def sanitize(self):
        for child in self.root:
            if child.tag == "header":
                self.sanitize_header(child)
            elif child.tag == "chunks":
                self.sanitize_chunks(child)
        with open(self.output_file, 'w') as f:
            for line in self.output_lines:
                f.write(line + "\n")
            f.write("\n")


if __name__ == '__main__':
    optparser = argparse.ArgumentParser()
    optparser.add_argument("--test", required=True, help="Path to the test executable")
    optparser.add_argument("--renderdoccmd", required=True, help="Path to renderdoccmd executable")
    optparser.add_argument("--reference-dir", required=True, help="Path to golden reference directory")
    optparser.add_argument("--frame-count", type=int, required=True, help="Number of frames to capture")
    optparser.add_argument("--generate-reference", type=bool, required=False, help="Whether to generate reference captures")
    optparser.add_argument("--render-system", required=True, help="Which render system to use (dx11, dx12, vulkan, opengl)")
    options = optparser.parse_args()

    # run the test 
    test_exe = Path(options.test)
    if not test_exe.exists():
        print(f"Error: Test executable not found: {test_exe}", file=sys.stderr)
        sys.exit(1)
    if not test_exe.is_file():
        print(f"Error: Test executable is not a file: {test_exe}", file=sys.stderr)
        sys.exit(1)
 
    test_dir = test_exe.parent.resolve()
    print(f"Changing working directory to: {test_dir}")
    print(f"Write config for test executable: {test_exe.stem}")
    write_config(test_dir, test_exe.stem, render_system=options.render_system, stop_cycle=options.frame_count)   
    print(f"Running test executable: {test_exe}")
    rdc_file = test_dir / f"{test_exe.stem}_capture.rdc"
    if rdc_file.exists():
        print(f"Removing existing capture file: {rdc_file}")
        rdc_file.unlink()
    test_env = os.environ.copy()
    test_env["MGE_RENDERDOC_LIBRARY_PATH"] = str(Path(options.renderdoccmd).resolve().parent)
    test_env.pop("MGE_RENDER_SYSTEM", None)
    process = subprocess.Popen([str(test_exe), "--config-name", f"test_{test_exe.stem}"], cwd=test_dir, env=test_env)
    process.wait()
    if process.returncode != 0:
        print(f"Error: Test executable exited with code {process.returncode}", file=sys.stderr)
        sys.exit(1)
    print("Test executable completed successfully.")
    rdc_file = test_dir / f"{test_exe.stem}_capture.rdc"
    if not rdc_file.exists():
        print(f"Error: Capture file not found: {rdc_file}", file=sys.stderr)
        sys.exit(1)
    print(f"Found capture file: {rdc_file}")
    # convert capture to xml
    xml_file = test_dir / f"{test_exe.stem}_{options.render_system}_capture.xml"
    if xml_file.exists():
        print(f"Removing existing XML file: {xml_file}")
        xml_file.unlink()   
    print(f"Converting capture to XML: {xml_file}")
    convert_process = subprocess.Popen([str(options.renderdoccmd), "convert", "-f" , str(rdc_file), "-o", str(xml_file), "-i", "rdc", "-c", "xml"], cwd=test_dir)
    convert_process.wait()
    if convert_process.returncode != 0:
        print(f"Error: Conversion to XML failed with code {convert_process.returncode}", file=sys.stderr)
        sys.exit(1)
    print("Conversion to XML completed successfully.")
    if not xml_file.exists():
        print(f"Error: XML file not found after conversion: {xml_file}", file=sys.stderr)
        sys.exit(1)  
    print(f"Sanitizing XML file: {xml_file}")
    sanitized_file = test_dir / f"{test_exe.stem}_{options.render_system}.md"
    sanitizer = Sanitizer(xml_file, sanitized_file)
    sanitizer.sanitize()
    if not sanitized_file.exists():
        print(f"Error: Sanitized file not found: {sanitized_file}", file=sys.stderr)
        sys.exit(1)
    if options.generate_reference:
        reference_dir = Path(options.reference_dir)
        if not reference_dir.exists():
            print(f"Creating reference directory: {reference_dir}")
            reference_dir.mkdir(parents=True, exist_ok=True)
        reference_file = reference_dir / f"{test_exe.stem}_{options.render_system}.md"
        print(f"Generating reference file: {reference_file}")
        shutil.copyfile(sanitized_file, reference_file)
        print("Reference file generated successfully.")
    else:
        reference_dir = Path(options.reference_dir)
        reference_file = reference_dir / f"{test_exe.stem}_{options.render_system}.md"
        if not reference_file.exists():
            print(f"Error: Reference file not found: {reference_file}", file=sys.stderr)
            sys.exit(1)
        print(f"Comparing sanitized file with reference: {reference_file}")
        with open(sanitized_file, 'r') as f1, open(reference_file, 'r') as f2:
            sanitized_lines = f1.readlines()
            reference_lines = f2.readlines()
        differences = []
        for i, (line1, line2) in enumerate(zip(sanitized_lines, reference_lines)):
            if line1 != line2:
                differences.append(f"Line {i+1} differs:\n  Sanitized: {line1}  Reference: {line2}")
        if len(sanitized_lines) != len(reference_lines):
            differences.append(f"File length differs: Sanitized has {len(sanitized_lines)} lines, Reference has {len(reference_lines)} lines.")
        if not differences:
            print("SUCCESS: Captures match the reference!")
            sys.exit(0)
        else:
            print(f"FAIL: Found {len(differences)} difference(s):")
            for diff in differences:
                print(diff)
            sys.exit(1)