
#include <stdio.h>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <time.h>
#include <fstream>
#include <thread>

//ROS
#include "ros/ros.h"
#include "ros/console.h"

#include "sensor_msgs/Image.h"
#include "geometry_msgs/Pose.h"
#include <ros/callback_queue.h>
// #include <ros/package.h>

//Open CV
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>

//Aruco
#include "aruco.h"
#include "cvdrawingutils.h"
#include "RMM.h"


using namespace std;

RMM::RMM()
{
	package_path = ros::package::getPath("abb2400_sim");

	Image = cv::Mat(480, 640, CV_8UC3);
	ImageF = cv::Mat(480, 640, CV_8UC3);
	depthImg = cv::Mat(480, 640, CV_8UC1);
	depthImgF = cv::Mat(480, 640, CV_8UC1);
	depth = cv::Mat(480, 640, CV_16UC1, cv::Scalar::all(0));
	depthF = cv::Mat(480, 640, CV_16UC1, cv::Scalar::all(0));
	objectPoints = cv::Mat(1, 3, CV_32FC1);
	point = cv::Mat(3, 1, CV_32F);

	img = Image.data;
	imgD = new int16_t[640 * 480];

	pointTEST = 10;
	pointN = 10;

	tRC = cv::Mat(3, 1, CV_32F);
	tMC = cv::Mat(3, 1, CV_32F);
	tGR = cv::Mat(3, 1, CV_32F);
	xRC = cv::Mat(3, 1, CV_32F);
	yRC = cv::Mat(3, 1, CV_32F);
	zRC = cv::Mat(3, 1, CV_32F);
	yRC_ = cv::Mat(3, 1, CV_32F);
	tMG = cv::Mat(3, 1, CV_32F);
	TRC = cv::Mat(4, 4, CV_32F);
	TCR = cv::Mat(4, 4, CV_32F);
	TGC = cv::Mat(4, 4, CV_32F);
	TGR = cv::Mat(4, 4, CV_32F);
	rot = cv::Mat(3, 3, CV_32F);
	rotT = cv::Mat(3, 3, CV_32F);
	RRC = cv::Mat(3, 3, CV_32F);
	RRC_ = cv::Mat(3, 3, CV_32F);
	RGR = cv::Mat(3, 3, CV_32F);
	pointsRobotTEST = cv::Mat(3, pointTEST, CV_32F);
	pointsCameraTEST = cv::Mat(3, pointTEST, CV_32F);
	tMT = cv::Mat(3, 1, CV_32F);
	RMT = cv::Mat(3, 3, CV_32F);
	RobotCoord = cv::Mat(4, 1, CV_32F);

	// work space of robot
	ifstream myfile;
	myfile.open(package_path + "/workspace.txt");
	myfile >> x_min;
	myfile >> x_max;
	myfile >> y_min;
	myfile >> y_max;
	myfile >> z_min;
	myfile >> z_max;
	myfile.close();

	// set dictionary
	MDetector.setDictionary(0xFFFF);

	height = 0;
	toolID = 921;
	
	objectID = 537;
	speed = 50;
	
	MarkerSize = 0.14;
	ROS_ERROR("MARKER SIZE: %f", MarkerSize);
	
	ax = 1.0618;
	bx = -9.9546;
	ay = 1.0147;
	by = 0.8933;
}

RMM::~RMM()
{
}

void RMM::Init(std::string cam_type)
{	
	ROS_WARN("Camera type: %s", cam_type.c_str());
	if (cam_type == "astra")
	{
		ROS_WARN("Reading astra file.");
		CamParam.readFromXMLFile(package_path + "/out_camera_calibration_astra.yml");
	}
	else
	{
		ROS_WARN("Reading kinect file.");
		CamParam.readFromXMLFile(package_path + "/ost.yaml");
	}
}

void RMM::Finish()
{
}

