import os
import sys
import subprocess
import shutil

vulkan_version = "1.2.170.0"
vulkan_installer_cache_dir = 'vulkan-installer'

# if not exist $(VULKAN_INSTALLER_CACHE_FOLDER_FOR_CMD)\VulkanSDK-1.1.101.0-Installer.exe


def install_vulkan():
    if os.path.exists(vulkan_installer_cache_dir):
        shutil.rmtree(vulkan_installer_cache_dir)
    os.mkdir('vulkan-installer')
    subprocess.run(["curl", "--show-error", "--output",
                    '%s\VulkanSDK-%s-Installer.exe' % (vulkan_installer_cache_dir, vulkan_version), "https://sdk.lunarg.com/sdk/download/%s/windows/VulkanSDK-%s-Installer.exe?u=" % (vulkan_version, vulkan_version)], )


try:
    install_vulkan()
except:
    print("Exception %s" % str(sys.exc_info()), flush=True)
    sys.exit(1)

sys.exit(0)
