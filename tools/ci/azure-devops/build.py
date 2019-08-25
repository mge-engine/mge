import sys
import time
import os

class Builder:

    def __init__(self):
        pass

    def dumpEnvironment(self):
        for v in os.environ:
            print(v + ": " + os.environ[v])

    def run(self):
        self.dumpEnvironment()
        pass

if __name__ == "__main__":
    builder = Builder()
    builder.run()
    sys.exit(0)
