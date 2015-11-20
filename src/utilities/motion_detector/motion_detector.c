//
// Created by Artem on 11/20/15.
//

#include <opencv2/core/types_c.h>
#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/video/tracking.hpp>
#include "motion_detector.h"

float motionfactor(const CvMat* fback_flow_map, CvMat* cflowmap, int step) {
    int x, y;
    float totalx = 0;
    float totaly = 0;
    float dx = 0;
    float dy = 0;
    float overall_total = 0;

    for( y = 0; y < cflowmap->rows; y += step) {
        for( x = 0; x < cflowmap->cols; x += step) {
            CvPoint2D32f fxy = CV_MAT_ELEM(*fback_flow_map, CvPoint2D32f, y, x);

            //if the value is negative (as values are vector velocity changes between image pixels)
            //then make it positive only want scalar values to calculate totals.

            if(fxy.x < 0) {
                dx  = (fxy.x * -1);
            }else{
                dx = fxy.x;
            }

            if(fxy.y < 0)
            {
                dy  = (fxy.y * -1);
            }else{
                dy = fxy.y;
            }

            totalx =  totalx+dx;
            totaly = totaly+dy;
        }
    }

    overall_total = totalx + totaly;


    return overall_total;

}

struct MotionDetector mdMakeMotionDetector(){
    CvMat *zeroMat = 0;
    struct MotionDetector motionDetector = {.motion_min_val = 3000,
                                            .matrixOfGray = zeroMat,
                                            .cflow = zeroMat,
                                            .fback_flow_map = zeroMat,
                                            .prev_frame_grayscale = zeroMat,
                                            .temp = 0};
    free(zeroMat);
    return motionDetector;
}

void mdFreeMotionDetector(struct MotionDetector motionDetector) {
    cvReleaseMat(&motionDetector.matrixOfGray);
    cvReleaseMat(&motionDetector.prev_frame_grayscale);
    cvReleaseMat(&motionDetector.fback_flow_map);
    cvReleaseMat(&motionDetector.cflow);
}

int isMotionDetected(struct MotionDetector motionDetector, IplImage *capturedImage) {
    int initialFrame = motionDetector.matrixOfGray == 0;
    if (!motionDetector.matrixOfGray) {
        //create a matrix for matrixOfGray, with same dimensions as webcam image
        motionDetector.matrixOfGray = cvCreateMat(capturedImage->height, capturedImage->width, CV_8UC1);
        //matrix for prevgrey same dimensions as grey
        motionDetector.prev_frame_grayscale = cvCreateMat(motionDetector.matrixOfGray->rows, motionDetector.matrixOfGray->cols, motionDetector.matrixOfGray->type);
        //maps for storing fback_flow_map data from the farneback function
        motionDetector.fback_flow_map = cvCreateMat(motionDetector.matrixOfGray->rows, motionDetector.matrixOfGray->cols, CV_32FC2);
        motionDetector.cflow = cvCreateMat(motionDetector.matrixOfGray->rows, motionDetector.matrixOfGray->cols, CV_8UC3);
    }

    //convert frame to grayscale image and place it in the memory for matrixOfGray
    cvCvtColor(capturedImage, motionDetector.matrixOfGray, CV_BGR2GRAY);

    if (!initialFrame) {
        cvCalcOpticalFlowFarneback(motionDetector.prev_frame_grayscale, motionDetector.matrixOfGray, motionDetector.fback_flow_map, 0.5, 3, 15, 3, 5, 1.2, 0);
        cvCvtColor(motionDetector.prev_frame_grayscale, motionDetector.cflow, CV_GRAY2BGR);
        return (motionfactor(motionDetector.fback_flow_map, motionDetector.cflow, 16) > motionDetector.motion_min_val);
    }
    return 0;
}