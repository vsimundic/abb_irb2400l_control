#pragma once
#define DEFAULT_BUFLEN 512
#include "opencv2/opencv.hpp"
//ROS
#include "ros/ros.h"
#include "geometry_msgs/Pose.h"
#include "sensor_msgs/Image.h"
#include <ros/callback_queue.h>
#include "cv_bridge/cv_bridge.h"
#include "abb2400_sim/move_req.h"
#include "aruco.h"
#include <ros/package.h>
// #include <stdio.h>
// #include <string>

class robotStatus
{
  public:
    double pt[3];
    double quat[4];

    void updateData()
    {
        ros::getGlobalCallbackQueue()->callAvailable(ros::WallDuration(1));
    }

    void printData()
    {
        ROS_INFO("x: %f", pt[0]);
        ROS_INFO("y: %f", pt[1]);
        ROS_INFO("z: %f\n", pt[2]);
        ROS_INFO("x: %f", quat[0]);
        ROS_INFO("y: %f", quat[1]);
        ROS_INFO("z: %f", quat[2]);
        ROS_INFO("w: %f", quat[3]);
    }

    void poseCb(const geometry_msgs::Pose::ConstPtr &currentPosemsg)
    {
        pt[0] = currentPosemsg->position.x;
        pt[1] = currentPosemsg->position.y;
        pt[2] = currentPosemsg->position.z;

        quat[0] = currentPosemsg->orientation.x;
        quat[1] = currentPosemsg->orientation.y;
        quat[2] = currentPosemsg->orientation.z;
        quat[3] = currentPosemsg->orientation.w;
    }
};

abb2400_sim::move_req makeRequest(double x, double y, double z, double a, double b, double g, double w)
{
    abb2400_sim::move_req req;
    req.request.point.x = x;
    req.request.point.y = y;
    req.request.point.z = z;

    req.request.orientation.x = a;
    req.request.orientation.y = b;
    req.request.orientation.z = g;
    req.request.orientation.w = w;
    return req;
}

class astraROS
{
	public:
		cv::Mat colorImg, depthImg;

	void depthImageCb(const sensor_msgs::Image::ConstPtr& imgCb)
	{
		// ROS_ERROR("Start of depthImageCb");
		cv_bridge::CvImagePtr cv_ptr;
		cv_ptr=cv_bridge::toCvCopy(imgCb, sensor_msgs::image_encodings::TYPE_16UC1);
		depthImg=cv_ptr->image;
		// ROS_ERROR("End of depthImageCb");

	}

	void colorImageCb(const sensor_msgs::Image::ConstPtr& imgCb)
	{
		// ROS_ERROR("Start of colorImageCb");
		cv_bridge::CvImagePtr cv_ptr;
		cv_ptr=cv_bridge::toCvCopy(imgCb, sensor_msgs::image_encodings::TYPE_8UC3);
		colorImg=cv_ptr->image;
		// ROS_ERROR("End of colorImageCb");

	}

	void updateData()
    {
        ros::getGlobalCallbackQueue()->callAvailable(ros::WallDuration(1));
    }
};

class RMM

{
	//functions:
  public:

	RMM();
	virtual ~RMM();

	void Init(std::string cam_type);
	void Finish();
	void GetImage();  //refreshes image from camera
	void Calibrate(); //calibrates camera
	void CalibrateSquareMin();

	//void CalibrateSquareMin2();
	void MoveJoints(float dq0, float dq1, float dq2); //moves each joint for a given angle difference
	void MoveToCoordinates(int x, int y, int z);	  //positions robot tool on a given coordinates
	void MoveToCoordinatesVS(int xr, int yr, int zr); // corrects robot's pose using visual servoing
	void Test1MatrixMovement();
	void IdentifyObject();
	void GetPoint(bool bObject = false);
	void CalibrateABB();
	void testMatrix();
	void errorCalib();
	void TmatrixInput();
	void ToolPositionP(float x, float y, float z, float xT, float yT, float zT);
	void PointTransformationP(float cameraX, float cameraY, float cameraZ);
	void demonstration();

	void ToolPosition(float qw, float qx, float qy, float qz, float x, float y, float z);
	void PointTransformation(float cameraX, float cameraY, float cameraZ); // saves the coordinates of the robot into RobotCoord

	void MoveToCoord(float x, float y, float z);

	//variables:
  public:
	astraROS imgAstra;
    ros::NodeHandle n;
	ros::Subscriber depthListener = n.subscribe("DepthImage",1,&astraROS::depthImageCb,&imgAstra);
	ros::Subscriber colorListener = n.subscribe("ColorImage",1, &astraROS::colorImageCb, &imgAstra);
	robotStatus rs;
    ros::ServiceClient moveRobot = n.serviceClient<abb2400_sim::move_req>("/move_req");

	std::string package_path;

	double scale;
	float MarkerSize, norma, theta;
	int speed;
	int height;
	int objectID, toolID;
	int itoolID, iobjectID;
	int xr, yr, zr;
	float ax, bx, ay, by; // correction coefficients

	int x_min;
	int x_max;
	int y_min;
	int y_max;
	int z_min;
	int z_max;
	int pointTEST;
	int pointN;
	int MarkFlag;

	float qw, qx, qy, qz;

	cv::Mat Image;
	cv::Mat ImageF;
	cv::Mat depthImg;
	cv::Mat depthImgF;
	cv::Mat depth;
	cv::Mat depthF;
	cv::Mat tRC, tMC, tGR, xRC, yRC, zRC, yRC_, tMG;
	cv::Mat TRC, TCR, TGC, TGR;
	cv::Mat rot, rotT, RRC, RRC_, RGR;
	cv::Mat objectPoints;
	cv::Mat point;
	cv::Mat tMT;
	cv::Mat RMT;
	cv::Mat RobotCoord;

	//promjena
	cv::Mat pointsRobotTEST;
	cv::Mat pointsCameraTEST;

	aruco::CameraParameters CamParam;
	aruco::MarkerDetector MDetector;
	std::vector<aruco::Marker> Markers;

	int iResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	unsigned char* img;
	int16_t* imgD;
};