void RMM::GetImage()
{
	imgAstra.updateData();
	Image = imgAstra.colorImg;
	depth = imgAstra.depthImg;
	ImageF = Image;
	cv::imshow("in", ImageF);

	double minVal, maxVal;
	depthF = depth;
	cv::minMaxLoc(depthF, &minVal, &maxVal);
	depthF.convertTo(depthImg, CV_8U, -255.0f / maxVal, 255.0f);
	cv::imshow("Depth image", depthImg);

	cv::waitKey(2000);
	Markers = MDetector.detect(ImageF, CamParam, MarkerSize);

	printf("%d markers detected.\n", Markers.size());

	iobjectID = itoolID = -1;

	for (int i = 0; i < Markers.size(); i++)
	{
		ROS_ERROR("MARKER ID: %d", Markers[i].id);

		if (Markers[i].id == objectID)
			iobjectID = i;
		if (Markers[i].id == toolID)
			itoolID = i;
	}
}

void RMM::GetPoint(bool bObject)
{
	MarkFlag = 0;
	int ID;
	int attempts = 0;

	do
	{
		GetImage();

		ID = (bObject ? iobjectID : itoolID);

		if (ID < 0)
		{
			attempts++;
			printf("Marker is not detected.\n");
		}
	} while (ID < 0 && attempts < 5);
	//Center of marker on 2D image
	if (attempts < 5)
	{
		objectPoints.at<float>(0, 0) = 0;
		objectPoints.at<float>(0, 1) = 0;
		objectPoints.at<float>(0, 2) = 0;
		vector<cv::Point2f> imagePoints;
		cv::projectPoints(objectPoints, Markers.at(ID).Rvec, Markers.at(ID).Tvec, CamParam.CameraMatrix, CamParam.Distorsion, imagePoints);
		float imgPxX = imagePoints.at(0).x;
		float imgPxY = imagePoints.at(0).y;
		int16_t zDepth = 0;
		int br = 0;

		cv::circle(ImageF, cv::Point(imgPxX, imgPxY), 3, cv::Scalar(0, 0, 255), -1, 8);
		cv::imshow("image2", ImageF);
		cv::circle(depthImg, cv::Point(imgPxX, imgPxY), 3, cv::Scalar(0, 0, 255), -1, 8);
		cv::imshow("image2", depthImg);
		cv::waitKey(100);

		//Searching depth based on camera

		for (int i = -2; i <= 2; i++)
		{
			for (int j = -2; j <= 2; j++)
			{
				if (depthF.at<uint16_t>(imgPxY + j, imgPxX + i) != 0)
				{
					zDepth += depthF.at<uint16_t>(imgPxY + j, imgPxX + i);
					br++;
			
				}
			}
		}		

		float zAruco = 1000 * Markers.at(ID).Tvec.at<float>(2, 0);

		float zDepth_;

		if (br == 0)
			zDepth_ = zAruco;
		else
			zDepth_ = (float)zDepth / (float)br;
		
		float scaleDepth = zDepth_ / zAruco;
		point.at<float>(0, 0) = Markers.at(ID).Tvec.at<float>(0, 0) * scaleDepth;
		point.at<float>(1, 0) = Markers.at(ID).Tvec.at<float>(1, 0) * scaleDepth;
		point.at<float>(2, 0) = Markers.at(ID).Tvec.at<float>(2, 0) * scaleDepth;
	}
	else
		MarkFlag = 1;
}

