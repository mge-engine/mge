param(
    [Parameter(Mandatory = $true)]
    [string]$BaseBranch
)

# Get all local branches
$branches = git branch | ForEach-Object { $_.Trim().TrimStart('* ') }

# Find all matching branches with pattern base.###
$pattern = "^$([regex]::Escape($BaseBranch))\.(\d{3})$"
$maxNum = 0

foreach ($branch in $branches) {
    if ($branch -match $pattern) {
        $num = [int]$matches[1]
        if ($num -gt $maxNum) {
            $maxNum = $num
        }
    }
}

# Increment to next version
$nextNum = $maxNum + 1
$newBranch = "$BaseBranch.{0:D3}" -f $nextNum

Write-Host "Switching to branch: $newBranch"
git checkout -b $newBranch

if ($LASTEXITCODE -ne 0) {
    Write-Error "Failed to create branch $newBranch"
    exit 1
}
