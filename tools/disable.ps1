param(
    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]]$features
)

# Call this script to disable some properties for MGE
# Usage: disable.ps1 [features]
# Available features:
#   trace           Disable trace to console
#   renderdoc       Disable RenderDoc recording
#                 
# Example: .\disable.ps1 trace renderdoc

foreach ($feature in $features) {
    switch -Wildcard ($feature.ToLower()) {
        "trace" {
            Write-Output "Disable trace to console"
            $env:MGE_TRACE_TO_STDOUT = $null
            $env:MGE_TRACE_ENABLED = $null
        }
        "renderdoc" {
            Write-Output "Disable RenderDoc support"
            $env:MGE_RENDERDOC_ENABLED = $null
        }
        default {
            Write-Output "Unknown feature: $feature"
        }
    }
}
