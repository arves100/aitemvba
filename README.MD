# AI-TEM VBA 0.2.1
AI-TEM VBA is a fork of VBA-Link 1.7.2 with AI ability for Street Fighter 3 Zer (GBA).

The emulator also includes Python scripting, which does only supports SF3Z.

Look at mypyScript.py for the script

## Changes since AI-TEM 0.2.1
- Added ability to hook serial cable link under Python (Gb/Gbc only)

## Build instructions (Windows)
Install Python 2.7.16 under C:\Python27 and install NASM, Make sure to enable VS8 integration.

Follow the instructions under (NASM Installation dir)\VSrules\nasm.README for adding nasm rules on vs2008.

Open GBA.sln with VS2008 and build it.

A copy of some parts of Microsoft DirectX SDK are provided for building DirectDraw, DirectSound and Direct3D8.
