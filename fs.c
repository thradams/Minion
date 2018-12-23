#include "fs.h"


#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)

#include <windows.h>
#include <Strsafe.h>
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>



int fs_mkdir(const char* path)
{
    //ach of these functions returns the value 0 if the new directory was created.
    return _mkdir(path);
}

bool ListDirectoryContents(const char *sDir,
    void(*Callback)(const char*, void* data),
    void* pData)
{
    WIN32_FIND_DATAA fdFile;
    HANDLE hFind = NULL;

    char sPath[MAX_PATH];

    sprintf(sPath, "%s\\*.*", sDir);

    if ((hFind = FindFirstFileA(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    do
    {
        //Find first file will always return "."
        //    and ".." as the first two directories.
        if (strcmp(fdFile.cFileName, ".") != 0
            && strcmp(fdFile.cFileName, "..") != 0)
        {

            //Build up our file path using the passed in
            //  [sDir] and the file/foldername we just found:
            sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);

            //Is the entity a File or Folder?
            if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
            {
                ListDirectoryContents(sPath, Callback, pData); //Recursion, I love it!
            }
            else
            {
                //ver se eh js
                Callback(fdFile.cFileName, pData);
            }
        }
    } while (FindNextFileA(hFind, &fdFile)); //Find the next file.

    FindClose(hFind); //Always, Always, clean things up!

    return true;
}



int ForEachFile(const char* directory, void(*Callback)(const char*))
{
    WIN32_FIND_DATAA ffd;
    LARGE_INTEGER filesize;
    char szDir[MAX_PATH];
    size_t length_of_arg;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;


    StringCchLengthA(directory, MAX_PATH, &length_of_arg);

    if (length_of_arg > (MAX_PATH - 3))
    {
        return (-1);
    }

    StringCchCopyA(szDir, MAX_PATH, directory);
    StringCchCatA(szDir, MAX_PATH, "\\*.txt");

    hFind = FindFirstFileA(szDir, &ffd);

    if (INVALID_HANDLE_VALUE == hFind)
    {
        dwError = GetLastError();
        return dwError;
    }

    do
    {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            char szDir2[MAX_PATH];

            StringCchCopyA(szDir2, MAX_PATH, directory);
            StringCchCatA(szDir2, MAX_PATH, ffd.cFileName);

            //Não é recursiva por enquanto ffd.cFileName
            ForEachFile(szDir2, Callback);

        }
        else
        {
            filesize.LowPart = ffd.nFileSizeLow;
            filesize.HighPart = ffd.nFileSizeHigh;

            Callback(ffd.cFileName);
        }
    } while (FindNextFileA(hFind, &ffd) != 0);

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        return dwError;
    }

    FindClose(hFind);
    return 0;
}



int ForEachDir(const char* directory, void(*Callback)(const char*))
{
    WIN32_FIND_DATAA ffd;
    LARGE_INTEGER filesize;
    char szDir[MAX_PATH];
    size_t length_of_arg;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;


    HRESULT hr = StringCchLengthA(directory, MAX_PATH, &length_of_arg);

    if (FAILED(hr))
    {
        return -1;
    }

    if (length_of_arg > (MAX_PATH - 3))
    {
        return (-1);
    }

    StringCchCopyA(szDir, MAX_PATH, directory);
    StringCchCatA(szDir, MAX_PATH, "\\*.txt");

    hFind = FindFirstFileA(szDir, &ffd);

    if (INVALID_HANDLE_VALUE == hFind)
    {
        return dwError;
    }

    do
    {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            //Não é recursiva por enquanto ffd.cFileName
            filesize.LowPart = ffd.nFileSizeLow;
            filesize.HighPart = ffd.nFileSizeHigh;
            Callback(ffd.cFileName);
        }
        else
        {

        }
    } while (FindNextFileA(hFind, &ffd) != 0);

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        return dwError;
    }

    FindClose(hFind);
    return 0;
}
#else

#include <dirent.h> 

/* let us make a recursive function to print the content of a given folder */

void show_dir_content(char * path)
{
    DIR * d = opendir(path); // open the path
    if (d == NULL) return; // if was not able return
    struct dirent * dir; // for the directory entries
    while ((dir = readdir(d)) != NULL) // if we were able to read somehting from the directory
    {
        if (dir->d_type != DT_DIR) // if the type is not directory just print it with blue
            printf("%s%s\n", BLUE, dir->d_name);
        else
            if (dir->d_type == DT_DIR && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) // if it is a directory
            {
                printf("%s%s\n", GREEN, dir->d_name); // print its name in green
                char d_path[255]; // here I am using sprintf which is safer than strcat
                sprintf(d_path, "%s/%s", path, dir->d_name);
                show_dir_content(d_path); // recall with the new path
            }
    }
    closedir(d); // finally close the directory
}

#endif





void CCopyFile(const char* sourceName, const char* destName)
{
    FILE * outputCSS = fopen(destName, "w");
    FILE * inputCSS = fopen(sourceName, "r");

    if (inputCSS)
    {
        if (outputCSS)
        {
            while (!feof(inputCSS))
            {
                char c = fgetc(inputCSS);
                if (c != EOF)
                    putc(c, outputCSS);
            }
            fclose(outputCSS);
        }
        fclose(inputCSS);
    }
}