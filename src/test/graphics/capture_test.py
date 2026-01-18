#!/usr/bin/env python3
"""
RenderDoc capture validation tests for blackscreen sample.

Tests run the blackscreen sample with RenderDoc capture enabled,
export frames to XML, sanitize non-deterministic fields, and compare
against golden reference files per backend.
"""

import os
import sys
import subprocess
import time
import shutil
from pathlib import Path
import argparse
import unittest









class BlackscreenCaptureTest(unittest.TestCase):
    """Test fixture for RenderDoc capture validation."""

    @classmethod
    def setUpClass(cls):
        """Set up test environment from arguments."""
        parser = argparse.ArgumentParser()
        parser.add_argument('--blackscreen-exe', required=True, help='Path to blackscreen executable')
        parser.add_argument('--renderdoccmd', required=True, help='Path to renderdoccmd executable')
        parser.add_argument('--sanitize-script', required=True, help='Path to sanitize_capture.py')
        parser.add_argument('--compare-script', required=True, help='Path to compare_captures.py')
        parser.add_argument('--golden-dir', required=True, help='Path to golden reference directory')
        parser.add_argument('--frame-count', type=int, default=5, help='Number of frames to capture')
        parser.add_argument('unittest_args', nargs='*')
        
        args = parser.parse_args()
        
        # Set class attributes
        cls.blackscreen_exe = Path(args.blackscreen_exe)
        cls.renderdoccmd = Path(args.renderdoccmd)
        cls.sanitize_script = Path(args.sanitize_script)
        cls.compare_script = Path(args.compare_script)
        cls.golden_dir = Path(args.golden_dir)
        cls.frame_count = args.frame_count
        cls.build_dir = cls.blackscreen_exe.parent
        
        # Parse remaining args for unittest
        sys.argv[1:] = args.unittest_args

    def run_blackscreen_with_capture(self, backend):
        """Run blackscreen sample with RenderDoc capture enabled."""
        print(f"Running blackscreen with backend: {backend}")
        
        # Create configuration JSON
        config_path = self.build_dir / 'blackscreen_test.json'
        with open(config_path, 'w') as f:
            f.write('{\n')
            f.write('  "graphics": {\n')
            f.write('    "frame_debugger": "renderdoc",\n')
            f.write('    "record_frames": true\n')
            f.write('  }\n')
            f.write('}\n')
        
        try:
            # Set up environment
            env = os.environ.copy()
            env['MGE_RENDER_SYSTEM'] = backend
            env['MGE_CONFIG_FILE'] = str(config_path)
            
            # Launch blackscreen
            process = subprocess.Popen(
                [str(self.blackscreen_exe)],
                cwd=str(self.build_dir),
                env=env,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE
            )
            
            # Wait for capture file to appear or timeout
            start_time = time.time()
            timeout = 10.0
            capture_found = False
            
            while time.time() - start_time < timeout:
                # Check if .rdc file exists
                for entry in self.build_dir.glob('blackscreen*.rdc'):
                    if entry.is_file():
                        capture_found = True
                        break
                
                if capture_found:
                    # Wait a bit more for capture to complete
                    time.sleep(0.5)
                    break
                
                # Check if process is still running
                if process.poll() is not None:
                    stdout, stderr = process.communicate()
                    print(f"Process exited early: {stderr.decode()}")
                    break
                
                time.sleep(0.1)
            
            # Terminate process
            try:
                process.terminate()
                process.wait(timeout=5)
            except subprocess.TimeoutExpired:
                process.kill()
                process.wait()
            
            return capture_found
            
        finally:
            # Cleanup config file
            if config_path.exists():
                config_path.unlink()

    def find_latest_capture(self):
        """Find the most recently created blackscreen capture file."""
        captures = list(self.build_dir.glob('blackscreen*.rdc'))
        if not captures:
            return None
        
        # Return the newest by modification time
        return max(captures, key=lambda p: p.stat().st_mtime)

    def run_python_script(self, script, args):
        """Run a Python script and return success status."""
        cmd = [sys.executable, str(script)] + [str(arg) for arg in args]
        print(f"Running: {' '.join(cmd)}")
        
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.stdout:
            print(result.stdout)
        if result.stderr:
            print(result.stderr, file=sys.stderr)
        
        return result.returncode == 0

    def test_backend_capture(self, backend):
        """Test RenderDoc capture for a specific backend."""
        # Run blackscreen with capture
        self.assertTrue(
            self.run_blackscreen_with_capture(backend),
            f"Failed to capture frames for {backend}"
        )
        
        # Find the capture file
        capture_file = self.find_latest_capture()
        self.assertIsNotNone(capture_file, f"No capture file found for {backend}")
        print(f"Found capture: {capture_file}")
        
        # Sanitize the capture
        sanitized_xml = self.build_dir / f"{backend}_sanitized.xml"
        self.assertTrue(
            self.run_python_script(self.sanitize_script, [
                self.renderdoccmd,
                capture_file,
                sanitized_xml
            ]),
            f"Failed to sanitize capture for {backend}"
        )
        
        # Compare with golden reference
        golden_xml = self.golden_dir / f"{backend}.xml"
        
        if not golden_xml.exists():
            print(f"WARNING: Golden reference not found: {golden_xml}")
            print(f"Skipping comparison - create baseline first")
            print(f"Sanitized XML saved to: {sanitized_xml}")
            # Don't fail the test, just skip comparison
            return
        
        comparison_passed = self.run_python_script(self.compare_script, [
            golden_xml,
            sanitized_xml
        ])
        
        # Cleanup on success, keep on failure for debugging
        if comparison_passed:
            capture_file.unlink()
            sanitized_xml.unlink()
            print(f"Capture test passed for {backend}, files cleaned up")
        else:
            print(f"Capture comparison failed for {backend}")
            print(f"Keeping files for inspection:")
            print(f"  Capture: {capture_file}")
            print(f"  Sanitized: {sanitized_xml}")
        
        self.assertTrue(comparison_passed, f"Capture comparison failed for {backend}")

    def test_dx11(self):
        """Test DirectX 11 capture."""
        self.test_backend_capture('dx11')

    def test_dx12(self):
        """Test DirectX 12 capture."""
        self.test_backend_capture('dx12')

    def test_opengl(self):
        """Test OpenGL capture."""
        self.test_backend_capture('opengl')

    def test_vulkan(self):
        """Test Vulkan capture."""
        self.test_backend_capture('vulkan')


if __name__ == '__main__':
    unittest.main()
