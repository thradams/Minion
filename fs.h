#pragma once
#include <stdbool.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)


#define FS_MAX_PATH 260

#else

#define FS_MAX_PATH 260

#endif


//List all files (recursive)
bool ListDirectoryContents(const char *sDir,
    void(*Callback)(const char*, void* data),
    void* pData);

int fs_mkdir(const char* path);



void CCopyFile(const char* sourceName, const char* destName);