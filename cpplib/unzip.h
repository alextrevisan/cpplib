#ifndef UNZIP_H
#define UNZIP_H

#include "XUnzip.h"

bool unzipAll(TCHAR * pszArchive1)
{
    double counter = 0;
    HZIP hz = OpenZip(pszArchive1, 0, ZIP_FILENAME);
    if(!hz)
    {
        std::cout<<"Error opening file!";

    }
    ZIPENTRY ze;
    memset(&ze, 0, sizeof(ze));
    GetZipItem(hz, -1, &ze);
    int numitems = ze.index;
    printf("\nitems in archive: %d\n", numitems);

    for (int i = 0; i < numitems; i++)
    {
        GetZipItem(hz, i, &ze);
        //printf("%d  %s\n", i, ze.name);
        counter++;
        TCHAR* targetname = ze.name;
        ZRESULT zr = UnzipItem(hz, i, targetname, 0, ZIP_FILENAME);
        std::cout<<std::hex<<zr<<std::endl;
    }
}

#endif // UNZIP_H
