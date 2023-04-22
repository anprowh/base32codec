#pragma comment(lib, "user32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "shlwapi.lib")
#include <Windows.h>
#include <ShObjIdl.h>
#include <Shlwapi.h>
#include <stdlib.h>
#include <shtypes.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
        COINIT_DISABLE_OLE1DDE);

    // Get command line arguments
    int argc = 0;
    LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argc < 2)
    {
        MessageBoxW(NULL, L"Please provide a filename as an argument", L"Error", MB_OK|MB_ICONERROR);
        return 0;
    }

    // Get full path of input file to make sure it works with CreateProcessW
    LPWSTR pszInputFilePath = _wfullpath(NULL, argv[1], 0);

    // Make sure the file exists
    if (PathFileExistsW(pszInputFilePath) == FALSE)
    {
        MessageBoxW(NULL, L"File does not exist", L"Error", MB_OK|MB_ICONERROR);
        return 0;
    }


    if (SUCCEEDED(hr))
    {
        IFileSaveDialog *pFileSave;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
                IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

        pFileSave->SetDefaultExtension(L"b32");
        pFileSave->SetFileName(L"output");COMDLG_FILTERSPEC rgSpec[] =
        {
            {L"All Files", L"*.*"}
        };
        pFileSave->SetFileTypes(ARRAYSIZE(rgSpec), rgSpec);

        if (SUCCEEDED(hr))  
        {
            // Show the Open dialog box.
            hr = pFileSave->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileSave->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        // Needed for CreateProcessW
                        STARTUPINFOW si = { sizeof(si) };
                        PROCESS_INFORMATION pi;
                        ZeroMemory(&si, sizeof(si));
                        ZeroMemory(&pi, sizeof(pi));
                        LPSTARTUPINFOW psi = &si;
                        // Create argument string for base32codec.exe
                        LPWSTR pszArg;
                        // Get length of argument string
                        SIZE_T cbArg = (wcslen(L" \"") + wcslen(pszInputFilePath) +
                             wcslen(L"\" \"") + wcslen(pszFilePath) + wcslen(L"\"") + 1) * sizeof(WCHAR);
                        if (argc >= 3)
                        {
                            cbArg += (wcslen(L" ") + wcslen(argv[2])) * sizeof(WCHAR);
                        }
                        pszArg = (LPWSTR)CoTaskMemAlloc(cbArg * sizeof(WCHAR));
                        wcscpy(pszArg, L" \"");
                        wcscat(pszArg, pszInputFilePath);
                        wcscat(pszArg, L"\" \"");
                        wcscat(pszArg, pszFilePath);
                        wcscat(pszArg, L"\"");
                        if (argc >= 3)
                        {
                            wcscat(pszArg, L" ");
                            wcscat(pszArg, argv[2]);
                        }

                        // Construct path to base32codec.exe
                        // Take folder of main and append base32codec.exe
                        LPWSTR pszBase32CodecPath;
                        // Copy argv[0] to pszBase32SaveFolder
                        LPWSTR pszBase32SaveFolder = (LPWSTR)CoTaskMemAlloc((wcslen(argv[0]) + 1) * sizeof(WCHAR));
                        wcscpy(pszBase32SaveFolder, argv[0]);
                        if (PathRemoveFileSpecW(pszBase32SaveFolder) == FALSE)
                        {
                            MessageBoxW(NULL, L"PathRemoveFileSpecW failed", L"Error", MB_OK|MB_ICONERROR);
                            CoTaskMemFree(pszBase32SaveFolder);
                            CoTaskMemFree(pszArg);
                            CoTaskMemFree(pszInputFilePath);
                            LocalFree(argv);
                            return 0;
                        }

                        SIZE_T cbBase32CodecPath = (wcslen(pszBase32SaveFolder) + 1 + wcslen(L"base32codec.exe") + 1) * sizeof(WCHAR);
                        pszBase32CodecPath = (LPWSTR)CoTaskMemAlloc(cbBase32CodecPath * sizeof(WCHAR));
                        wcscpy(pszBase32CodecPath, pszBase32SaveFolder);
                        wcscat(pszBase32CodecPath, L"\\base32codec.exe");


                        CreateProcessW(pszBase32CodecPath, pszArg, NULL, NULL, FALSE, 0, NULL, NULL, psi, &pi);

                        // Wait until base32codec.exe exits.
                        WaitForSingleObject(pi.hProcess, INFINITE);

                        // Get return code of base32codec.exe
                        DWORD dwExitCode;
                        GetExitCodeProcess(pi.hProcess, &dwExitCode);

                        // Report error if base32codec.exe failed
                        // Show message box with exit code
                        LPWSTR pszExitCode;
                        SIZE_T cbExitCode = (wcslen(L"base32codec.exe failed\nExit code: ") + 10 + 1) * sizeof(WCHAR);
                        pszExitCode = (LPWSTR)CoTaskMemAlloc(cbExitCode * sizeof(WCHAR));
                        wsprintfW(pszExitCode, L"base32codec.exe failed\n");
                        wsprintfW(pszExitCode, L"Exit code: %d", dwExitCode);
                        if (dwExitCode != 0)
                        {
                            MessageBoxW(NULL, pszExitCode, L"Error", MB_OK|MB_ICONERROR);
                        }

                        CoTaskMemFree(pszFilePath);
                        CoTaskMemFree(pszArg);
                    }
                    pItem->Release();
                }
            }
            pFileSave->Release();
        }
        CoUninitialize();
    }
    // Free memory
    CoTaskMemFree(pszInputFilePath);
    LocalFree(argv);

    return 0;
}