void RMM::CalibrateABB()
{

	std::stringstream ss;

	int x, y, z;

	ifstream myfile;

	int i = 0;
	int counterImage;
	float xI = 1000, yI = 1000, zI = 1000;


	/* MOJE */
	qx = -0.362125;
	qy = 0.599044;
	qz = 0.434382;
	qw = 0.566853;

	/* ORIGINALNO */
	// qw = 0.56864;
	// qx = 0.81744;
	// qy = 0.05173;
	// qz = 0.07599;
	
	
	ToolPosition(qw, qx, qy, qz, 4.5, 25, 53); // creating a rotation and translation matrix for the tool

	cv::Mat PastPoints(70, 3, CV_32F);
	cv::Mat pointsRobot(3, pointN, CV_32F), p(3, 1, CV_32F);
	cv::Mat pointsCamera(3, pointN, CV_32F);
	cv::Mat robotCentroid(3, 1, CV_32F);
	cv::Mat cameraCentroid(3, 1, CV_32F);
	cv::Mat cpointsRobot(3, pointN, CV_32F);
	cv::Mat cpointsCamera(3, pointN, CV_32F);
	float sumRobot = 0;
	float sumCamera = 0;
	cv::Mat scaleMat(3, 1, CV_32F);
	cv::Mat robInverse(1, 3, CV_32F);
	cv::Mat N(4, 4, CV_32F);
	cv::Mat lambda(4, 1, CV_32F);
	cv::Mat eigenVec(4, 4, CV_32F);
	
	string pCam_filename = package_path + "/pCamCalib.txt";
	string tMatrix_filename = package_path + "/TRC.txt";
	string sc_filename = package_path + "/sc.txt";
	string inverz_filename = package_path + "/TCR.txt";

	FILE *pCam = fopen(pCam_filename.c_str(), "w");
	FILE *tMatrix = fopen(tMatrix_filename.c_str(), "w");
	FILE *sc = fopen(sc_filename.c_str(), "w");
	FILE *inverz = fopen(inverz_filename.c_str(), "w");

	// init centroid sum
	// robot
	robotCentroid.at<float>(0, 0) = 0;
	robotCentroid.at<float>(1, 0) = 0;
	robotCentroid.at<float>(2, 0) = 0;

	// camera
	cameraCentroid.at<float>(0, 0) = 0;
	cameraCentroid.at<float>(1, 0) = 0;
	cameraCentroid.at<float>(2, 0) = 0;

	printf("\nCalibrating...\n");
	string pRobot_filename = package_path + "/pRobotCalib.txt";
	while (i < pointN)
	{

		srand(time(NULL));

		x = rand() % (x_max - x_min) + x_min;
		y = rand() % (y_max - y_min) + y_min;
		z = rand() % (z_max - z_min) + z_min;

		p.at<float>(0, 0) = x;
		p.at<float>(1, 0) = y;
		p.at<float>(2, 0) = z;
		p = RMT * tMT + p;

		pointsRobot.at<float>(0, i) = p.at<float>(0, 0);
		pointsRobot.at<float>(1, i) = p.at<float>(1, 0);
		pointsRobot.at<float>(2, i) = p.at<float>(2, 0);

		printf("\nPoint %d: x = %.f, y = %.f, z = %.f\n", i, pointsRobot.at<float>(0, i), pointsRobot.at<float>(1, i), pointsRobot.at<float>(2, i));

		try
		{
			MoveToCoord(x, y, z); // izmjena

			counterImage = 0;

			GetPoint(); // izmjena
			if (MarkFlag == 0)
			{
				FILE *pRobot = fopen(pRobot_filename.c_str(), "a");

				pointsCamera.at<float>(0, i) = point.at<float>(0, 0) * 1000;
				pointsCamera.at<float>(1, i) = point.at<float>(1, 0) * 1000;
				pointsCamera.at<float>(2, i) = point.at<float>(2, 0) * 1000;

				//suming columns in mat (centroid)
				//robot

				robotCentroid.at<float>(0, 0) += pointsRobot.at<float>(0, i);
				robotCentroid.at<float>(1, 0) += pointsRobot.at<float>(1, i);
				robotCentroid.at<float>(2, 0) += pointsRobot.at<float>(2, i);

				//camera
				cameraCentroid.at<float>(0, 0) += pointsCamera.at<float>(0, i);
				cameraCentroid.at<float>(1, 0) += pointsCamera.at<float>(1, i);
				cameraCentroid.at<float>(2, 0) += pointsCamera.at<float>(2, i);
				fprintf(pRobot, "%d\t%d\t%d\n", x, y, z);
				fprintf(pCam, "%f\t%f\t%f\n", pointsCamera.at<float>(0, i), pointsCamera.at<float>(1, i), pointsCamera.at<float>(2, i));
				fclose(pRobot);
				i++;
			}
		}
		catch (std::exception& e)
		{
			ROS_ERROR(e.what());
			return;
		}
			
	}

	fclose(pCam);

	////get centroid
	robotCentroid /= pointN;
	cameraCentroid /= pointN;

	//  refer all measurements to the centroids
	for (i = 0; i < pointN; i++)
	{

		cpointsRobot.col(i) = pointsRobot.col(i) - robotCentroid;
		cpointsCamera.col(i) = pointsCamera.col(i) - cameraCentroid;
	}

	scale = 1;
	printf("\nScale: %lf\n", scale); // scale = 1
	fprintf(sc, "%13.10lf", scale);
	fclose(sc);
	float Sxx = 0;
	float Sxy = 0;
	float Sxz = 0;
	float Syx = 0;
	float Syy = 0;
	float Syz = 0;
	float Szx = 0;
	float Szy = 0;
	float Szz = 0;

	for (i = 0; i < pointN; i++)
	{
		Sxx += cpointsRobot.at<float>(0, i) * cpointsCamera.at<float>(0, i);
		Sxy += cpointsRobot.at<float>(0, i) * cpointsCamera.at<float>(1, i);
		Sxz += cpointsRobot.at<float>(0, i) * cpointsCamera.at<float>(2, i);
		Syx += cpointsRobot.at<float>(1, i) * cpointsCamera.at<float>(0, i);
		Syy += cpointsRobot.at<float>(1, i) * cpointsCamera.at<float>(1, i);
		Syz += cpointsRobot.at<float>(1, i) * cpointsCamera.at<float>(2, i);
		Szx += cpointsRobot.at<float>(2, i) * cpointsCamera.at<float>(0, i);
		Szy += cpointsRobot.at<float>(2, i) * cpointsCamera.at<float>(1, i);
		Szz += cpointsRobot.at<float>(2, i) * cpointsCamera.at<float>(2, i);
	}

	N.at<float>(0, 0) = Sxx + Syy + Szz;
	N.at<float>(0, 1) = Syz - Szy;
	N.at<float>(0, 2) = Szx - Sxz;
	N.at<float>(0, 3) = Sxy - Syx;
	N.at<float>(1, 0) = Syz - Szy;
	N.at<float>(1, 1) = Sxx - Syy - Szz;
	N.at<float>(1, 2) = Sxy + Syx;
	N.at<float>(1, 3) = Szx + Sxz;
	N.at<float>(2, 0) = Szx - Sxz;
	N.at<float>(2, 1) = Sxy + Syx;
	N.at<float>(2, 2) = -Sxx + Syy - Szz;
	N.at<float>(2, 3) = Syz + Szy;
	N.at<float>(3, 0) = Sxy - Syx;
	N.at<float>(3, 1) = Szx + Sxz;
	N.at<float>(3, 2) = Syz + Szy;
	N.at<float>(3, 3) = -Sxx - Syy + Szz;

	//eigenvalues and eigenVector
	cv::eigen(N, lambda, eigenVec);
	//get rotation
	float qr = eigenVec.at<float>(0, 0); //quaternion coef
	float qi = eigenVec.at<float>(0, 1);
	float qj = eigenVec.at<float>(0, 2);
	float qk = eigenVec.at<float>(0, 3);

	RRC.at<float>(0, 0) = 1 - 2 * qj * qj - 2 * qk * qk;
	RRC.at<float>(0, 1) = 2 * (qi * qj - qk * qr);
	RRC.at<float>(0, 2) = 2 * (qi * qk + qj * qr);
	RRC.at<float>(1, 0) = 2 * (qi * qj + qk * qr);
	RRC.at<float>(1, 1) = 1 - 2 * qi * qi - 2 * qk * qk;
	RRC.at<float>(1, 2) = 2 * (qj * qk - qi * qr);
	RRC.at<float>(2, 0) = 2 * (qi * qk - qj * qr);
	RRC.at<float>(2, 1) = 2 * (qj * qk + qi * qr);
	RRC.at<float>(2, 2) = 1 - 2 * qi * qi - 2 * qj * qj;

	tRC = cameraCentroid - scale * RRC * robotCentroid;

	//transformation matrix
	TRC.at<float>(0, 0) = RRC.at<float>(0, 0);
	TRC.at<float>(0, 1) = RRC.at<float>(0, 1);
	TRC.at<float>(0, 2) = RRC.at<float>(0, 2);
	TRC.at<float>(0, 3) = tRC.at<float>(0);
	TRC.at<float>(1, 0) = RRC.at<float>(1, 0);
	TRC.at<float>(1, 1) = RRC.at<float>(1, 1);
	TRC.at<float>(1, 2) = RRC.at<float>(1, 2);
	TRC.at<float>(1, 3) = tRC.at<float>(1);
	TRC.at<float>(2, 0) = RRC.at<float>(2, 0);
	TRC.at<float>(2, 1) = RRC.at<float>(2, 1);
	TRC.at<float>(2, 2) = RRC.at<float>(2, 2);
	TRC.at<float>(2, 3) = tRC.at<float>(2);
	TRC.at<float>(3, 0) = 0;
	TRC.at<float>(3, 1) = 0;
	TRC.at<float>(3, 2) = 0;
	TRC.at<float>(3, 3) = 1;

	fprintf(tMatrix, "\n%13.15f\t%13.15f\t%13.15f\t%13.15f\n%13.15f\t%13.15f\t%13.15f\t%13.15f\n%13.15f\t%13.15f\t%13.15f\t%13.15f\n%13.15f\t%13.15f\t%13.15f\t%13.15f\n", TRC.at<float>(0, 0),
			TRC.at<float>(0, 1), TRC.at<float>(0, 2), TRC.at<float>(0, 3), TRC.at<float>(1, 0), TRC.at<float>(1, 1),
			TRC.at<float>(1, 2), TRC.at<float>(1, 3), TRC.at<float>(2, 0), TRC.at<float>(2, 1), TRC.at<float>(2, 2),
			TRC.at<float>(2, 3), TRC.at<float>(3, 0), TRC.at<float>(3, 1), TRC.at<float>(3, 2), TRC.at<float>(3, 3));

	fclose(tMatrix);
	cv::Mat e(3, 1, CV_32F);
	e.at<float>(0) = 0;
	e.at<float>(1) = 0;
	e.at<float>(2) = 0;

	for (i = 0; i < pointN; i++)
	{
		e += pointsCamera.col(i) - scale * RRC * pointsRobot.col(i) - tRC;
	}

	float error = cv::norm(e) * cv::norm(e);
	printf("\nNorm error e= %f", error);

	//Testing
	tMG.at<float>(0) = 0;
	tMG.at<float>(1) = 0;
	tMG.at<float>(2) = height;
	RRC_ = RRC.t();
	TCR = TRC.inv();

	fprintf(inverz, "\n%13.15f\t%13.15f\t%13.15f\t%13.15f\n%13.15f\t%13.15f\t%13.15f\t%13.15f\n%13.15f\t%13.15f\t%13.15f\t%13.15f\n%13.15f\t%13.15f\t%13.15f\t%13.15f\n", TRC.at<float>(0, 0),
			TCR.at<float>(0, 1), TCR.at<float>(0, 2), TCR.at<float>(0, 3), TCR.at<float>(1, 0), TCR.at<float>(1, 1),
			TCR.at<float>(1, 2), TCR.at<float>(1, 3), TCR.at<float>(2, 0), TCR.at<float>(2, 1), TCR.at<float>(2, 2),
			TCR.at<float>(2, 3), TCR.at<float>(3, 0), TCR.at<float>(3, 1), TCR.at<float>(3, 2), TCR.at<float>(3, 3));

	fclose(inverz);
}
// test the calibration on random  points
void RMM::testMatrix()
{
	std::stringstream ss;
	int i;
	float counterImage;
	float x, y, z;

	i = 0;

	FILE *pRobot = fopen("pRobotTEST.txt", "w");
	FILE *pCam = fopen("pCamTEST.txt", "w");
	while (i < (pointTEST))
	{

		srand(time(NULL));

		x = rand() % (x_max - x_min) + x_min;
		y = rand() % (y_max - y_min) + y_min;
		z = rand() % (z_max - z_min) + z_min;

		pointsRobotTEST.at<float>(0, i) = x;
		pointsRobotTEST.at<float>(1, i) = y;
		pointsRobotTEST.at<float>(2, i) = z;

		printf("\nPoint %d: x = %.f, y = %.f, z = %.f\n", i, pointsRobotTEST.at<float>(0, i), pointsRobotTEST.at<float>(1, i), pointsRobotTEST.at<float>(2, i));

		MoveToCoord(x, y, z);

		counterImage = 0;

		GetPoint();
		pointsCameraTEST.at<float>(0, i) = point.at<float>(0, 0) * 1000;
		pointsCameraTEST.at<float>(1, i) = point.at<float>(1, 0) * 1000;
		pointsCameraTEST.at<float>(2, i) = point.at<float>(2, 0) * 1000;

		fprintf(pRobot, "%f\t%f\t%f\n", pointsRobotTEST.at<float>(0, i), pointsRobotTEST.at<float>(1, i), pointsRobotTEST.at<float>(2, i));
		fprintf(pCam, "%f\t%f\t%f\n", pointsCameraTEST.at<float>(0, i), pointsCameraTEST.at<float>(1, i), pointsCameraTEST.at<float>(2, i));

		i++;
	}
	fclose(pRobot);
	fclose(pCam);
}
// calculates errors of calibration, for robot and camera
void RMM::errorCalib()
{

	cv::Mat shift(3, 1, CV_32F), shift_expanded(4, 1, CV_32F);
	cv::Mat temp(4, 1, CV_32F), CamCoord(4, 1, CV_32F);

	shift = RMT * tMT;
	shift_expanded.at<float>(0, 0) = shift.at<float>(0, 0);
	shift_expanded.at<float>(1, 0) = shift.at<float>(1, 0);
	shift_expanded.at<float>(2, 0) = shift.at<float>(2, 0);
	shift_expanded.at<float>(3, 0) = 0;

	FILE *fError = fopen("error.txt", "w");
	FILE *pRobot = fopen("pRobotCalc.txt", "w");
	FILE *pCam = fopen("pCamCalc.txt", "w");

	float errorX, errorY, errorZ, errorXc, errorYc, errorZc;
	int i;
	for (i = 0; i < (pointTEST); i++)
	{

		temp.at<float>(0, 0) = pointsRobotTEST.at<float>(0, i);
		temp.at<float>(1, 0) = pointsRobotTEST.at<float>(1, i);
		temp.at<float>(2, 0) = pointsRobotTEST.at<float>(2, i);
		temp.at<float>(3, 0) = 1;

		CamCoord = TRC * (temp + shift_expanded);
		PointTransformation(pointsCameraTEST.at<float>(0, i), pointsCameraTEST.at<float>(1, i), pointsCameraTEST.at<float>(2, i));

		fprintf(pRobot, "%f\t%f\t%f\n", RobotCoord.at<float>(0, 0), RobotCoord.at<float>(1, 0), RobotCoord.at<float>(2, 0));

		errorX = RobotCoord.at<float>(0, 0) - pointsRobotTEST.at<float>(0, i);
		errorY = RobotCoord.at<float>(1, 0) - pointsRobotTEST.at<float>(1, i);
		errorZ = RobotCoord.at<float>(2, 0) - pointsRobotTEST.at<float>(2, i);

		errorXc = CamCoord.at<float>(0, 0) - pointsCameraTEST.at<float>(0, i);
		errorYc = CamCoord.at<float>(1, 0) - pointsCameraTEST.at<float>(1, i);
		errorZc = CamCoord.at<float>(2, 0) - pointsCameraTEST.at<float>(2, i);

		fprintf(fError, "%f mm \t%f mm \t%f mm \t%f mm \t%f mm \t%f mm \n", errorX, errorY, errorZ, errorXc, errorYc, errorZc);
		fprintf(pCam, "%f\t%f\t%f\n", CamCoord.at<float>(0, 0), CamCoord.at<float>(1, 0), CamCoord.at<float>(2, 0));
	}
	fclose(fError);
	fclose(pRobot);
}

