@echo off
rem Switch to a versioned branch, incrementing the highest existing version
rem Usage: switchbranch.cmd <base-branch-name>
rem Example: switchbranch.cmd main
rem   If main.002 exists, switches to main.003

powershell -ExecutionPolicy Bypass -File "%~dp0switchbranch.ps1" %*
