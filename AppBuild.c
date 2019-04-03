#include "AppBuild.h"



#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "fs.h"

#define FS_MAX_PATH 256


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

    struct error_code ec = { 0 };

    fs_copy_file(inputCSSNAme, outputCSSName, &ec);

    outputCSSName[0] = 0;
    strcat(outputCSSName, folderName);
    strcat(outputCSSName, "/html/runtime.js");

    inputCSSNAme[0] = 0;
    strcat(inputCSSNAme, rootFolder);
    strcat(inputCSSNAme, "/html/runtime.js");

    fs_copy_file(inputCSSNAme, outputCSSName, &ec);

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

        struct directory_iterator di = { 0 };
        if (directory_iterator_init(&di, buffer, &ec))
        {
            do
            {
                int l = (int) strlen(di.fileName);
                if (l > 3 &&
                    di.fileName[l - 3] == '.' &&
                    di.fileName[l - 2] == 'j'   &&
                    di.fileName[l - 1] == 's')
                {
                    fprintf(indexHtmlFile, "    <script src=\"%s\"></script>\n", di.fileName);

                    int l = (int)strlen(di.fileName);

                    //copia o JS para o destino
                    char inputJSName[FS_MAX_PATH] = { 0 };
                    strcat(inputJSName, folderName);
                    strcat(inputJSName, "/source/");
                    strcat(inputJSName, di.fileName);

                    char outputJSName[FS_MAX_PATH] = { 0 };
                    strcat(outputJSName, folderName);
                    strcat(outputJSName, "/html/");
                    strcat(outputJSName, di.fileName);

                    char inputXMLName[FS_MAX_PATH] = { 0 };
                    strcat(inputXMLName, folderName);
                    strcat(inputXMLName, "/source/");
                    strncat(inputXMLName, di.fileName, l - 2);
                    strcat(inputXMLName, "html");


                    FILE * outputJS = fopen(outputJSName, "w");
                    FILE * inputJS = fopen(inputJSName, "r");
                    FILE * inputXML = fopen(inputXMLName, "r");

                    

                    if (inputJS)
                    {
                        while (!feof(inputJS))
                        {
                            int c = fgetc(inputJS);
                            if (c != EOF)
                                putc(c, outputJS);
                        }
                    }

					if (inputXML)
					{
						char justName[FS_MAX_PATH] = { 0 };
						strncat(justName, di.fileName, l - 3);

						fprintf(outputJS, "\n%s.Template=`", justName);// subtract(a, b) { return a - b }
					
													
						while (!feof(inputXML))
						{
							int c = fgetc(inputXML);
							if (c != EOF)
								putc(c, outputJS);
						}
						fprintf(outputJS, "`;\n\n");
						fclose(inputXML);
					}

                    fclose(inputJS);
                    fclose(outputJS);
                }
                else if (l > 4 &&
                    di.fileName[l - 4] == '.' &&
                    di.fileName[l - 3] == 'c'   &&
                    di.fileName[l - 2] == 's' &&
                    di.fileName[l - 1] == 's')
                {
                    fprintf(indexHtmlFile, "    <link rel=\"stylesheet\" href=\"%s\"/>\n", di.fileName);

                    char outputCSSName[FS_MAX_PATH] = { 0 };
                    strcat(outputCSSName, folderName);
                    strcat(outputCSSName, "/html/");
                    strcat(outputCSSName, di.fileName);

                    char inputCSSNAme[FS_MAX_PATH] = { 0 };
                    strcat(inputCSSNAme, folderName);
                    strcat(inputCSSNAme, "/source/");
                    strncat(inputCSSNAme, di.fileName, l - 3);
                    strcat(inputCSSNAme, "css");

                    fs_copy_file(inputCSSNAme, outputCSSName, &ec);
                }
                //printf("%s %s\n", di.fileName, di.bIsDir ? "(dir)" : "");
            } while (directory_iterator_next(&di));
            directory_iterator_destroy(&di);
        }
      
        fprintf(indexHtmlFile, "%s", PART2);
        fclose(indexHtmlFile);
    }
}


void CreateApp(const char* templateFolder, const char* appFolder)
{

}