void RMM::TmatrixInput()
{

	qw = 0.51749;
	qx = 0.78157;
	qy = 0.2825;
	qz = 0.20383;

	ToolPosition(qw, qx, qy, qz, 4.5, 25, 53);
	string TCR_filename = package_path + "/TCR.txt";
	ifstream myfile;
	myfile.open(TCR_filename);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			myfile >> TCR.at<float>(i, j);
		}
	}
	myfile.close();

	string TRC_filename = package_path + "/TRC.txt";

	myfile.open(TRC_filename);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			myfile >> TRC.at<float>(i, j);
		}
	}
	myfile.close();
}

void RMM::demonstration()
{

	FILE *fError = fopen("errorDEM.txt", "w");

	float errorX, errorY, errorZ, errorXc, errorYc, errorZc;

	cv::Mat pointsCamera(3, 12, CV_32F);
	cv::Mat PointsDem(3, 12, CV_32F);
	cv::Mat quant(4, 12, CV_32F);

	cv::Mat shift(3, 1, CV_32F), shift_expanded(4, 1, CV_32F);
	cv::Mat temp(4, 1, CV_32F), CamCoord(4, 1, CV_32F);

	qw = 0.65945;
	qx = 0.65060;
	qy = 0.26333;
	qz = 0.26925;

	ToolPosition(qw, qx, qy, qz, -1, -80, 126 + 4);

	for (int i = 0; i < 12; i++)
	{
		toolID = 70 + i;

		GetPoint();
		pointsCamera.at<float>(0, i) = point.at<float>(0, 0) * 1000;
		pointsCamera.at<float>(1, i) = point.at<float>(1, 0) * 1000;
		pointsCamera.at<float>(2, i) = point.at<float>(2, 0) * 1000;

		PointTransformationP(point.at<float>(0, 0) * 1000, point.at<float>(1, 0) * 1000, point.at<float>(2, 0) * 1000);

		quant.at<float>(0, i) = qw;
		quant.at<float>(1, i) = qx;
		quant.at<float>(2, i) = qy;
		quant.at<float>(3, i) = qz;

		PointsDem.at<float>(0, i) = RobotCoord.at<float>(0, 0);
		PointsDem.at<float>(1, i) = RobotCoord.at<float>(1, 0);
		PointsDem.at<float>(2, i) = RobotCoord.at<float>(2, 0);
	}

	///move robot to markers

	int input;
	int flag = 0;

	do
	{

		printf("\nSelect marker, press 0 to exit demonstration\n");
		cin >> input;

		if (input > 12)
			printf("\nNo such marker, please enter valid marker number\n");

		if (input < 13 && input > 0)
		{

			qw = quant.at<float>(0, input - 1);
			qx = quant.at<float>(1, input - 1);
			qy = quant.at<float>(2, input - 1);
			qz = quant.at<float>(3, input - 1);

			ToolPosition(qw, qx, qy, qz, -1, -80, 126 + 4);

			printf("\nPoint %d: x = %.f, y = %.f, z = %.f\n", input, PointsDem.at<float>(0, input - 1), PointsDem.at<float>(1, input - 1), PointsDem.at<float>(2, input - 1));
			MoveToCoord(PointsDem.at<float>(0, input - 1), PointsDem.at<float>(1, input - 1), PointsDem.at<float>(2, input - 1));
		}
		if (input == 0)
			flag = 1;

	} while (flag == 0);

	fclose(fError);
}

