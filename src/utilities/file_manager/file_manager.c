//
// Created by Artem on 11/19/15.
//

#include <opencv2/highgui/highgui_c.h>
#include <time.h>
#include "file_manager.h"
#include "TargetConditionals.h"

#define OS_X_USER_ENV "USER"
#define OS_X_USERS_DIR "/Users/"

#define LINUX_USER_ENV "USERNAME"
#define LINUX_HOME_DIR "/home/"

char* currentUserHomePath() {
    char userHomeDirectoryPath[100] = "";
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
    strcat(userHomeDirectoryPath, OS_X_USERS_DIR);
    strcat(userHomeDirectoryPath, getenv(OS_X_USER_ENV));
    strcat(userHomeDirectoryPath, "/Pictures/");
#else
#   error "Unknown Apple platform"
#endif
#elif __linux__
    // Linux
    strcat(userHomeDirectoryPath, LINUX_HOME_DIR);
    strcat(userHomeDirectoryPath, getenv(LINUX_USER_ENV));
    strcat(userHomeDirectoryPath, "/Pictures/");
#elif __unix__ // all unixes not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#   error "Unknown compiler"
#endif
    return userHomeDirectoryPath;
}

void fmSaveImage(IplImage *iplImage) {
    //get the time
    time_t now;
    time(&now);
    char *time_string_raw = ctime(&now);

    char time_string[100] = "";
    strncpy(time_string, time_string_raw, strlen(time_string_raw)-1);//remove \n

    char imageName[50] = "motion_detect_";
    char *userHomePath = currentUserHomePath();

    strcat(imageName, time_string);
    strcat(imageName, ".jpg");

    char imageSavingPath[100] = "";
    strcat(imageSavingPath, userHomePath);
    strcat(imageSavingPath, imageName);


    cvSaveImage(imageSavingPath, iplImage, 0);

    time_string_raw = NULL;
    userHomePath = NULL;
}

