#include <stdio.h>
#include <stdlib.h>
#include <opencv2/highgui/highgui_c.h>
#include <unistd.h>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include "constants/constants.h"

int main(int argc, char** argv) {
    short first_run = 0;
    short capture_first = 0;

    CvMat *curr_img, *prev_img;

    cvNamedWindow("alectryon", CV_WINDOW_AUTOSIZE);
    CvCapture* webcam = cvCreateCameraCapture(CV_CAP_ANY);
    cvSetCaptureProperty(webcam, CV_CAP_PROP_FRAME_WIDTH, 352);
    cvSetCaptureProperty(webcam, CV_CAP_PROP_FRAME_HEIGHT, 288);
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
                cvShowImage("alectryon", image);
                //detecting motion
                if(!curr_img)
                {
                    curr_img = cvCreateMat(image->height, image->width, CV_8UC1);
                    prev_img = cvCreateMat(curr_img->rows, curr_img->cols, curr_img->type);
                    first_run = 1;
                }
                cvCvtColor(image, curr_img, CV_BGR2GRAY);
                if(first_run == 1)
                {
                    double norm_L2 = cvNorm(prev_img, curr_img, CV_L2, NULL);
                    if(norm_L2 > 30000 || capture_first == 0)
                        norm_L2 = 0; capture_first = 1; /*There is a bug at launch that causes double
							  * to equal > 70000. This fixes the false alarm.*/
                    if(norm_L2 > 3000)
                    {
                        printf("Intruder detected, saving image!\tnorm_L2 = %f\n", norm_L2);
                        cvShowImage("Detect", curr_img);
//                        save_image(frame, save_location);
                        /*if(i > 5)
                        {
                            //save_video();
                            printf("Video is being saved instead! i = %i\n", i);
                        }
                        else
                            save_image(frame); */
                        sleep(0.5); //delay 500 ms not to fill directory

                    }
                }

                CvMat *temp;
                CV_SWAP(prev_img, curr_img, temp);
                //
                if(cvWaitKey(100) == ESC_KEY) {
                    cvDestroyWindow("alectryon");
                    cvReleaseImage(&image);
                    break;
                }
            }
        }
    }
    return 0;
}