void RMM::MoveToCoord(float x, float y, float z)
{
	abb2400_sim::move_req req = makeRequest(x / 1000, y / 1000, z / 1000, qx, qy, qz, qw);
	if (moveRobot.call(req))
		ROS_INFO("Server response: %i", req.response.retCode);
}

// creates rotation and translation matrix for the given tool
void RMM::ToolPosition(float qw, float qx, float qy, float qz, float x, float y, float z)
{

	RMT.at<float>(0, 0) = 1 - 2 * qy * qy - 2 * qz * qz;
	RMT.at<float>(0, 1) = 2 * qx * qy - 2 * qz * qw;
	RMT.at<float>(0, 2) = 2 * qx * qz + 2 * qy * qw;
	RMT.at<float>(1, 0) = 2 * qx * qy + 2 * qz * qw;
	RMT.at<float>(1, 1) = 1 - 2 * qx * qx - 2 * qz * qz;
	RMT.at<float>(1, 2) = 2 * qy * qz - 2 * qx * qw;
	RMT.at<float>(2, 0) = 2 * qx * qz - 2 * qy * qw;
	RMT.at<float>(2, 1) = 2 * qy * qz + 2 * qx * qw;
	RMT.at<float>(2, 2) = 1 - 2 * qx * qx - 2 * qy * qy;
	// translation of end effector
	tMT.at<float>(0, 0) = x;  
	tMT.at<float>(1, 0) = y; 
	tMT.at<float>(2, 0) = z; 
}

