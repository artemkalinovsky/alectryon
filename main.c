#include <stdio.h>
#include <opencv2/highgui/highgui_c.h>

int main(int argc, char** argv) {
    cvNamedWindow("Thief", CV_WINDOW_AUTOSIZE);
    CvCapture* webcam = cvCreateCameraCapture(CV_CAP_ANY);
    cvSetCaptureProperty(webcam, CV_CAP_PROP_FRAME_WIDTH, 352); // width of viewport of camera
    cvSetCaptureProperty(webcam, CV_CAP_PROP_FRAME_HEIGHT, 288); // heigh
    if (!webcam) {
        fprintf(stderr, "couldn’t find the webcam.\n");
        exit(1);
    } else {
        fprintf(stdout, "webcam founded.\n");
        while (1) {
            IplImage *image = cvQueryFrame(webcam);
            if (!image) {
                fprintf(stderr, "couldn't read an image from the webcam\n");
                exit(1);
            } else {
                fprintf(stdout, "read image from webcam.\n");
                cvShowImage("Thief", image);
                if(cvWaitKey(100) == 27) {
                    cvDestroyWindow("Thief");
                    cvReleaseImage(&image);
                    break;
                }
            }
        }
    }
    return 0;
}
