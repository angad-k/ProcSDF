#pragma once
#include <Windows.h>
#include <string>
#include <shobjidl.h>

namespace WindowsInterface {
    std::pair<bool, std::string> openFile()
    {
        std::string l_selectedFile;
        std::string l_filePath;
        //  CREATE FILE OBJECT INSTANCE
        HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (FAILED(f_SysHr))
        {
            printf("WINDOWSAPI : ENCOUNTERED AN ERROR: (%d)\n", GetLastError());
            return std::make_pair<bool, std::string>(false, "");
        }

        // CREATE FileOpenDialog OBJECT
        IFileOpenDialog* f_FileSystem;
        f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
        if (FAILED(f_SysHr)) {
            CoUninitialize();
            printf("WINDOWSAPI : ENCOUNTERED AN ERROR: (%d)\n", GetLastError());
            return std::make_pair<bool, std::string>(false, "");
        }

        //  SHOW OPEN FILE DIALOG WINDOW
        f_SysHr = f_FileSystem->Show(NULL);
        if (FAILED(f_SysHr)) {
            f_FileSystem->Release();
            CoUninitialize();
            printf("WINDOWSAPI : ENCOUNTERED AN ERROR: (%d)\n", GetLastError());
            return std::make_pair<bool, std::string>(false, "");
        }

        //  RETRIEVE FILE NAME FROM THE SELECTED ITEM
        IShellItem* f_Files;
        f_SysHr = f_FileSystem->GetResult(&f_Files);
        if (FAILED(f_SysHr)) {
            f_FileSystem->Release();
            CoUninitialize();
            printf("WINDOWSAPI : ENCOUNTERED AN ERROR: (%d)\n", GetLastError());
            return std::make_pair<bool, std::string>(false, "");
        }

        //  STORE AND CONVERT THE FILE NAME
        PWSTR f_Path;
        f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
        if (FAILED(f_SysHr)) {
            f_Files->Release();
            f_FileSystem->Release();
            CoUninitialize();
            printf("WINDOWSAPI : ENCOUNTERED AN ERROR: (%d)\n", GetLastError());
            return std::make_pair<bool, std::string>(false, "");
        }

        //  FORMAT AND STORE THE FILE PATH
        std::wstring path(f_Path);
        std::string c(path.begin(), path.end());
        l_filePath = c;

        //  FORMAT STRING FOR EXECUTABLE NAME
        const size_t slash = l_filePath.find_last_of("/\\");
        l_selectedFile = l_filePath.substr(slash + 1);

        //  SUCCESS, CLEAN UP
        CoTaskMemFree(f_Path);
        f_Files->Release();
        f_FileSystem->Release();
        CoUninitialize();

        printf("SELECTED FILE: %s\nFILE PATH: %s\n\n", l_selectedFile.c_str(), l_filePath.c_str());

        return std::make_pair<bool, std::string>(true, "");
    }

}