void RMM::PointTransformation(float cameraX, float cameraY, float cameraZ)
{

	cv::Mat shift(3, 1, CV_32F), shift_expanded(4, 1, CV_32F);

	cv::Mat temp(4, 1, CV_32F);

	shift = RMT * tMT;

	shift_expanded.at<float>(0, 0) = shift.at<float>(0, 0);
	shift_expanded.at<float>(1, 0) = shift.at<float>(1, 0);
	shift_expanded.at<float>(2, 0) = shift.at<float>(2, 0);
	shift_expanded.at<float>(3, 0) = 1;

	temp.at<float>(0, 0) = cameraX;
	temp.at<float>(1, 0) = cameraY;
	temp.at<float>(2, 0) = cameraZ;
	temp.at<float>(3, 0) = 1;

	RobotCoord = TCR * temp - shift_expanded;

} // returns coordinates of marker minus the tool shift in RobotCoord variable

void RMM::ToolPositionP(float x, float y, float z, float xT, float yT, float zT)
{

	RMT.at<float>(0, 0) = -y / (sqrt(x * x + y * y));
	RMT.at<float>(0, 1) = 0;
	RMT.at<float>(0, 2) = x / (sqrt(x * x + y * y));
	RMT.at<float>(1, 0) = x / (sqrt(x * x + y * y));
	RMT.at<float>(1, 1) = 0;
	RMT.at<float>(1, 2) = y / (sqrt(x * x + y * y));
	RMT.at<float>(2, 0) = 0;
	RMT.at<float>(2, 1) = 1;
	RMT.at<float>(2, 2) = 0;

	qw = sqrt(1 + RMT.at<float>(0, 0) + RMT.at<float>(1, 1) + RMT.at<float>(2, 2)) / 2;
	qx = (RMT.at<float>(2, 1) - RMT.at<float>(1, 2)) / (4 * qw);
	qy = (RMT.at<float>(0, 2) - RMT.at<float>(2, 0)) / (4 * qw);
	qz = (RMT.at<float>(1, 0) - RMT.at<float>(0, 1)) / (4 * qw);
	// translation of end effector
	tMT.at<float>(0, 0) = xT; 
	tMT.at<float>(1, 0) = yT; 
	tMT.at<float>(2, 0) = zT; 
}

