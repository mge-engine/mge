#!/usr/bin/env pwsh
# Copyright (c) 2026 Alexander Schroeder
# SPDX-License-Identifier: MIT
#
# Cleanup local branches that have been merged via pull requests
#
# Usage:
#   .\cleanup-merged-branches.ps1           # Delete branches with merged PRs
#   .\cleanup-merged-branches.ps1 -DryRun   # Show what would be deleted
#   .\cleanup-merged-branches.ps1 -Force    # Force delete unmerged branches
#
# Requires: git, gh CLI (GitHub CLI)

param(
    [switch]$DryRun = $false,
    [switch]$Force = $false
)

Write-Host "Checking for local branches with merged pull requests..." -ForegroundColor Cyan

# Get all local branches
$local_branches = @(git branch --format='%(refname:short)')
if ($LASTEXITCODE -ne 0) {
    Write-Error "Failed to get local branches"
    exit 1
}

# Get all merged PRs from GitHub
Write-Host "Fetching merged PRs from GitHub..." -ForegroundColor Cyan
$closed_prs_json = gh pr list --state merged --limit 200 --json headRefName
if ($LASTEXITCODE -ne 0) {
    Write-Error "Failed to fetch PRs from GitHub. Make sure gh CLI is installed and authenticated."
    exit 1
}

$closed_prs = $closed_prs_json | ConvertFrom-Json

# Build unique set of branches to delete
$to_delete = @{}
foreach ($pr in $closed_prs) {
    if ($local_branches -contains $pr.headRefName) {
        $to_delete[$pr.headRefName] = $true
    }
}

$unique_to_delete = @($to_delete.Keys | Where-Object { $_ -ne "main" })

if ($unique_to_delete.Count -eq 0) {
    Write-Host "No branches with merged PRs found." -ForegroundColor Green
    exit 0
}

Write-Host "`nFound $($unique_to_delete.Count) branch(es) with merged PRs:" -ForegroundColor Yellow
foreach ($branch in $unique_to_delete) {
    Write-Host "  - $branch" -ForegroundColor Yellow
}

if ($DryRun) {
    Write-Host "`nDry run - no branches deleted." -ForegroundColor Cyan
    exit 0
}

Write-Host ""
$deleted_count = 0
$skipped_count = 0
$failed_count = 0

foreach ($branch in $unique_to_delete) {
    # Check if branch is used by a worktree
    $worktree_info = git worktree list --porcelain | Select-String -Pattern "branch refs/heads/$branch"
    if ($worktree_info) {
        Write-Host "Skipping $branch (in use by worktree)" -ForegroundColor Yellow
        $skipped_count++
        continue
    }

    # Try normal delete first
    git branch -d $branch 2>&1 | Out-Null
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Deleted $branch" -ForegroundColor Green
        $deleted_count++
    }
    else {
        # Try force delete if requested
        if ($Force) {
            git branch -D $branch 2>&1 | Out-Null
            if ($LASTEXITCODE -eq 0) {
                Write-Host "Force deleted $branch" -ForegroundColor Green
                $deleted_count++
            }
            else {
                Write-Host "Failed to delete $branch" -ForegroundColor Red
                $failed_count++
            }
        }
        else {
            Write-Host "Failed to delete $branch (use -Force to override)" -ForegroundColor Red
            $failed_count++
        }
    }
}

Write-Host ""
Write-Host "Summary:" -ForegroundColor Cyan
Write-Host "  Deleted: $deleted_count" -ForegroundColor Green
if ($skipped_count -gt 0) {
    Write-Host "  Skipped: $skipped_count" -ForegroundColor Yellow
}
if ($failed_count -gt 0) {
    Write-Host "  Failed: $failed_count" -ForegroundColor Red
}
