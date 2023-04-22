!define NAME "Base32Codec"
!define REGPATH_UNINSTSUBKEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}"
!define ESTIMATED_SIZE 238

; The name of the installer
Name "${NAME}"

; The file to write
OutFile "Base32CodecInstaller.exe"

; Request application privileges for Windows Vista and higher
RequestExecutionLevel user

; Build Unicode installer
Unicode True

; User installation directory
InstallDir "$PROFILE\Base32Codec"

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKCU "${REGPATH_UNINSTSUBKEY}" "UninstallString"

;--------------------------------

; Pages

!include "MUI2.nsh"

!define MUI_FINISHPAGE_TEXT "Base32 Encoder and Decoder are installed successfully. If you selected to add Context \
menu items, then to use encoder or decoder right click on a file and click $\"Base32 Encode$\" or $\"Base32 Decode$\". \
Menu asking for destination file will appear."

!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

;--------------------------------

!define USER_PROGRAM_NAME "base32save.exe"
!define PROGRAM_NAME "base32codec.exe"

; The stuff to install
Section "Base32 Codec (required)" SecBase32Codec

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "${PROGRAM_NAME}"
  File "${USER_PROGRAM_NAME}"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKCU "${REGPATH_UNINSTSUBKEY}" "DisplayName" "${NAME}"
  WriteRegStr HKCU "${REGPATH_UNINSTSUBKEY}" "UninstallString" '"$InstDir\uninstall.exe"'
  WriteRegDWORD HKCU "${REGPATH_UNINSTSUBKEY}" "EstimatedSize" ${ESTIMATED_SIZE}
  WriteRegDWORD HKCU "${REGPATH_UNINSTSUBKEY}" "NoModify" 1
  WriteRegDWORD HKCU "${REGPATH_UNINSTSUBKEY}" "NoRepair" 1
  WriteUninstaller "$INSTDIR\uninstall.exe"

  ; Add start menu shortcut for the uninstaller
  CreateShortCut "$SMPROGRAMS\${NAME}\Uninstall Base32 Codec.lnk" "$INSTDIR\uninstall.exe"

SectionEnd

; Create context menu items
!define REGKEYEN1 "Software\Classes\*\shell\Base32Encode"
!define REGKEYEN2 "Software\Classes\*\shell\Base32Encode\command"  
Section "Add Context Menu Encoder" SecCtxtMenuEncoder
  WriteRegStr HKCU "${REGKEYEN1}" "" "Base32 Encode"
  WriteRegStr HKCU "${REGKEYEN2}" "" "$INSTDIR\${USER_PROGRAM_NAME} $\"%1$\""
SectionEnd

!define REGKEYDE1 "Software\Classes\*\shell\Base32Decode"
!define REGKEYDE2 "Software\Classes\*\shell\Base32Decode\command"
!define DECODER_NAME "base32decode.exe"
Section "Add Context Menu Decoder" SecCtxtMenuDecoder
  WriteRegStr HKCU "${REGKEYDE1}" "" "Base32 Decode"
  WriteRegStr HKCU "${REGKEYDE2}" "" "$INSTDIR\${USER_PROGRAM_NAME} $\"%1$\" -d"
SectionEnd

LangString DESC_SecBase32Codec ${LANG_ENGLISH} "Base32 Codec is a simple command line tool for encoding and decoding files in base32 format. It is based on RFC 4648 specification. It is written in C++ and it is portable across all platforms supported by Qt 4.5.0+."
LangString DESC_SecCtxtMenuEncoder ${LANG_ENGLISH} "Adds context menu item for encoding files in base32 format."
LangString DESC_SecCtxtMenuDecoder ${LANG_ENGLISH} "Adds context menu item for decoding files in base32 format."

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecBase32Codec} $(DESC_SecBase32Codec)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecCtxtMenuEncoder} $(DESC_SecCtxtMenuEncoder)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecCtxtMenuDecoder} $(DESC_SecCtxtMenuDecoder)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------

!macro DeleteFileOrAskAbort path
  ClearErrors
  Delete "${path}"
  IfErrors 0 +3
    MessageBox MB_ABORTRETRYIGNORE|MB_ICONSTOP 'Unable to delete "${path}"!' IDRETRY -3 IDIGNORE +2
    Abort "Aborted"
!macroend

; Uninstaller

Section "Uninstall"

  ; Remove files and uninstaller
  !insertmacro DeleteFileOrAskAbort "$InstDir\${PROGRAM_NAME}"
  !insertmacro DeleteFileOrAskAbort "$InstDir\${USER_PROGRAM_NAME}"
  !insertmacro DeleteFileOrAskAbort $INSTDIR\uninstall.exe

  ; Remove start menu shortcut
  Delete "$SMPROGRAMS\${NAME}\Uninstall Base32 Codec.lnk"
  
  ; Remove registry keys
  DeleteRegKey HKCU "${REGPATH_UNINSTSUBKEY}"
  DeleteRegKey HKCU "${REGKEYEN1}"
  DeleteRegKey HKCU "${REGKEYDE1}"

  ; Remove directory (if it's empty)
  RMDir "$INSTDIR"

SectionEnd