/* transforms given camera coordinates into coordinates in robot system 
and setting the end tool position to be paralel to the xy plane*/
void RMM::PointTransformationP(float cameraX, float cameraY, float cameraZ){

	cv::Mat shift(3, 1, CV_32F), shift_expanded(4, 1, CV_32F);

	cv::Mat temp(4, 1, CV_32F);

	float x, y, z;

	temp.at<float>(0, 0) = cameraX;
	temp.at<float>(1, 0) = cameraY;
	temp.at<float>(2, 0) = cameraZ;
	temp.at<float>(3, 0) = 1;

	RobotCoord = TCR*temp;

	x = RobotCoord.at<float>(0, 0);
	y = RobotCoord.at<float>(1, 0);
	z = RobotCoord.at<float>(2, 0);

	RMT.at<float>(0, 0) = -y/(sqrt(x*x+y*y));
	RMT.at<float>(0, 1) = 0;
	RMT.at<float>(0, 2) = x / (sqrt(x*x + y*y));
	RMT.at<float>(1, 0) = x / (sqrt(x*x + y*y));
	RMT.at<float>(1, 1) = 0;
	RMT.at<float>(1, 2) = y / (sqrt(x*x + y*y));
	RMT.at<float>(2, 0) = 0;
	RMT.at<float>(2, 1) = 1;
	RMT.at<float>(2, 2) = 0;

	
	qw = sqrt(1 + RMT.at<float>(0, 0) + RMT.at<float>(1, 1) + RMT.at<float>(2, 2)) / 2;
	qx = (RMT.at<float>(2, 1) - RMT.at<float>(1, 2)) / (4 * qw);
	qy = (RMT.at<float>(0, 2) - RMT.at<float>(2, 0)) / (4 * qw);
	qz = (RMT.at<float>(1, 0) - RMT.at<float>(0, 1)) / (4 * qw);

	shift = RMT*tMT;

	shift_expanded.at<float>(0, 0) = shift.at<float>(0, 0);
	shift_expanded.at<float>(1, 0) = shift.at<float>(1, 0);
	shift_expanded.at<float>(2, 0) = shift.at<float>(2, 0);
	shift_expanded.at<float>(3, 0) = 1;

	RobotCoord = RobotCoord - shift_expanded;


} // returns coordinates of marker minus the tool shift in RobotCoord variable