#include "AppBuild.h"



#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "fs.h"


struct Capture
{
  FILE* file;
  const char* folderName;
};


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
    char inputJSName[FS_MAX_PATH] = { 0 };
    strcat(inputJSName, folderName);
    strcat(inputJSName, "/source/");
    strcat(inputJSName, name);

    char outputJSName[FS_MAX_PATH] = { 0 };
    strcat(outputJSName, folderName);
    strcat(outputJSName, "/html/");
    strcat(outputJSName, name);

    char inputXMLName[FS_MAX_PATH] = { 0 };
    strcat(inputXMLName, folderName);
    strcat(inputXMLName, "/source/");
    strncat(inputXMLName, name, l - 2);
    strcat(inputXMLName, "html");


    FILE * outputJS = fopen(outputJSName, "w");
    FILE * inputJS = fopen(inputJSName, "r");
    FILE * inputXML = fopen(inputXMLName, "r");
    
    if (inputXML)
    {
      char justName[FS_MAX_PATH] = { 0 };
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

    char outputCSSName[FS_MAX_PATH] = { 0 };
    strcat(outputCSSName, folderName);
    strcat(outputCSSName, "/html/");
    strcat(outputCSSName, name);

    char inputCSSNAme[FS_MAX_PATH] = { 0 };
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
" </head>\n"\
"   <body onload=\"Main()\">\n"\
"   </body>\n"\
"</html>\n"

void BuildApp(const char* folderName, const char* rootFolder)
{

  char outputCSSName[FS_MAX_PATH] = { 0 };
  strcat(outputCSSName, folderName);
  strcat(outputCSSName, "/html/core.js");

  char inputCSSNAme[FS_MAX_PATH] = { 0 };
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


  char buffer[FS_MAX_PATH] = { 0 };
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


void CreateApp(const char* templateFolder, const char* appFolder)
{

}