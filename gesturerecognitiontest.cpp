#include "gesturerecognitiontest.h"
#include "frameset.h"
#include <math.h>

using namespace std;
using namespace cv;

#define PI 3.14159265

GestureRecognitionTEST::GestureRecognitionTEST()
{

   numFrame = 0;
   vJoints.resize(28, double(0));
   jointsOld.resize(28, tripletPoint());
   jointsNewCoord.resize(28, tripletPoint());


}

void GestureRecognitionTEST::recognize(FrameSet &frame){

    _frame = frame;

    numFrame++;

    //drawSkeleton();

    //cout << frame.headRotation.x << endl;

    drawHead();

    UpdateVelocities();

    if(numFrame > 1 && _frame.bodyJoint[FS_handRight].x != FLT_MAX)
        changeCoord();

    emit FrameAcquired();

}

void GestureRecognitionTEST::drawSkeleton(){

    imageSkeleton = Mat(424, 512, CV_8UC3, Scalar(255, 255, 255));

    for(int i=0;i<28;++i)
        if((_frame.bodyJointDepth[i].x != FLT_MAX))
            circle(imageSkeleton, Point(_frame.bodyJointDepth[i].x, _frame.bodyJointDepth[i].y), 8, Scalar(255, 0, 0), -1);

}

void GestureRecognitionTEST::drawHead(){

    //if((_frame.bodyJoint[FS_head].x != FLT_MAX))
      //  circle(_frame.depth, Point(_frame.bodyJointDepth[FS_head].x, _frame.bodyJointDepth[FS_head].y), abs(_frame.bodyJointDepth[FS_head].y - _frame.bodyJointDepth[FS_neck].y), Scalar(0, 255, 0), 2);
}

void GestureRecognitionTEST::UpdateVelocities(){

    //Reinitialize new image
    imageSkeleton = Mat(424, 512, CV_8UC3, Scalar(255, 255, 255));

    //Motion Values (check in hand right)
    if(numFrame > 1 && _frame.bodyJoint[FS_handRight].x != FLT_MAX){

        double lambda = 0.95;

        double dirac;

        for(int i=0;i<28;++i)
            if(_frame.bodyJoint[i].x != FLT_MAX){

                dirac = sqrt(pow(_frame.bodyJoint[i].x - jointsOld[i].x, 2) + pow(_frame.bodyJoint[i].y - jointsOld[i].y, 2) + pow(_frame.bodyJoint[i].z - jointsOld[i].z, 2));

                if(dirac > 0.05)
                    dirac = 1;
                else
                    dirac = 0;

                vJoints[i] = vJoints[i]*lambda + (1-lambda)*dirac;

                circle(imageSkeleton, Point(_frame.bodyJointDepth[i].x, _frame.bodyJointDepth[i].y), 9, Scalar(0, 0, 255*vJoints[i]), -1);

            }

        for(int i=0;i<28;++i){
            jointsOld[i].x = _frame.bodyJoint[i].x;
            jointsOld[i].y = _frame.bodyJoint[i].y;
            jointsOld[i].z = _frame.bodyJoint[i].z;
        }

    }

    else if (_frame.bodyJointDepth[FS_handRight].x != FLT_MAX){

        for(int i=0;i<28;++i)
            if(_frame.bodyJoint[i].x != FLT_MAX){
                jointsOld[i].x = _frame.bodyJoint[i].x;
                jointsOld[i].y = _frame.bodyJoint[i].y;
                jointsOld[i].z = _frame.bodyJoint[i].z;
            }

    }

}

