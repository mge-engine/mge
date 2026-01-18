import os
import sys
import subprocess
import time
import shutil
from pathlib import Path
import argparse 







if __name__ == '__main__':
    optparser = argparse.ArgumentParser()
    optparser.add_argument("--test", required=True, help="Path to the test executable")
    optparser.add_argument("--renderdoccmd", required=True, help="Path to renderdoccmd executable")
    optparser.add_argument("--reference-dir", required=True, help="Path to golden reference directory")
    optparser.add_argument("--frame-count", type=int, required=True, help="Number of frames to capture")
    optparser.add_argument("--generate-reference", type=bool, required=True, help="Whether to generate reference captures")
    optparser.add_argument("--render-system", required=True, help="Which render system to use (dx11, dx12, vulkan, opengl)")
    options = optparser.parse_args()

