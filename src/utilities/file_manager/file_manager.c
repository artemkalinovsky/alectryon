//
// Created by Artem on 11/19/15.
//

#include <opencv2/highgui/highgui_c.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/errno.h>
#include "file_manager.h"
#include "TargetConditionals.h"
#include "../../constants/os_x_constants.h"
#include "../../constants/linux_constants.h"


char* alectryonDirectoryPath() {
    char alectryonDirPath[100] = "";
#ifdef _WIN32
    //define something for Windows (32-bit and 64-bit, this part is common)
   #ifdef _WIN64
      //define something for Windows (64-bit only)
   #endif
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
    // iOS Simulator
#elif TARGET_OS_IPHONE
    // iOS device
#elif TARGET_OS_MAC
    // OS X
    strcat(alectryonDirPath, OS_X_USERS_DIR);
    strcat(alectryonDirPath, getenv(OS_X_USER_ENV));
    strcat(alectryonDirPath, "/Pictures/Alectryon/");
#else
#   error "Unknown Apple platform"
#endif
#elif __linux__
    // Linux
    strcat(alectryonDirPath, LINUX_HOME_DIR);
    strcat(alectryonDirPath, getenv(LINUX_USER_ENV));
    strcat(alectryonDirPath, "/Pictures/Alectryon/");
#elif __unix__ // all unixes not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#   error "Unknown compiler"
#endif
    return alectryonDirPath;
}

int isAlectryonDirectoryAvailable() {
    char *alectryonImagesDir = alectryonDirectoryPath();
    DIR* dir = opendir(alectryonImagesDir);
    if (dir) {
        closedir(dir);
        return 1;
    } else if (ENOENT == errno) {
        /* Directory does not exist. */
        mkdir(alectryonImagesDir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        return 1;
    } else {
        /* opendir() failed for some other reason. */
        return 0;
    }
}

char *savingImageFileName() {
    //get the time
    time_t now;
    time(&now);
    char *time_string_raw = ctime(&now);

    char time_string[100] = "";
    strncpy(time_string, time_string_raw, strlen(time_string_raw)-1);//remove \n

    char imageName[50] = "motion_detect_";
    strcat(imageName, time_string);
    strcat(imageName, ".jpg");

    return imageName;
}

void fmSaveImage(IplImage *iplImage){
    if (isAlectryonDirectoryAvailable()) {
        char imageSavingPath[100] = "";
        strcat(imageSavingPath, alectryonDirectoryPath());
        strcat(imageSavingPath, savingImageFileName());
        cvSaveImage(imageSavingPath, iplImage, 0);
    }
}

