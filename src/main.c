#include <stdio.h>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <unistd.h>
#include "utilities/file_manager/file_manager.h"
#include "constants/constants.h"
#include "utilities/motion_detector/motion_detector.h"


int main(int argc, char *argv[])
{
    CvFont font;

    //create font for use in drawing
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);

    //stall the program before webcam starts
    fprintf(stdout, "capture will begin in 10 seconds... \n");
    sleep(5);
    fprintf(stdout, "capture is about to start... \n");
    sleep(3);
    fprintf(stdout, "capture starting... \n");
    sleep(2);

    //create a capure session
    CvCapture*webcam = cvCreateCameraCapture(CV_CAP_ANY);
    cvSetCaptureProperty(webcam, CV_CAP_PROP_FPS, 20);
    cvSetCaptureProperty(webcam, CV_CAP_PROP_FRAME_HEIGHT, 375);
    cvSetCaptureProperty(webcam, CV_CAP_PROP_FRAME_WIDTH, 500);

    if (!webcam) {
        fprintf(stdout, "The program cannot access your webcam device \n");
        fprintf(stdout, "or you do not have one installed \n");
        return -1;
    }

    //create a named window, for displaying images from the camera
    cvNamedWindow(APP_WINDOW_NAME, CV_WINDOW_NORMAL);

    IplImage *capturedImage;
    struct MotionDetector motionDetector = mdMakeMotionDetector();
    //loop forever
    while(1) {
        //get an image from the camera
        capturedImage = cvQueryFrame(webcam);

        if (!capturedImage) {
            fprintf(stdout, "couldn't read an image from the webcam\n");
            break;
        }

        if(isMotionDetected(&motionDetector, capturedImage)) {
            cvPutText(capturedImage, "Motion Detected", cvPoint(10, 370), &font, cvScalar(0, 0, 255, 0));
            fmSaveImage(capturedImage);
        } else {
            cvPutText(capturedImage, "No Motion Detected", cvPoint(10, 370), &font, cvScalar(0, 255, 0, 0));
        }

        cvShowImage(APP_WINDOW_NAME, capturedImage);

        //wait for 100ms, if ESC key was pressed during that time
        //then break the loop, else swap data between current and
        //previous frames

        if(cvWaitKey(100) == ESC_KEY) {
            break;
        } else {
            CvMat* temp;
            CV_SWAP(motionDetector.prev_frame_grayscale, motionDetector.matrixOfGray, temp);
        }
    }

    //release memory
    capturedImage = NULL;
    cvReleaseImage(&capturedImage);
    cvReleaseCapture(&webcam);
    cvDestroyWindow(APP_WINDOW_NAME);
    mdFreeMotionDetector(&motionDetector);
    return 0;
}