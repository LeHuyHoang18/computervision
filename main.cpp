#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <vector>

#include "StereoVision.h"

using namespace cv;
using namespace std;


float baseline = 9.0;
float focalLenght = 6.0;
float alpha = 56.6;


int main() {

    Mat leftFrame, rightFrame;
    VideoCapture capLeft(0);
    VideoCapture capRight(1);

    StereoVision stereovision(baseline, alpha, focalLenght);

    if (!capLeft.isOpened()) {
        cout << "Cannot Open Left Camera" << endl;
        return -1;
    }

    if (!capRight.isOpened()) {
        cout << "Cannot Open Right Camera" << endl;
        return -1;
    }

    Mat leftMask, rightMask;
    Mat leftResFrame, rightResFrame;

    Point leftCircle, rightCircle;

    float ballDepth = 0;

    while (true) {

        capLeft.read(leftFrame);
        capRight.read(rightFrame);

       
        leftMask = stereovision.add_HSV_filter(leftFrame, 0);
        rightMask = stereovision.add_HSV_filter(rightFrame, 1);

       
        bitwise_and(leftFrame, leftFrame, leftResFrame, leftMask);
        bitwise_and(rightFrame, rightFrame, rightResFrame, rightMask);

        
    
        leftCircle = stereovision.find_ball(leftFrame, leftMask);
        rightCircle = stereovision.find_ball(rightFrame, rightMask);


        if (!leftCircle.x || !rightCircle.x) {
            putText(leftFrame, "Tracking Lost", { 75, 50 }, FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2);
            putText(rightFrame, "Tracking Lost!", { 75, 75 }, FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2);
        } else {

            ballDepth = stereovision.find_depth(leftCircle, rightCircle, leftFrame, rightFrame);

            putText(leftFrame, "Tracking!", { 75, 50 }, FONT_HERSHEY_SIMPLEX, 0.7, (125, 250, 0), 2);
            putText(rightFrame, "Tracking!", { 75, 75 }, FONT_HERSHEY_SIMPLEX, 0.7, (125, 250, 0), 2);

            cout << "Ball depth: " << ballDepth << endl;
 
        }

        imshow("Left Frame", leftFrame);
        imshow("Right Frame", rightFrame);
        imshow("Left Mask", leftMask);
        imshow("Right Mask", rightMask);


        if ((waitKey(1) & 0xFF) == 'q') {
            break;
        }
    }

    capLeft.release();
    capRight.release();

    destroyAllWindows();

    return 0;
}
