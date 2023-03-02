#include <dirent.h>
#include <cerrno>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    /* DIR & dirent => https://pubs.opengroup.org/onlinepubs/009604599/basedefs/dirent.h.html */    
    // a stream is an abstraction for an ordered list of items (e.g. list of directory entries)
    DIR *streamp; /* a pointer to directory stream */
    // doesn't need to explicitly define struct in front in C++ (vice versa for C)
    dirent *dep; /* a pointer to the next directory in the stream */

    /* opendir() => https://pubs.opengroup.org/onlinepubs/009604599/functions/opendir.html */
    streamp = opendir(argv[1]); /* pointer to handle if OK, NULL on error */
    errno = 0;
    /* readdir() => https://pubs.opengroup.org/onlinepubs/009604599/functions/readdir.html */
    while((dep = readdir(streamp)) != NULL) { 
        printf("Found file: %s\n", dep->d_name);
    }

    if(errno != 0) printf("readdir error");
    /* closedir() => https://pubs.opengroup.org/onlinepubs/009604599/functions/closedir.html */
    closedir(streamp);
    exit(0);
}
