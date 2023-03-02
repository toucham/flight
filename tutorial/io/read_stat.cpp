#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

/* checks the type of the file */
int main(int argc, char const *argv[])
{
    /* struct stat => https://linux.die.net/man/2/stat */
    struct stat st;
    std::string type, readok;

    // stat(char* filename, struct stat *buf) -> int
    stat(argv[1], &st); /* fills the struct stat with information of the file */

    /** Macros to check the file type:
     * S_ISREG(m) checks if its a regular file
     * S_ISDIR(m) check if it's a directory file
     * S_ISSOCK(m) check if it's a network socket
     * */ 

    printf("%s\n", st.st_mode);

    if(S_ISREG(st.st_mode)) {
        type = "regular";
    } else if(S_ISDIR(st.st_mode)) {
        type = "directory";
    } else {
        type = "other";
    }

    if((st.st_mode & S_IRUSR)) { /* check read access */
        readok = "yes";
    } else {
        readok = "no";
    } 

    printf("type: %s, read: %s\n", type.c_str(), readok.c_str());
    exit(0);
}
