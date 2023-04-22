# Base 32 Codec
Windows application that adds context menu items for files. These items will allow user to encode and decode any file to and from Base32 encoding with RFC 4648 alphabet.

## Installation
Current Windows installer is available on release page. After you install it you can use context menu items (right click any file) to encode / decode file.

## Build Yourself
### Requirements
To build project yourself there are a few requirements:
* Windows SDK installed. You can download it from Microsoft Visual Studio Installer. Select "Classic C++ Applications Development" and install.
* Have some C compiler. I used gcc from MinGW.
* Have NSIS installed
### Build
Build codec: `gcc -mwindows .\main.c .\base32buffer.c .\base32file.c .\util.c -o base32codec.exe`. -mwindows parameter makes executable run without command line.

Build Windows save file GUI: use Visual Studio compiler `cl`. Add required libraries and include path. Here is an example: 

`cl /I"C:\Program Files\Microsoft Visual Studio\<version>\Community\VC\Tools\MSVC\<version>\include" /I"C:\Program Files (x86)\Windows Kits\<version>\Include\<version>\um" /I"C:\Program Files (x86)\Windows Kits\<version>\Include\<version>\shared" /I"C:\Program Files (x86)\Windows Kits\<version>\Include\<version>\ucrt" .\main.cpp  /Fe"base32save.exe" /link /LIBPATH:"C:\Program Files\Microsoft Visual Studio\<version>\Community\VC\Tools\MSVC\<version>\lib\x64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\<version>\Lib\<version>\um\x64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\<version>\Lib\<version>\ucrt\x64"`

Replace \<version\> with version that you installed

Build installer: you can use either console version `makensis .\installer.nsi` (not tested) or open `makensisw.exe` and use window version.
