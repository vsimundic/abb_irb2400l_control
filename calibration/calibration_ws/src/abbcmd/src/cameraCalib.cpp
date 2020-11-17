#include "RMM.h"
#include "ros/ros.h"
#include "aruco.h"
#include <iostream>

using namespace ros;
using namespace aruco;
using namespace std;

int main(int argc, char **argv)
{
    init(argc, argv, "cameraCalib");
    NodeHandle n;
    RMM rmm;

    std::string cam_type;

    n.getParam(ros::this_node::getName() + "/cam_type", cam_type);

    rmm.Init(cam_type);

    int input;

    cout<<"------------------------------------------------------------------------------"<<endl
    <<"Press 1 to initiate calibration or press 0 to load transformation matrix."<<endl;
    cin >> input;
    switch(input)
    {
        case 1:
            rmm.CalibrateABB();
            cout<<"------------------------------------------------------------------------------"<<endl
            <<"Do you wish to test the calibration? Press 1 for yes or 0 for no."<<endl;
            cin >> input;
            if (input==1)
            {
                rmm.testMatrix();
                rmm.errorCalib();
            }
            break;
        
        default:
            rmm.TmatrixInput();
            cout<<"------------------------------------------------------------------------------"<<endl
            <<"Do you wish to test the calibration? Press 1 for yes or 0 for no."<<endl;
            cin>>input;
            if (input == 1)
            {
                rmm.testMatrix();
                rmm.errorCalib();
            }
            break;
    }
    cout<<"------------------------------------------------------------------------------"<<endl
    <<"Press 1 to run the demostration or 0 to skip it."<<endl;
    cin >> input;
    if (input == 1)
        rmm.demonstration();

    rmm.qx=0;
    rmm.qy=0.70711;
    rmm.qz=0;
    rmm.qw=0.70711;
    rmm.MoveToCoord(1035,0,1620);
    return 0;
}