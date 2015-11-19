#include <stdio.h>
#include <stdlib.h>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <unistd.h>
#include "utilities/file_manager/file_manager.h"

//globals

int motion_min_val = 300;

CvMat  *prev_frame_grayscale = 0, *gray = 0, *fback_flow_map = 0, *cflow = 0, *temp = 0;

IplImage *cap_frame;

FILE *file_er;

CvFont font;




float motionfactor(const CvMat* fback_flow_map, CvMat* cflowmap, int step)
{
    int x, y;
    float totalx = 0;
    float totaly = 0;
    float dx = 0;
    float dy = 0;
    float overall_total = 0;

    for( y = 0; y < cflowmap->rows; y += step)
    {
        for( x = 0; x < cflowmap->cols; x += step)
        {


            CvPoint2D32f fxy = CV_MAT_ELEM(*fback_flow_map, CvPoint2D32f, y, x);


            //if the value is negative (as values are vector velocity changes between image pixels)
            //then make it positive only want scalar values to calculate totals.

            if(fxy.x < 0)
            {
                dx  = (fxy.x * -1);
            }else
            {
                dx = fxy.x;
            }

            if(fxy.y < 0)
            {
                dy  = (fxy.y * -1);
            }else
            {
                dy = fxy.y;
            }

            totalx =  totalx+dx;
            totaly = totaly+dy;


        }
    }

    overall_total = totalx + totaly;


    return overall_total;

}





int main(int argc, char *argv[])
{

    //check for and interperet passed flags

    if(argc != 1)
    {
        int sens = atoi(argv[1]);

        fprintf(stdout, "sensitivity arg is %i \n",sens);

        int motion_min_val = sens;

    }else{
        fprintf(stdout, "the sensitivity input was left un-entered so sensitivity has been defaulted to %i \n", motion_min_val);
    }


    //create font for use in drawing
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);


    //check if log file exists already
    if(file_er = fopen("log.dat", "r"))
    {
        file_er = fopen("log.dat", "a");

    }else{

        file_er = fopen("log.dat", "w");

    }

    //stall the program before capture starts
    fprintf(stdout, "capture will begin in 10 seconds... \n");
    sleep(5);
    fprintf(stdout, "capture is about to start... \n");
    sleep(3);
    fprintf(stdout, "capture starting... \n");
    sleep(2);

    //create a ca[ure session
    CvCapture* capture = cvCreateCameraCapture(0);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FPS, 20);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 375);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 500);



    if(!capture)
    {

        fprintf(stdout, "The program cannot access your webcam device \n");
        fprintf(stdout, "or you do not have one installed \n");
        return -1;
    }

    //create a named window, for displaying images from the camera
    cvNamedWindow("Motion Detector Main Window", CV_WINDOW_NORMAL);

    //loop forever
    while(1)
    {

        int InitialFrame = gray == 0;

        //get an image from the camera
        cap_frame = cvQueryFrame(capture);

        if(!cap_frame)
        {
            break;
        }


        if(!gray)
        {

            //create a matrix for gray, with same dimensions as webcam image
            gray = cvCreateMat(cap_frame->height, cap_frame->width, CV_8UC1);
            //matrix for prevgrey same dimensions as grey
            prev_frame_grayscale = cvCreateMat(gray->rows, gray->cols, gray->type);
            //maps for storing fback_flow_map data from the farneback function
            fback_flow_map = cvCreateMat(gray->rows, gray->cols, CV_32FC2);
            cflow = cvCreateMat(gray->rows, gray->cols, CV_8UC3);
        }


        //convert frame to grayscale image and place it in the memory for gray
        cvCvtColor(cap_frame, gray, CV_BGR2GRAY);

        if( !InitialFrame )
        {

            cvCalcOpticalFlowFarneback(prev_frame_grayscale, gray, fback_flow_map, 0.5, 3, 15, 3, 5, 1.2, 0);

            cvCvtColor(prev_frame_grayscale, cflow, CV_GRAY2BGR);

            if(motionfactor(fback_flow_map, cflow, 16) > motion_min_val)
            {
                cvPutText(cap_frame, "Motion Detected", cvPoint(10, 370), &font, cvScalar(0, 0, 255, 0));
                fmSaveImage(cap_frame);
            }else {

                cvPutText(cap_frame, "No Motion Detected", cvPoint(10, 370), &font, cvScalar(0, 255, 0, 0));

            }


            cvShowImage("Motion Detector Main Window", cap_frame);

        }


        //wait for 30ms, if any keys were pressed during that time
        //then break the loop, else swap data between current and
        //previous frames

        if(cvWaitKey(20)>=0)
        {
            break;

        }else{
            CvMat* temp;
            CV_SWAP(prev_frame_grayscale, gray, temp);
        }
    }

    //release memory
    cap_frame = NULL;

    cvReleaseImage(&cap_frame);
    cvReleaseCapture(&capture);
    cvReleaseMat(&gray);
    cvReleaseMat(&prev_frame_grayscale);
    cvReleaseMat(&fback_flow_map);
    cvReleaseMat(&cflow);
    fclose(file_er);
    cvDestroyWindow("Motion Detector Main Window");

    return 0;
}