void GestureRecognitionTEST::changeCoord(){

    imageNew = Mat(424, 512, CV_8UC3, Scalar(255, 255, 255));

    //Find versors
    tripletPoint p7 = _frame.bodyJoint[FS_shoulderRight];
    tripletPoint p4 = _frame.bodyJoint[FS_shoulderLeft];
    tripletPoint p13 = _frame.bodyJoint[FS_spineBase];

    //N1 versor
    double normaN1 = sqrt( pow(p7.x-p4.x, 2) + pow(p7.y-p4.y, 2) + pow(p7.x-p4.x, 2));
    tripletPoint N1;
    N1.x = ( (p7.x-p4.x)/normaN1 );
    N1.y = ( (p7.y-p4.y)/normaN1 );
    N1.z = ( (p7.z-p4.z)/normaN1 );

    //Tmp versor
    double normaU = sqrt( pow(p7.x-p13.x, 2) + pow(p7.y-p13.y, 2) + pow(p7.x-p13.x, 2));
    tripletPoint U;
    U.x = ( (p7.x-p13.x)/normaU );
    U.y = ( (p7.y-p13.y)/normaU );
    U.z = ( (p7.z-p13.z)/normaU );

    //N3 versor
    tripletPoint N3;
    N3.x = N1.y*U.z - N1.z*U.y;
    N3.y = N1.z*U.x - N1.x*U.z;
    N3.z = N1.x*U.y - N1.y*U.x;
    double normaN3 = sqrt( pow(N3.x,2) + pow(N3.y,2) + pow(N3.z,2) );
    N3.x = N3.x / normaN3;
    N3.y = N3.y / normaN3;
    N3.z = N3.z / normaN3;

    //N2 versor
    tripletPoint N2;
    N2.x = N3.y*N1.z - N3.z*N1.y;
    N2.y = N3.z*N1.x - N3.x*N1.z;
    N2.z = N3.x*N1.y - N3.y*N1.x;

    //Change coordinates
    tripletPoint pCenter = _frame.bodyJoint[FS_spineBase];
    for(int i=0;i<28;++i){
        if( (_frame.bodyJoint[i].x != FLT_MAX) && (i != FS_spineBase) ){

            double R = sqrt( pow(_frame.bodyJoint[i].x-pCenter.x, 2) + pow(_frame.bodyJoint[i].y-pCenter.y, 2) + pow(_frame.bodyJoint[i].x-pCenter.x, 2));

            tripletPoint p;
            p.x = _frame.bodyJoint[i].x - pCenter.x;
            p.y = _frame.bodyJoint[i].y - pCenter.y;
            p.z = _frame.bodyJoint[i].z - pCenter.z;

            //New Coordinates
            jointsNewCoord[i].x = p.x*N1.x + p.y*N1.y + p.z*N1.z;
            jointsNewCoord[i].y = p.x*N2.x + p.y*N2.y + p.z*N2.z;
            jointsNewCoord[i].z = p.x*N3.x + p.y*N3.y + p.z*N3.z;

            //Draw joints
//            tripletPoint screen;
//            double zoom = 150;
//            screen.x = jointsNewCoord[i].x / jointsNewCoord[i].z + zoom  + 256;
//            screen.y = jointsNewCoord[i].y / jointsNewCoord[i].z + zoom  + 212;

//            circle(imageNew, Point(screen.x, screen.y), 9, Scalar(0, 0, 255), -1);
//            if(i == FS_head)
//                circle(imageNew, Point(screen.x, screen.y), 9, Scalar(0, 255, 0), -1);

        }
    }

    if(firstChange){
        firstChange = false;
        tripletPoint N; N.x = 1; N.y = 0; N.z = 0;//versore asse x
        Nold = N;
        //Nold = N1; ///angolo rispetto alla prima direzione individuata
    }
    else{
        double cos = (Nold.x*N1.x + Nold.y*N1.y + Nold.z*N1.z) / (sqrt(pow(Nold.x,2)+pow(Nold.y,2)+pow(Nold.z,2)) * sqrt(pow(N1.x,2)+pow(N1.y,2)+pow(N1.z,2)));
        //cout << N1.x << "\t" << N1.y << "\t" << N1.z << endl;
        double angle = acos(cos) * 180/PI;

        int n = 1; n = n * N1.z;
        if(N1.z > 0.)
            shoulderRotation = angle;
        else
            shoulderRotation = -angle;
    }

}
