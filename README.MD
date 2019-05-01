# AI-TEM VBA 0.2.1
AI-TEM VBA is a fork of VBA-Link 1.7.2 with AI ability for Street Fighter 3 Zer (GBA).

The emulator also includes Python scripting, which does only supports SF3Z.

Look at mypyScript.py for the script

## Changes since AI-TEM 0.2
### 0.2.1
- Added ability to hook serial cable link under Python (Gb/Gbc only)
- Added ability to call a function when the python script gets destroyed
- Updated to Visual Studio 2008
- Added error check when the script is imported

## API
### Functions
| Name | Arguments | Description | Return |
| ---- | --------- | ----------- | ------ |
| Main | Reference to CSz3z class | Called when the script is loaded | Exit code |
| Link | unsigned char input | Called when a serial link is performed | Output to the link transfer |
| Destroy | None | Called when the script is destroyed | None |
| Redirection | None | Called when the script is created, before the main, usefull for hooking the output to a file | None |

## Build instructions (Windows)
Install Python 2.7.16 under C:\Python27 and install NASM, Make sure to enable VS8 integration.

Follow the instructions under (NASM Installation dir)\VSrules\nasm.README for adding nasm rules on vs2008.

Open GBA.sln with VS2008 and build it.

A copy of some parts of Microsoft DirectX SDK are provided for building DirectDraw, DirectSound and Direct3D8.

## Stuff that would be nice to do
- Add changes from AGAI, AAI versions
- Switch to Python 3
- Use a newer branch of VBA (I will fork VBA-M later and add the support on it)
- Make a generic python interface (not only for SF3Z)
- Move to Visual Studio 2019
- Make Open dialog for actions like Script and so on
