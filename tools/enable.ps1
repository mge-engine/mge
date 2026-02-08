param(
    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]]$features
)

# Call this script to enable some properties for MGE
# Usage: enable.ps1 [features]
# Available features:
#   opengl          Enable OpenGL render system
#   vulkan          Enable Vulkan render system
#   dx11|directx11  Enable DirectX11 render system
#   dx12|directx12  Enable DirectX12 render system
#   trace           Enable trace to console
#   renderdoc       Enable RenderDoc recording
#                 
# Example: .\enable.ps1 dx12 trace renderdoc

foreach ($feature in $features) {
    switch -Wildcard ($feature.ToLower()) {
        "opengl" {
            Write-Output "Enable OpenGL render system"
            $env:MGE_RENDER_SYSTEM = "opengl"
        }
        "vulkan" {
            Write-Output "Enable Vulkan render system"
            $env:MGE_RENDER_SYSTEM = "vulkan"
        }
        { $_ -in "dx11", "directx11" } {
            Write-Output "Enable DirectX11 render system"
            $env:MGE_RENDER_SYSTEM = "dx11"
        }
        { $_ -in "dx12", "directx12" } {
            Write-Output "Enable DirectX12 render system"
            $env:MGE_RENDER_SYSTEM = "dx12"
        }
        "trace" {
            Write-Output "Enable trace to console"
            $env:MGE_TRACE_TO_STDOUT = "1"
            $env:MGE_TRACE_ENABLED = "1"
        }
        "renderdoc" {
            Write-Output "Enable RenderDoc support"
            $env:MGE_RENDERDOC_ENABLED = "1"
        }
        default {
            Write-Output "Unknown feature: $feature"
        }
    }
}
