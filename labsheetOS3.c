//
// Created by JamesBurling on 27/11/2020.
//


// OPEN FILES //
FILE *fd = fopen("wombat.txt", "r"); // stdio.h
int open(const char *path, int flags, mode_t mode);// POSIX // Returns a positive int of file descriptor, -1 if error

// CREATE FILES //
int creat(const char *pathname, mode_t mode);

// CLOSE FILES //
int close(int fd);// unistd.h

// READING & WRITING DATA //
/* Both return number of bytes read/written or -1 if error
 * Data read to/written from buffer (buf) of size (count)
 */
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);

// RANDOM ACCESS FILES //
off_t lseek(int fd, off_t offset, int whence);
/*
 * OFFSETS:
 * SEEK_SET - the offset is relative to the start of the file.
 * SEEK_CUR - the offset is relative to the current position within the file.
 * SEEK_END - the offset is relative to the current end of the file.
 */

// NAVIGATING HIERARCHICAL DIRECTORY STRUCTURE //

// GET CURRENT WORKING DIRECTORY //
char *getcwd(char *buffer, size_t size); // buffer = null if error
// CHANGE WORKING DIRECTORY //
int chdir(const char *path); // Return -1 if error
// MAKE A DIRECTORY //
int mkdir(const char *path, mode_t mode); // mode sets attributes (access permissions)
// OPENING A DIRECTORY //
DIR *opendir(const char *name);
// CLOSING A DIRECTORY //
int closedir(DIR *dirp);
// READING A DIRECTORY //
/* Reads file sequentially until none left, then NULL is returned */
struct dirent *readdir(DIR *dirp);

struct dirent {// DIRECTORY STRUCTURE
    ino_t d_ino; // I-node number of file
    off_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[256]; // name of file
};

/* example reading directory*/
DIR *dp = opendir("/usr/bin");
struct dirent *entry;
while ((entry = readdir(dp)) != NULL){
 printf("%8d - %s\n", entry->d_ino, entry->d_name);
}
closedir(dp);

// READING & MODIFYING FILE ATTRIBUTES //

// READ
int stat(const char *pathname, struct stat *statbuf);
// Stat struct MACROS //
S_ISDIR() // returns true if directory
S_ISREG // returns true if a regular file
// stat field st_mode contains file acces permissions
getpduid() // get name of user ID
getgrgid() // get primary group id to which owner belongs
strftime() // get dat and time
localtime() // break up time_t value
// chmod and utime etc can also be used