#include "WindowsInterface.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

// Credits to   : xCENTx 
// from         : https://stackoverflow.com/questions/68601080/how-do-you-open-a-file-explorer-dialogue-in-c
// for being kind enough for helping internet strangers navigate the wild wild west of native Windows API.
// openFile is picked from the above link.
std::pair<bool, std::string> WindowsInterface::openFile()
{
    std::string l_filePath;
    //  CREATE FILE OBJECT INSTANCE
    HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(f_SysHr))
    {
        ERROR("Encountered an error : " + std::to_string(GetLastError()) + "\n");
        return std::make_pair<bool, std::string>(false, "");
    }

    // CREATE FileOpenDialog OBJECT
    IFileOpenDialog* f_FileSystem;
    f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
    if (FAILED(f_SysHr)) {
        CoUninitialize();
        ERROR("Encountered an error : " + std::to_string(GetLastError()) + "\n");
        return std::make_pair<bool, std::string>(false, "");
    }

    //  SHOW OPEN FILE DIALOG WINDOW
    f_SysHr = f_FileSystem->Show(NULL);
    if (FAILED(f_SysHr)) {
        f_FileSystem->Release();
        CoUninitialize();
        ERROR("Encountered an error : " + std::to_string(GetLastError()) + "\n");
        return std::make_pair<bool, std::string>(false, "");
    }

    //  RETRIEVE FILE NAME FROM THE SELECTED ITEM
    IShellItem* f_Files;
    f_SysHr = f_FileSystem->GetResult(&f_Files);
    if (FAILED(f_SysHr)) {
        f_FileSystem->Release();
        CoUninitialize();
        ERROR("Encountered an error : " + std::to_string(GetLastError()) + "\n");
        return std::make_pair<bool, std::string>(false, "");
    }

    //  STORE AND CONVERT THE FILE NAME
    PWSTR f_Path;
    f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
    if (FAILED(f_SysHr)) {
        f_Files->Release();
        f_FileSystem->Release();
        CoUninitialize();
        ERROR("Encountered an error : " + std::to_string(GetLastError()) + "\n");
        return std::make_pair<bool, std::string>(false, "");
    }

    //  FORMAT AND STORE THE FILE PATH
    std::wstring path(f_Path);
    std::string c(path.begin(), path.end());
    l_filePath = c;

    //  SUCCESS, CLEAN UP
    CoTaskMemFree(f_Path);
    f_Files->Release();
    f_FileSystem->Release();
    CoUninitialize();

    return std::make_pair<bool, std::string>(true, l_filePath.c_str());
}


// Credits : The Cherno
// From : https://youtu.be/zn7N7zHgCcs?t=589
std::pair<bool, std::string> WindowsInterface::saveFile(const char * p_filter) {
    OPENFILENAMEA l_fileInfoStruct;
    CHAR l_file[260] = { 0 };
    ZeroMemory(&l_fileInfoStruct, sizeof(OPENFILENAMEA));
    l_fileInfoStruct.lStructSize = sizeof(OPENFILENAME);
    l_fileInfoStruct.lpstrFile = l_file;
    l_fileInfoStruct.nMaxFile = sizeof(l_file);
    l_fileInfoStruct.lpstrFilter = p_filter;
    l_fileInfoStruct.nFilterIndex = 1;
    l_fileInfoStruct.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    
    if (GetSaveFileNameA(&l_fileInfoStruct) == TRUE) {
        return std::make_pair(true, l_fileInfoStruct.lpstrFile);
    }

    return std::make_pair(false, std::string());
}