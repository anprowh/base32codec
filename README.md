# Base 32 Codec
Windows application that adds context menu items for files. These items will allow user to encode and decode any file to and from Base32 encoding with RFC 4648 alphabet.

## Installation
Current Windows installer is available on release page. After you install it you can use context menu items (right click any file) to encode / decode file.

## Build Yourself
### Requirements
To build project yourself there are a few requirements:
* OS Windows. (tested on Windows 11. May work with other OS, but no guide available)
* Windows SDK installed. You can download it from Microsoft Visual Studio Installer. Select "Classic C++ Applications Development" and install.
* Have some C compiler. I used gcc from MinGW.
* Have NSIS installed

### Build
Move to build directory `cd build`

Build codec: `gcc -mwindows ..\src\main.c ..\src\base32buffer.c ..\src\base32file.c ..\src\util.c -o base32codec.exe`. `-mwindows` parameter makes executable run without command line.

Build Windows "save file" GUI: use Visual Studio compiler `cl`. Add required libraries and include path. Here is an example: 

`cl /I"C:\Program Files\Microsoft Visual Studio\<version>\Community\VC\Tools\MSVC\<version>\include" /I"C:\Program Files (x86)\Windows Kits\<version>\Include\<version>\um" /I"C:\Program Files (x86)\Windows Kits\<version>\Include\<version>\shared" /I"C:\Program Files (x86)\Windows Kits\<version>\Include\<version>\ucrt" ..\src\main.cpp  /Fe"base32save.exe" /link /LIBPATH:"C:\Program Files\Microsoft Visual Studio\<version>\Community\VC\Tools\MSVC\<version>\lib\x64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\<version>\Lib\<version>\um\x64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\<version>\Lib\<version>\ucrt\x64"`

Replace \<version> with version that you installed

Build installer: you can use either console version `makensis /NOCD ..\src\installer.nsi` (NOCD to get exe files from current directory) or open `makensisw.exe` and use window version. If you use `makensisw`, you need to move/copy `installer.nsi` from src to build.


Build tests: `gcc -I ..\include\Check\include ..\src\base32buffer.c ..\src\base32file.c ..\src\util.c ..\tests\test_file.c -L..\include\Check\lib -lcheck -static -o run_tests.exe`

### Run
If you made the installer you can just install program using `Base32CodecInstaller.exe` and use file context menu to encode/decode files

If you want to run programs from command line here is a guide:
* Encode file to Base32: `.\base32codec.exe <input_file> <output_encoded_file>`
* Decode file from Base32: `.\base32codec.exe <input_file> <output_decoded_file> -d`
* Encode file to Base32 with "save file" dialog: `.\base32save.exe <input_file>`
* Decode file from Base32 with "save file" dialog: `.\base32save.exe <input_file> -d`

Note that when using `base32codec.exe` you will not see output in console if program was compiled with `-mwindows`.
