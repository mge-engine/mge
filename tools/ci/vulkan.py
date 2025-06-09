# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
import os
import sys
import subprocess
import shutil

vulkan_version = "1.4.313.1"
vulkan_installer_cache_dir = 'vulkan-installer'
vulkan_installer = '%s\VulkanSDK-%s-Installer.exe' % (
    vulkan_installer_cache_dir, vulkan_version)


def run_process(cmd):
    cmd_string = " ".join(cmd)
    print("Running command: {}".format(cmd_string), flush=True)
    return subprocess.run(cmd)


def vulkan_installer_exists():
    return os.path.exists(vulkan_installer)


def vulkan_installed():
    return os.path.exists("C:\\VulkanSDK\%s" % vulkan_version)


def download_vulkan():
    if os.path.exists(vulkan_installer_cache_dir):
        shutil.rmtree(vulkan_installer_cache_dir)
    os.mkdir('vulkan-installer')
    print("Downloading vulkan installer", flush=True)
    run_process(["curl", "--show-error", "--output",
                 '%s\VulkanSDK-%s-Installer.exe' % (vulkan_installer_cache_dir, vulkan_version), "https://sdk.lunarg.com/sdk/download/%s/windows/VulkanSDK-%s-Installer.exe?Human=true&u=" % (vulkan_version, vulkan_version)], )


def install_vulkan():
    print("Run vulkan installer", flush=True)
    print("Installer: {}".format(vulkan_installer), flush=True)
    run_process([vulkan_installer, "in", "-d",
                 "-d", "--al", "--da", "-c"])


try:
    if not vulkan_installed():
        if not vulkan_installer_exists():
            download_vulkan()
        install_vulkan()
    else:
        print("Vulkan is already installed", flush=True)
except:
    print("Exception %s" % str(sys.exc_info()), flush=True)
    sys.exit(1)

sys.exit(0)
