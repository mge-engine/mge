#!/usr/bin/env python3
"""
Sanitize RenderDoc capture XML by removing non-deterministic fields.

Usage: sanitize_capture.py <renderdoccmd> <input.rdc> <output.xml>
"""

import sys
import os
import subprocess
import xml.etree.ElementTree as ET
import re
from pathlib import Path


def convert_to_xml(renderdoccmd, rdc_file, xml_file):
    """Convert RenderDoc capture to XML using renderdoccmd."""
    cmd = [renderdoccmd, 'convert', '-f', 'xml', rdc_file, xml_file]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Error converting capture: {result.stderr}", file=sys.stderr)
        return False
    return True


def sanitize_xml(xml_file, sanitized_file):
    """Sanitize XML by removing timestamps, addresses, and frame IDs."""
    try:
        tree = ET.parse(xml_file)
        root = tree.getroot()
        
        # Remove or normalize non-deterministic elements
        # Patterns to sanitize:
        # - Timestamps: any element with 'time', 'timestamp' in name/attribute
        # - Memory addresses: hex patterns like 0x[0-9a-fA-F]+
        # - Frame numbers/IDs in attributes
        # - Driver version strings
        # - GPU-specific data that varies between runs
        
        def sanitize_element(elem):
            # Sanitize attributes
            for attr in list(elem.attrib.keys()):
                value = elem.attrib[attr]
                
                # Remove timestamp attributes
                if 'time' in attr.lower() or 'timestamp' in attr.lower():
                    del elem.attrib[attr]
                    continue
                
                # Sanitize memory addresses
                if re.search(r'0x[0-9a-fA-F]{8,}', value):
                    elem.attrib[attr] = re.sub(r'0x[0-9a-fA-F]{8,}', '0xSANITIZED', value)
                
                # Sanitize frame IDs (keep structure but normalize values)
                if 'frameid' in attr.lower() or 'frameno' in attr.lower():
                    elem.attrib[attr] = '0'
                
                # Normalize floating point precision to 6 decimal places
                try:
                    if '.' in value:
                        float_val = float(value)
                        elem.attrib[attr] = f"{float_val:.6f}"
                except ValueError:
                    pass
            
            # Sanitize text content
            if elem.text:
                # Sanitize memory addresses in text
                elem.text = re.sub(r'0x[0-9a-fA-F]{8,}', '0xSANITIZED', elem.text)
                
                # Normalize floating point numbers
                def normalize_float(match):
                    try:
                        return f"{float(match.group(0)):.6f}"
                    except:
                        return match.group(0)
                
                elem.text = re.sub(r'\d+\.\d+', normalize_float, elem.text)
            
            # Recursively sanitize children
            for child in elem:
                sanitize_element(child)
        
        sanitize_element(root)
        
        # Write sanitized XML
        tree.write(sanitized_file, encoding='utf-8', xml_declaration=True)
        return True
        
    except Exception as e:
        print(f"Error sanitizing XML: {e}", file=sys.stderr)
        return False


def main():
    if len(sys.argv) != 4:
        print("Usage: sanitize_capture.py <renderdoccmd> <input.rdc> <output.xml>")
        sys.exit(1)
    
    renderdoccmd = sys.argv[1]
    rdc_file = sys.argv[2]
    sanitized_file = sys.argv[3]
    
    # Check if input file exists
    if not os.path.exists(rdc_file):
        print(f"Error: Input file not found: {rdc_file}", file=sys.stderr)
        sys.exit(1)
    
    # Convert to XML first (temporary file)
    temp_xml = Path(rdc_file).with_suffix('.xml')
    
    print(f"Converting {rdc_file} to XML...")
    if not convert_to_xml(renderdoccmd, rdc_file, str(temp_xml)):
        sys.exit(1)
    
    print(f"Sanitizing XML...")
    if not sanitize_xml(str(temp_xml), sanitized_file):
        sys.exit(1)
    
    # Clean up temporary XML
    if temp_xml.exists():
        temp_xml.unlink()
    
    print(f"Sanitized XML written to {sanitized_file}")
    return 0


if __name__ == '__main__':
    sys.exit(main())
