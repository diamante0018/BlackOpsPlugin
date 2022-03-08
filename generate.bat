@echo off
echo Updating submodules...
call git submodule update --init --recursive
tools\premake5 %* vs2022
