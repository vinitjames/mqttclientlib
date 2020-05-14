#ifndef FILEACCESS_H
#define FILEACCESS_H
#include <stdio.h>

#ifdef _WIN_32
   #include <io.h>
   #define FILE_EXISTS(path) _access(path, 0)
   #define FILE_ISREADABLE(path) _access(path, 4)
   

#elif linux
   #include <unistd.h>
   #define FILE_EXISTS(path) access(path, F_OK)
   #define FILE_ISREADABLE(path) access(path, R_OK)

#endif // MACRO

enum FILE_IO_STATUS {FILE_NOTEXIST,
					 FILE_UNREADABLE,
					 FILE_READABLE 

};

enum FILE_IO_STATUS is_readable(const char* path) {
    
    if (FILE_EXISTS(path) == -1) {
        return FILE_NOTEXIST;
    }

    if (FILE_ISREADABLE(path) == -1) {
        return FILE_UNREADABLE;
    }
	
    return FILE_READABLE;

}


#endif /* FILEACCESS_H */

