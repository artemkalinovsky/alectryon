//
// Created by Artem on 11/20/15.
//

#ifndef ALECTRYON_MOTION_DETECTOR_H
#define ALECTRYON_MOTION_DETECTOR_H

struct MotionDetector {
    int motion_min_val;
    CvMat  *prev_frame_grayscale;
    CvMat *fback_flow_map;
    CvMat *cflow;
    CvMat *temp;
    CvMat *matrixOfGray;
};

struct MotionDetector mdMakeMotionDetector();
void mdFreeMotionDetector(struct MotionDetector*);
int isMotionDetected(struct MotionDetector*, IplImage*);

#endif //ALECTRYON_MOTION_DETECTOR_H
