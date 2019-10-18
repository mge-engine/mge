# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
import sys
import time
import os
import subprocess

VCPKG_SOURCE="https://github.com/mge-engine/vcpkg.git"
VCPKG_GITHASH="e417ff69b746f7842b3b9d0fceacb080498e1c5d"
VCPKG_PACKAGES=["sqlite3", "lua", "boost-filesystem", "boost-any", "boost-lexical-cast", "boost-math", "boost-algorithm", "boost-variant", "glm", "vulkan", "entt", "stb"]

class Unbuffered(object):
   def __init__(self, stream):
       self.stream = stream
   def write(self, data):
       self.stream.write(data)
       self.stream.flush()
   def writelines(self, datas):
       self.stream.writelines(datas)
       self.stream.flush()
   def __getattr__(self, attr):
       return getattr(self.stream, attr)

class Builder:

    def __init__(self):
        self.sourcesDirectory = os.environ["BUILD_SOURCESDIRECTORY"]
        self.binariesDirectory = os.environ["BUILD_BINARIESDIRECTORY"]
        self.agentBuildDirectory = os.environ["AGENT_BUILDDIRECTORY"]
        self.buildDirectory = os.path.join(self.sourcesDirectory, "build")
        self.vcpkgDirectory = os.path.join(self.buildDirectory, "vcpkg")
        self.plainEnvironment = os.environ.copy()
        self.compileEnvironment = dict()
        self.vcpkgEnvironment = dict() 
        for k in self.plainEnvironment:
            if not k.startswith("VCPKG"):
                if k == "PATH":
                    self.vcpkgEnvironment["PATH"] = "%s;%s" % (self.vcpkgDirectory, self.plainEnvironment["PATH"])
                else:
                    self.vcpkgEnvironment[k]=self.plainEnvironment[k]
        self.vcpkgEnvironment["VCPKG_PUBLIC_ABI_OVERRIDE"]="NO"

    def createDirectories(self):
        if not os.path.exists(self.buildDirectory):
            os.makedirs(self.buildDirectory)
            print("Creating directory %s" % self.buildDirectory)

    def dumpEnvironment(self):
        for v in os.environ:
            print(v + ": " + os.environ[v])


    def installVcpkg(self):
        if not os.path.exists(self.vcpkgDirectory):
            self.exec(["git", "clone", VCPKG_SOURCE], cwd=self.buildDirectory)
        if not os.path.exists(os.path.join(self.vcpkgDirectory, "vcpkg.exe")):
            self.exec(["bootstrap-vcpkg.bat", "-win64"], env=self.vcpkgEnvironment, cwd=self.vcpkgDirectory)

    def installVcpkgPackages(self):
        command = ["vcpkg.exe", "--vcpkg-root", self.vcpkgDirectory, "--triplet", "x64-windows", "install"] 
        command.extend(VCPKG_PACKAGES)
        self.exec(command, env=self.vcpkgEnvironment, cwd=self.vcpkgDirectory, shell=True)

    def captureCompileEnvironment(self):
        command = ["vcpkg.exe", "--vcpkg-root", self.vcpkgDirectory, "--triplet", "x64-windows", "env", "set"] 
        compileEnvString = subprocess.check_output(command, env=self.vcpkgEnvironment, cwd=self.vcpkgDirectory, shell=True)
        compileEnvString = compileEnvString.decode("utf-8")
        
        lines = compileEnvString.split("\r\n")
        for l in lines:
            try:
                (key, value) = l.split("=")
                self.compileEnvironment[key]=value
            except:
                pass
            
        self.compileEnvironment["CC"]="cl"
        self.compileEnvironment["CXX"]="cl"

    def configure(self):
        toolChain = os.path.join(self.vcpkgDirectory,  "scripts", "buildsystems", "vcpkg.cmake")
        toolChainOption = "-DCMAKE_TOOLCHAIN_FILE=%s" % toolChain
        command = ["C:\\ProgramData\\chocolatey\\bin\\cmake.exe", "-GNinja", "..", "-DCMAKE_BUILD_TYPE=RelWithDebInfo", "-DHEADLESS_ENVIRONMENT=ON", toolChainOption]
        self.exec(command, env=self.compileEnvironment, cwd=self.buildDirectory, shell=True)

    def build(self, target):
        command = ["C:\\ProgramData\\chocolatey\\bin\\ninja.exe", target]
        self.exec(command, env=self.compileEnvironment, cwd=self.buildDirectory, shell=True)

    def exec(self, command, env=None, cwd=None, shell=False):
        print("EXEC: %s" % " ".join(command))
        useShell = shell
        environment = self.plainEnvironment
        if env is not None:
            environment = env
        workingDir = self.buildDirectory
        if cwd is not None:
            workingDir = cwd
        print("CWD: %s" % workingDir)
        executedProcess = subprocess.Popen(command, env=environment, cwd=workingDir, shell=useShell)
        executedProcess.wait()
        if executedProcess.returncode != 0:
            raise Exception("%s returned %d" % (" ".join(command), executedProcess.returncode))

    def run(self):
        self.createDirectories()
        self.installVcpkg()
        self.installVcpkgPackages()
        self.captureCompileEnvironment()
        self.configure()
        self.build("all")
        self.build("test")
        pass

if __name__ == "__main__":
    sys.stdout = Unbuffered(sys.stdout)
    builder = Builder()
    builder.run()
    sys.exit(0)
