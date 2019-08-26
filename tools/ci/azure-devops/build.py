import sys
import time
import os
import subprocess

VCPKG_SOURCE="https://github.com/mge-engine/vcpkg.git"

class Builder:

    def __init__(self):
        self.sourcesDirectory = os.environ["BUILD_SOURCESDIRECTORY"]
        self.binariesDirectory = os.environ["BUILD_BINARIESDIRECTORY"]
        self.agentBuildDirectory = os.environ["AGENT_BUILDDIRECTORY"]
        self.buildDirectory = os.path.join(self.sourcesDirectory, "build")
        self.vcpkgDirectory = os.path.join(self.buildDirectory, "vcpkg")
        self.plainEnvironment = os.environ.copy()

    def createDirectories(self):
        if not os.path.exists(self.buildDirectory):
            os.makedirs(self.buildDirectory)
            print("Creating directory %s" % self.buildDirectory)

    def dumpEnvironment(self):
        for v in os.environ:
            print(v + ": " + os.environ[v])


    def installVcpkg(self):
        if os.path.exists(self.vcpkgDirectory):
            self.exec(["git", "fetch"], cwd=self.vcpkgDirectory)
        else:
            self.exec(["git", "clone", "VCPKG_SOURCE"], cwd=self.buildDirectory)

    def exec(self, command, env=None, cwd=None):
        environment = self.plainEnvironment
        if env is not None:
            environment = env
        workingDir = self.buildDirectory
        if cwd is not None:
            workingDir = cwd
        executedProcess = subprocess.Popen(command, env=environment, cwd=workingDir)
        executedProcess.wait()
        if executedProcess.returncode != 0:
            raise Exception("%s returned %d" % (" ".join(command), executedProcess.returncode))

    def run(self):
        self.createDirectories()
        self.installVcpkg()
        pass

if __name__ == "__main__":
    builder = Builder()
    builder.run()
    sys.exit(0)
