#include "AppBuild.h"


#include <windows.h>
#include <Strsafe.h>
#include <stdio.h>
#include <stdbool.h>


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

struct Capture
{
  FILE* file;
  const char* folderName;
};

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

static void _lambda_0(const char* name, void* data)
{
  struct Capture *pCapture = (struct Capture *)data;

  FILE * indexHtmlFile = pCapture->file;
  const char* folderName = pCapture->folderName;

  int l = strlen(name);
  if (l > 3 &&
    name[l - 3] == '.' &&
    name[l - 2] == 'j'   &&
    name[l - 1] == 's')
  {
    fprintf(indexHtmlFile, "    <script src=\"%s\"></script>\n", name);
    
    int l = strlen(name);

    //copia o JS para o destino
    char inputJSName[MAX_PATH] = { 0 };
    strcat(inputJSName, folderName);
    strcat(inputJSName, "/source/");
    strcat(inputJSName, name);

    char outputJSName[MAX_PATH] = { 0 };
    strcat(outputJSName, folderName);
    strcat(outputJSName, "/html/");
    strcat(outputJSName, name);

    char inputXMLName[MAX_PATH] = { 0 };
    strcat(inputXMLName, folderName);
    strcat(inputXMLName, "/source/");
    strncat(inputXMLName, name, l - 2);
    strcat(inputXMLName, "xml");


    FILE * outputJS = fopen(outputJSName, "w");
    FILE * inputJS = fopen(inputJSName, "r");
    FILE * inputXML = fopen(inputXMLName, "r");
    
    if (inputXML)
    {
      char justName[MAX_PATH] = { 0 };
      strncat(justName, name, l - 3);
      

      fprintf(outputJS, "Design.%s = `", justName);
      while (!feof(inputXML))
      {
        char c = fgetc(inputXML);
        if (c != EOF)
          putc(c, outputJS);
      }
      fprintf(outputJS, "`;\n\n");
      fclose(inputXML);
    }

    if (inputJS)
    {
      while (!feof(inputJS))
      {
        char c = fgetc(inputJS);
        if (c != EOF)
          putc(c, outputJS);
      }
    }
    fclose(inputJS);
    fclose(outputJS);
  }
  else if (l > 4 &&
    name[l - 4] == '.' &&
    name[l - 3] == 'c'   &&
    name[l - 2] == 's' &&
    name[l - 1] == 's')
  {
    fprintf(indexHtmlFile, "    <link rel=\"stylesheet\" href=\"%s\"/>\n", name);

    char outputCSSName[MAX_PATH] = { 0 };
    strcat(outputCSSName, folderName);
    strcat(outputCSSName, "/html/");
    strcat(outputCSSName, name);

    char inputCSSNAme[MAX_PATH] = { 0 };
    strcat(inputCSSNAme, folderName);
    strcat(inputCSSNAme, "/source/");
    strncat(inputCSSNAme, name, l - 3);
    strcat(inputCSSNAme, "css");

    CCopyFile(inputCSSNAme, outputCSSName);
  }
}

#define PART1 \
"<!DOCTYPE html>\n"\
"<html lang=\"en\">\n"\
"<head>\n"\
"    <link rel=\"manifest\" href=\"manifest.json\">\n"\
"    <link rel=\"apple-touch-icon\" href=\"single-page-icon.png\">\n"\
"    <meta charset=\"UTF-8\">\n"\
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"\
"    <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\n"\
"    <title>Document</title>\n"\
"    <script src=\"core.js\"></script>\n"\
"    <script src=\"runtime.js\"></script>\n"\
"\n"\
"\n"


#define PART2 \
"    <script>function OnLoad() { ShowScreen(\"Tela1\"); } </script>\n\n"\
" </head>\n"\
"   <body onload=\"OnLoad()\">\n"\
"   </body>\n"\
"</html>\n"

void BuildApp(const char* folderName, const char* rootFolder)
{

  char outputCSSName[MAX_PATH] = { 0 };
  strcat(outputCSSName, folderName);
  strcat(outputCSSName, "/html/core.js");

  char inputCSSNAme[MAX_PATH] = { 0 };
  strcat(inputCSSNAme, rootFolder);
  strcat(inputCSSNAme, "/html/core.js");
  

  CCopyFile(inputCSSNAme, outputCSSName);

  outputCSSName[0] = 0;
  strcat(outputCSSName, folderName);
  strcat(outputCSSName, "/html/runtime.js");

  inputCSSNAme[0] = 0;
  strcat(inputCSSNAme, rootFolder);
  strcat(inputCSSNAme, "/html/runtime.js");


  CCopyFile(inputCSSNAme, outputCSSName);


  char buffer[MAX_PATH] = { 0 };
  strcat(buffer, folderName);
  strcat(buffer, "/html/index.html");

  FILE * indexHtmlFile = fopen(buffer, "w");
  if (indexHtmlFile)
  {
    fprintf(indexHtmlFile, "%s", PART1);

    buffer[0] = 0;
    strcat(buffer, folderName);
    strcat(buffer, "/source");

    struct Capture capture = { indexHtmlFile, folderName };
    ListDirectoryContents(buffer, _lambda_0, &capture);

    fprintf(indexHtmlFile, "%s", PART2);
    fclose(indexHtmlFile);
  }

  //falta copiar arquivos basicos p destino
}



