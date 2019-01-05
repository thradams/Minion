#include "..\fs.h"
#include <stdio.h>



int main(int argc, char *argv[])
{
    char buffer[FS_MAX_PATH];
    fs_current_path(buffer);
    printf("%s", buffer);
    struct error_code ec = { 0 };
    fs_copy("./D11", "./D2", &ec);
}
