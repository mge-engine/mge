#!/usr/bin/env python3
"""
Compare two sanitized RenderDoc capture XMLs.

Usage: compare_captures.py <expected.xml> <actual.xml>
"""

import sys
import xml.etree.ElementTree as ET
from pathlib import Path


def compare_elements(elem1, elem2, path=""):
    """Recursively compare two XML elements."""
    differences = []
    
    # Compare tags
    if elem1.tag != elem2.tag:
        differences.append(f"{path}: Tag mismatch - expected '{elem1.tag}', got '{elem2.tag}'")
        return differences
    
    current_path = f"{path}/{elem1.tag}" if path else elem1.tag
    
    # Compare attributes
    attrs1 = set(elem1.attrib.keys())
    attrs2 = set(elem2.attrib.keys())
    
    missing_attrs = attrs1 - attrs2
    extra_attrs = attrs2 - attrs1
    
    if missing_attrs:
        differences.append(f"{current_path}: Missing attributes: {', '.join(missing_attrs)}")
    if extra_attrs:
        differences.append(f"{current_path}: Extra attributes: {', '.join(extra_attrs)}")
    
    # Compare common attributes
    for attr in attrs1 & attrs2:
        if elem1.attrib[attr] != elem2.attrib[attr]:
            differences.append(
                f"{current_path}@{attr}: expected '{elem1.attrib[attr]}', got '{elem2.attrib[attr]}'"
            )
    
    # Compare text content
    text1 = (elem1.text or "").strip()
    text2 = (elem2.text or "").strip()
    if text1 != text2:
        differences.append(f"{current_path}: Text mismatch - expected '{text1}', got '{text2}'")
    
    # Compare children
    children1 = list(elem1)
    children2 = list(elem2)
    
    if len(children1) != len(children2):
        differences.append(
            f"{current_path}: Child count mismatch - expected {len(children1)}, got {len(children2)}"
        )
        # Continue comparing up to the minimum length
    
    for i, (child1, child2) in enumerate(zip(children1, children2)):
        child_diffs = compare_elements(child1, child2, current_path)
        differences.extend(child_diffs)
    
    return differences


def compare_xml_files(expected_file, actual_file):
    """Compare two XML files and return list of differences."""
    try:
        tree1 = ET.parse(expected_file)
        tree2 = ET.parse(actual_file)
        
        root1 = tree1.getroot()
        root2 = tree2.getroot()
        
        return compare_elements(root1, root2)
        
    except Exception as e:
        return [f"Error parsing XML: {e}"]


def main():
    if len(sys.argv) != 3:
        print("Usage: compare_captures.py <expected.xml> <actual.xml>")
        sys.exit(1)
    
    expected_file = sys.argv[1]
    actual_file = sys.argv[2]
    
    # Check if files exist
    if not Path(expected_file).exists():
        print(f"Error: Expected file not found: {expected_file}", file=sys.stderr)
        sys.exit(1)
    
    if not Path(actual_file).exists():
        print(f"Error: Actual file not found: {actual_file}", file=sys.stderr)
        sys.exit(1)
    
    print(f"Comparing {expected_file} with {actual_file}...")
    differences = compare_xml_files(expected_file, actual_file)
    
    if not differences:
        print("✓ Captures match!")
        return 0
    else:
        print(f"✗ Found {len(differences)} difference(s):")
        for diff in differences:
            print(f"  {diff}")
        return 1


if __name__ == '__main__':
    sys.exit(main())
