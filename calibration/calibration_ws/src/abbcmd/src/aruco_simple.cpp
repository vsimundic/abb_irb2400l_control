/*****************************
Copyright 2011 Rafael Muñoz Salinas. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED BY Rafael Muñoz Salinas ''AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Rafael Muñoz Salinas OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those of the
authors and should not be interpreted as representing official policies, either expressed
or implied, of Rafael Muñoz Salinas.
********************************/

//#define simple
#ifdef simple
#include <string>
#include <iostream>
#include "aruco.h"
#include "cvdrawingutils.h"
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>


using namespace cv;
using namespace aruco;
//class for parsing command line
//operator [](string cmd) return  whether cmd is present //string operator ()(string cmd) return the value as a string: -cmd value
class CmdLineParser{int argc; char **argv; public: CmdLineParser(int _argc,char **_argv):argc(_argc),argv(_argv){}  bool operator[] ( string param ) {int idx=-1;  for ( int i=0; i<argc && idx==-1; i++ ) if ( string ( argv[i] ) ==param ) idx=i;    return ( idx!=-1 ) ;    } string operator()(string param,string defvalue="-1"){int idx=-1;    for ( int i=0; i<argc && idx==-1; i++ ) if ( string ( argv[i] ) ==param ) idx=i; if ( idx==-1 ) return defvalue;   else  return ( argv[  idx+1] ); }};

int main(int argc, char **argv) {
    try {
        CmdLineParser cml(argc,argv);
       /* if (argc ==1 || cml["-h"] ){
            cerr << "Usage: (in_image|video.avi) [-c cameraParams.yml] [-s markerSize] [-d <dicionary>:ARUCO default] [-o <outImage>] " << endl;
            cerr<<"\tDictionaries: "; for(auto dict:aruco::Dictionary::getDicTypes())    cerr<<dict<<" ";cerr<<endl;
            cout<<"Example to work with apriltags dictionary : video.avi -d TAG36h11"<<endl<<endl;
            return 0;
        }*/


        aruco::CameraParameters CamParam;

        // read the input image
        cv::Mat InImage;
        // Open input and read image
        VideoCapture vreader("D:/aruco-2.0.12/aruco-test-data-2.0/1_single/video.avi");
        
        // read camera parameters if specifed
        //if (cml["-c"]) 
		CamParam.readFromXMLFile("D:/aruco-2.0.12/aruco-test-data-2.0/1_single/intrinsics.yml");
        // read marker size if specified (default value -1)
		float MarkerSize = 0.05;
        //Create the detector
        MarkerDetector MDetector;
        MDetector.setThresholdParams(7, 7);
        MDetector.setThresholdParamRange(2, 0);
        //Set the dictionary you want to work with, if you included option -d in command line
        //see dictionary.h for all types
        if (cml["-d"])  //if the -d is in the command line
            MDetector.setDictionary( cml("-d"),0.f);//cml("-d") return the string after -d in the command line "example: ./program video.avi -d ARUCO", then, returns the string "ARUCO"
		do
		{
			if (vreader.isOpened()) vreader >> InImage;
			else{ cerr << "Could not open input" << endl; return -1; }
			// Ok, let's detect
			vector< Marker >  Markers = MDetector.detect(InImage, CamParam, MarkerSize);

			// for each marker, draw info and its boundaries in the image
			for (unsigned int i = 0; i < Markers.size(); i++) {
				cout << Markers[i] << endl;
				Markers[i].draw(InImage, Scalar(0, 0, 255), 2);
			}
			// draw a 3d cube in each marker if there is 3d info
			if (CamParam.isValid() && MarkerSize != -1)
				for (unsigned int i = 0; i < Markers.size(); i++) {
					CvDrawingUtils::draw3dCube(InImage, Markers[i], CamParam);
				}
			// show input with augmented information
			cv::namedWindow("in", 1);
			cv::imshow("in", InImage);
		}while( char(cv::waitKey(1))!=27); // wait for esc to be pressed


        if (cml["-o"]) cv::imwrite(cml("-o"), InImage);
    } catch (std::exception &ex)

    {
        cout << "Exception :" << ex.what() << endl;
    }
}

#endif

// orbbec_aruco_test.cpp : Defines the entry point for the console application.
//

#include<pcl/registration/registration.h>
//Socket
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <windows.h>

#include <stdio.h>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
//#include <C:/Users/Cupec/Downloads/AstraSDK-0.5.0-vs2013-win64/AstraSDK-0.5.0-20160426T103143Z-vs2013-win64/samples/common/key_handler.h>


//Open CV
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>

#define WIN32_LEAN_AND_MEAN

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "1025"

//Aruco
#include "aruco.h"
#include "cvdrawingutils.h"

//Python
#define _CRT_SECURE_NO_DEPRECATE
//#include <Python.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
//#include <windows.h>
//#include <winsock.h>
using namespace std;

#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

#include "C:\Users\student\aruco-2.0.12\build\utils\astraUtils.h"
#include "C:\Users\student\aruco-2.0.12\build\utils\RMM.h"

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
#include "RVLVTK.h"
#include <vtkTriangle.h>
#include <vtkAxesActor.h>
#include <vtkLine.h>
#include "RVLCore2.h"
#include "Util.h"
#include "Space3DGrid.h"
#include "Graph.h"
#include "Mesh.h"
#include "Visualizer.h"
#include "SceneSegFile.hpp"
#include "SurfelGraph.h"
#include "ObjectGraph.h"
#include "PlanarSurfelDetector.h"
#include "ObjectGraph.h"
#include "RVLRecognition.h"
#include "PSGMCommon.h"
#include "CTISet.h"
#include "VertexGraph.h"
#include "TG.h"
#include "TGSet.h"
#include "PSGM.h"
#include "ObjectDetector.h"
#include <pcl/common/common.h>
#include <pcl/PolygonMesh.h>
#include "PCLTools.h"
#include "RGBDCamera.h"
#include "PCLMeshBuilder.h"

unsigned char *img;
int16_t *imgD;

void GenerateSegmentNeighbourhood(RVL::PSGM * psgm, double radius)
{

	psgm->segmentN_PD.clear();
	pcl::PointCloud<pcl::PointXYZINormal>::Ptr cloud_destination(new pcl::PointCloud<pcl::PointXYZINormal>);
	//creating PCL point cloud
	cloud_destination->width = psgm->pMesh->NodeArray.n;
	cloud_destination->height = 1;
	cloud_destination->is_dense = false;
	cloud_destination->points.resize(cloud_destination->width * cloud_destination->height);


	int idx = 0;
	for (int i = 0; i <psgm->pMesh->NodeArray.n; i++)
	{
		if (psgm->clusterMap[psgm->pSurfels->surfelMap[i]] == -1)
			continue;

		cloud_destination->points[idx].x = psgm->pMesh->NodeArray.Element[i].P[0];
		cloud_destination->points[idx].y = psgm->pMesh->NodeArray.Element[i].P[1];
		cloud_destination->points[idx].z = psgm->pMesh->NodeArray.Element[i].P[2];

		cloud_destination->points[idx].normal_x = psgm->pMesh->NodeArray.Element[i].N[0];
		cloud_destination->points[idx].normal_y = psgm->pMesh->NodeArray.Element[i].N[1];
		cloud_destination->points[idx].normal_z = psgm->pMesh->NodeArray.Element[i].N[2];

		idx++;
	}

	pcl::search::KdTree<pcl::PointXYZINormal>::Ptr kdtree = boost::make_shared<pcl::search::KdTree<pcl::PointXYZINormal>>((new pcl::search::KdTree<pcl::PointXYZINormal>));
	kdtree->setInputCloud(cloud_destination);

	RVL::RECOG::PSGM_::Cluster *pCluster;
	RVL::Surfel *pSurfel;
	RVL::QLIST::Index2 *pt;

	//Finding centroids
	float *centroids = new float[3 * psgm->clusters.n];
	memset(centroids, 0, 3 * psgm->clusters.n*sizeof(float));
	int noPts;
	for (int iCluster = 0; iCluster < psgm->clusters.n; iCluster++)
	{
		pCluster = psgm->clusters.Element[iCluster];
		noPts = 0;
		for (int i = 0; i < pCluster->iSurfelArray.n; i++)
		{
			pSurfel = &psgm->pSurfels->NodeArray.Element[pCluster->iSurfelArray.Element[i]];
			pt = pSurfel->PtList.pFirst;
			while (pt)
			{
				centroids[3 * iCluster] += psgm->pMesh->NodeArray.Element[pt->Idx].P[0];
				centroids[3 * iCluster + 1] += psgm->pMesh->NodeArray.Element[pt->Idx].P[1];
				centroids[3 * iCluster + 2] += psgm->pMesh->NodeArray.Element[pt->Idx].P[2];
				noPts++;
				pt = pt->pNext;
			}
		}
		centroids[3 * iCluster] /= noPts;
		centroids[3 * iCluster + 1] /= noPts;
		centroids[3 * iCluster + 2] /= noPts;
	}

	std::vector<int> pointIdxRadiusSearch; //to store index of surrounding points
	std::vector<float> pointRadiusSquaredDistance; // to store distance to surrounding points
	pcl::PointXYZINormal searchPoint;
	vtkSmartPointer<vtkPoints> points;
	vtkSmartPointer<vtkFloatArray> normals;
	vtkSmartPointer<vtkCellArray> verts;
	vtkSmartPointer<vtkPolyData> PD;
	int ptIdx = 0;
	for (int iCluster = 0; iCluster < psgm->clusters.n; iCluster++)
	{
		points = vtkSmartPointer<vtkPoints>::New();
		normals = vtkSmartPointer<vtkFloatArray>::New();
		normals->SetNumberOfComponents(3);
		verts = vtkSmartPointer<vtkCellArray>::New();
		searchPoint.x = centroids[3 * iCluster];
		searchPoint.y = centroids[3 * iCluster + 1];
		searchPoint.z = centroids[3 * iCluster + 2];

		pointIdxRadiusSearch.clear();
		pointRadiusSquaredDistance.clear();
		kdtree->radiusSearch(searchPoint, radius, pointIdxRadiusSearch, pointRadiusSquaredDistance);
		ptIdx = 0;
		for (int i = 0; i < pointIdxRadiusSearch.size(); i++)
		{
			points->InsertNextPoint(cloud_destination->points[pointIdxRadiusSearch.at(i)].x, cloud_destination->points[pointIdxRadiusSearch.at(i)].y, cloud_destination->points[pointIdxRadiusSearch.at(i)].z);
			normals->InsertNextTuple(cloud_destination->points[pointIdxRadiusSearch.at(i)].normal);
			verts->InsertNextCell(1);
			verts->InsertCellPoint(ptIdx);
			ptIdx++;
		}

		PD = vtkSmartPointer<vtkPolyData>::New();
		PD->SetPoints(points);
		PD->GetPointData()->SetNormals(normals);
		PD->SetVerts(verts);

		//subsampling the scene:
		vtkSmartPointer<vtkCleanPolyData> cleanFilter = vtkSmartPointer<vtkCleanPolyData>::New();
		cleanFilter->SetInputData(PD);
		cleanFilter->PointMergingOn();
		cleanFilter->SetAbsoluteTolerance(0.005);
		cleanFilter->ToleranceIsAbsoluteOn();
		cleanFilter->Update();

		psgm->segmentN_PD.insert(std::make_pair(iCluster, cleanFilter->GetOutput()));
	}
	delete[] centroids;

}

int __cdecl main(int argc, char** argv)
{
	RMM rmm;
	rmm.Init();

	//RVL
//#define OBJECT_DETECTION
#ifdef OBJECT_DETECTION

		CRVLMem mem0;	// permanent memory

		mem0.Create(1000000);

		CRVLMem mem;	// cycle memory

		mem.Create(1000000000);

		//char cfgFileName[] = "C:\\Users\\Petra\\Documents\\RVL\\RVLObjectDetectionDemo\\RVLObjectDetectionDemo.cfg";
		char cfgFileName[] = "C:\\Users\\student\\RVL\\RVLRecognitionDemo\\RVLRecognitionDemo_all.cfg";
		DWORD flags = 0x00000000;
		// Create mesh builder.

		RVL::PCLMeshBuilder meshBuilder;
		RVL::ObjectDetector objectDetector;

		meshBuilder.CreateParamList(&mem0);

		meshBuilder.ParamList.LoadParams(cfgFileName);

		int w = 640;
		int h = 480;

		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr PC(new pcl::PointCloud<pcl::PointXYZRGBA>(w, h));

		meshBuilder.PC = PC;

		// Initialize surfel detection

		RVL::SurfelGraph surfels;

		surfels.pMem = &mem;

		surfels.CreateParamList(&mem0);

		surfels.ParamList.LoadParams(cfgFileName);

		RVL::PlanarSurfelDetector surfelDetector;

		surfelDetector.CreateParamList(&mem0);

		surfelDetector.ParamList.LoadParams(cfgFileName);

		// Initialize object detector.
		objectDetector.pMem0 = &mem0;
		objectDetector.pMem = &mem;
		objectDetector.cfgFileName = RVLCreateString("C:\\Users\\student\\RVL\\RVLObjectDetectionDemo\\RVLObjectDetectionDemo.cfg");

		objectDetector.Init();

		objectDetector.vpMeshBuilder = &meshBuilder;
		objectDetector.LoadMesh = RVL::LoadMesh;
		objectDetector.CreateMesh = RVL::CreateMesh;

		objectDetector.pSurfelDetector->pTimer = new CRVLTimer;

		RVL::Array2D<short int> depthImage;
		cv::Mat pMC_(3, 1, CV_32F);
		//rmm.GetPoint(rmm.objectID);
		//rmm.point.copyTo(pMC_);
		astra_temp_update();
		cv::flip(rmm.Image, rmm.ImageF, 1);
		cv::imshow("in", rmm.ImageF);

		memcpy(rmm.depth.data, imgD, w * h * sizeof(int16_t));
		double minVal, maxVal;
		cv::flip(rmm.depth, rmm.depthF, 1);
		cv::minMaxLoc(rmm.depthF, &minVal, &maxVal);
		rmm.depthF.convertTo(rmm.depthImg, CV_8U, -255.0f / maxVal, 255.0f);
		cv::imshow("Depth image", rmm.depthImg);

		//cv::waitKey();

		depthImage.Element = (short *)(rmm.depthF.data);
		depthImage.w = w;
		depthImage.h = h;

		mem.Clear();

		//objectDetector.DetectObjects(NULL, &depthImage, NULL); //captures an image from the camera -> for realtime detectObjects
		//objectDetector.DetectObjects("scene.ply", NULL, NULL);

		// Initialize recognition.
		RVL::PSGM recognition;

		recognition.CreateParamList(&mem0);

		recognition.ParamList.LoadParams(cfgFileName);

		//recognition.Create();

		recognition.pMem = &mem;
		recognition.pMem0 = &mem0;

		recognition.vpMeshBuilder = &meshBuilder;
		recognition.LoadMesh = RVL::LoadMesh;

		recognition.pSurfels = &surfels;

		recognition.pSurfelDetector = &surfelDetector;

		recognition.MTGSet.pMem = recognition.pMem0;

		recognition.Init(cfgFileName);

		recognition.LoadModelDataBase();
		recognition.LoadModelMeshDB("C:\\Users\\student\\FERIT_dataset\\modelsFERIT.txt", &recognition.vtkModelDB, false, 0.4);

		recognition.resultsFolder = "C:\\Users\\student\\FERIT_dataset\\ExpRez";

		RVL::Mesh mesh;
		LoadMesh(&meshBuilder, "C:\\Users\\student\\aruco-2.0.12\\build\\utils\\scene.ply", &mesh, false);
		mem.Clear();
		recognition.Interpret(&mesh);

		GenerateSegmentNeighbourhood(&recognition, 0.1);
		recognition.ICP_refined(RVL::PCLICP, RVL::PCLICPVariants::Point_to_plane, recognition.bestSceneSegmentMatches2);
		recognition.HypothesisEvaluation(recognition.bestSceneSegmentMatches2, true);

		//Colision check
		recognition.noCollisionHypotheses.clear();
		recognition.transparentHypotheses.clear();
		recognition.envelopmentColisionHypotheses.clear();
		recognition.GetHypothesesCollisionConsensus(&recognition.noCollisionHypotheses, &recognition.bestSceneSegmentMatches2, 10);


		unsigned char SelectionColor[3];

		SelectionColor[0] = 0;
		SelectionColor[1] = 255;
		SelectionColor[2] = 0;
		RVL::Visualizer visualizer;
		visualizer.Create();
		visualizer.renderer->RemoveAllViewProps();
		recognition.InitDisplay(&visualizer, &mesh, SelectionColor);
		recognition.GetTransparencyAndCollisionConsensus(&visualizer);
		visualizer.Run();

		RVL::RECOG::PSGM_::MatchInstance *pHypothesis = recognition.GetMatch(recognition.consensusHypotheses[0]);
		cv::Mat pRFMC(3, 1, CV_32F);

		pRFMC.at<float>(0) = pHypothesis->tICP[0];
		pRFMC.at<float>(1) = pHypothesis->tICP[1];
		pRFMC.at<float>(2) = pHypothesis->tICP[2];
		pOC = pRFMC;
#endif
		//end RVL

	//WSADATA wsaData;
	//SOCKET ConnectSocket = INVALID_SOCKET;
	rmm.ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	//char *sendbuf = "[1000, 0, 1200]";
	//memcpy(sendbuf, &x, sizeof(int));
	//char recvbuf[DEFAULT_BUFLEN];
	memset(rmm.recvbuf, 0, DEFAULT_BUFLEN);
	//int iResult;
	//int recvbuflen = DEFAULT_BUFLEN;

	// Validate the parameters
	if (argc != 2) {
		printf("usage: %s server-name\n", argv[0]);
		return 1;
	}

	// Initialize Winsock
	rmm.iResult = WSAStartup(MAKEWORD(2, 2), &rmm.wsaData);
	if (rmm.iResult != 0) {
		printf("WSAStartup failed with error: %d\n", rmm.iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	rmm.iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (rmm.iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", rmm.iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		rmm.ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (rmm.ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		rmm.iResult = connect(rmm.ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (rmm.iResult == SOCKET_ERROR) {
			closesocket(rmm.ConnectSocket);
			rmm.ConnectSocket = INVALID_SOCKET;
			continue;
		}

		break;
	}

	freeaddrinfo(result);

	if (rmm.ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	//int coordinates[9] = { 900, 0, 1200, 900, 100, 1200, 1000, 500, 1200 };
	//for (int i = 0; i < 3; i++)
	//{
	//	// Send a buffer
	//	ss.clear();
	//	ss.str("");
	//	ss << "[" << coordinates[i*3] << ", " << coordinates[i*3+1] << ", " << coordinates[i*3+2] << "]";
	//	iResult = send(ConnectSocket, ss.str().c_str(), ss.str().length(), 0);
	//	if (iResult == SOCKET_ERROR) {
	//		printf("send failed with error: %d\n", WSAGetLastError());
	//		closesocket(ConnectSocket);
	//		WSACleanup();
	//		return 1;
	//	}

	int input;



	printf("------------------------------------------------------------------------------");
	printf("\nPress 1 to initiate EtoH calibration, 2 to initiate EinH calibration or press 0 to load transformation matrix.\n");
	cin >> input;


	if (input == 1){
		rmm.CalibrateABB();
		printf("------------------------------------------------------------------------------");
		printf("\nDo you wish to test the calibration? Press 1 for yes or 0 for no.\n");
		cin >> input;

		if (input == 1){
			rmm.testMatrix();
			rmm.errorCalib();
		}
	}
	else if (input == 2)
	{
		rmm.CalibrateABBEinH();
	}
	else {
		rmm.TmatrixInput();
		printf("------------------------------------------------------------------------------");
		printf("\nDo you wish to test the calibration? Press 1 for yes or 0 for no.\n");
		cin >> input;

		if (input == 1){
			rmm.testMatrix();
			rmm.errorCalib();
		}
	}
	printf("------------------------------------------------------------------------------");
	printf("\nPress 1 to run the demostration, 2 for object detection or 0 to skip it.\n");
	cin >> input;
	if (input == 1){
		rmm.demonstration();
	}
	if (input == 2)
	{
		rmm.moveAboveObject();
	}


	







	// shutdown the connection since no more data will be sent
	rmm.iResult = send(rmm.ConnectSocket, "Close", 5, 0);
	Sleep(5000);
	rmm.iResult = shutdown(rmm.ConnectSocket, SD_SEND);
	if (rmm.iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(rmm.ConnectSocket);
		WSACleanup();
		return 1;
	}
	// cleanup
	closesocket(rmm.ConnectSocket);
	WSACleanup();







	//RANSACRec();

	//rmm.Init();
	//rmm.Calibrate();
	//rmm.MoveToCoordinates(150, 0, 150);
	//rmm.CalibrateSquareMin();
	//rmm.MoveToCoordinates(170, 0, 100);
	//rmm.Test1MatrixMovement();
	//rmm.GetImage();
	//Sleep(5000);
	//rmm.MoveToCoordinates(170, 0, 100);
	//rmm.GetImage();
	//rmm.IdentifyObject();
	//rmm.MoveToCoordinatesVS(rmm.xr, rmm.yr, rmm.zr);

	//rmm.objectID = 176;
	//for (int i = 0; i < 30; i++)
	//{
	//	rmm.IdentifyObject();
	//	rmm.MoveToCoordinatesVS(rmm.xr, rmm.yr, rmm.zr);
	//	cv::waitKey();
	//	rmm.MoveToCoordinates(170, 0, 90);
	//}

	/*for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 9; i++)
		{
			if (i != rmm.itoolID)
			{
				if (rmm.objectID == 150 || rmm.objectID == 159)
				{
				rmm.IdentifyObject();
				rmm.MoveToCoordinatesVS(rmm.xr, rmm.yr, rmm.zr);
				cv::waitKey();
				rmm.MoveToCoordinates(170, 0, 90);
				}
				rmm.objectID++;
			}
			
			if (rmm.objectID == 154) rmm.objectID++;
		}
		
		
	}*/
	
	
//	astra::initialize();
//	//set_key_handler();
//	//sf::RenderWindow window(sf::VideoMode(1280, 960), "Color Viewer");
//	astra::StreamSet streamSet;
//	astra::StreamReader reader = streamSet.create_reader();
//	astra::ImageStreamMode mode;
//	mode.set_height(480);
//	mode.set_width(640);
//	mode.set_fps(30);
//	mode.set_pixel_format(astra_pixel_formats::ASTRA_PIXEL_FORMAT_RGB888);
//	reader.stream<astra::ColorStream>().set_mode(mode);
//	reader.stream<astra::ColorStream>().start();
//
//
//	ColorFrameListener listener;
//	reader.add_listener(listener);
//
//	aruco::CameraParameters CamParam;
//	cv::Mat Image(480, 640, CV_8UC3);
//	cv::Mat ImageF(480, 640, CV_8UC3);
//	img = Image.data;
//
//	CamParam.readFromXMLFile("D:/aruco-2.0.12/build/bin/Release/out_camera_calibration.yml");//D:/aruco-2.0.12/aruco-test-data-2.0/1_single/intrinsics.yml
//	float MarkerSize = 0.03;
//	aruco::MarkerDetector MDetector;
//	MDetector.setThresholdParams(7, 7);
//	MDetector.setThresholdParamRange(2, 0);
//	MDetector.setCornerRefinementMethod(aruco::MarkerDetector::CornerRefinementMethod::LINES);
//	cv::namedWindow("in", 1);
//
//	////for video recording
//	//cv::VideoWriter vw("dobot.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, cv::Size(640, 480));
//	//
//
//	//Python code
//	PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *pArgs, *pClass, *pInstance;
//	
//
//	// Initialize the Python Interpreter
//	Py_Initialize();
//
//
//	PyObject *sys = PyImport_ImportModule("sys");
//	PyObject *path = PyObject_GetAttrString(sys, "path");
//	PyList_Append(path, PyString_FromString("D:/open-dobot-master/application/python/dobot"));
//
//	// Build the name object
//	pName = PyString_FromString("Robot");
//
//	// Load the module object
//	pModule = PyImport_Import(pName);
//
//	// pDict is a borrowed reference 
//	pDict = PyModule_GetDict(pModule);
//	pClass = PyDict_GetItemString(pDict, "Robot");
//	pInstance = PyObject_CallObject(pClass, NULL);
//	pArgs = PyTuple_New(4);
//
//	int i = 0;
//	int x, y, z, speed;
//
//	x = 180;
//	y = -50;
//	z = 150;
//	speed = 50;
//
//
//	//Communication opening
//	pValue = PyObject_CallMethod(pInstance, "Open", NULL);
//
//	//Initializing Dobot position
//	printf("\nInitializing position.\n");
//	pValue = PyObject_CallMethod(pInstance, "MoveJointsInitial", NULL);
//
//
//
//	pValue = PyObject_CallMethod(pInstance, "MoveToInitial", NULL);
//	pValue = PyObject_CallMethod(pInstance, "Move", "(iiii)", x, y, z, speed);
//	Sleep(5000);
//	
//	//Calibration with three points
//	//------------------------------
//	printf("\nNow calibrating camera.\n");
//	int a = 60, b = 60;
//	cv::Mat pR_1(3, 1, CV_32F), pR_2(3, 1, CV_32F), pR_3(3, 1, CV_32F); //points from Robot
//	cv::Mat pC_1(3, 1, CV_32F), pC_2(3, 1, CV_32F), pC_3(3, 1, CV_32F); //points from Camera
//	cv::Mat tRC(3, 1, CV_32F), tMC(3, 1, CV_32F), tGR(3, 1, CV_32F), xRC(3, 1, CV_32F), yRC(3, 1, CV_32F), zRC(3, 1, CV_32F), yRC_(3, 1, CV_32F), cross(3, 1, CV_32F), inverse(3, 1, CV_32F), tMG(3, 1, CV_32F);
//	cv::Mat TRC(4, 4, CV_32F), TCR(4, 4, CV_32F), TGC(4, 4, CV_32F), TGR(4, 4, CV_32F);
//	cv::Mat rot(3, 3, CV_32F), rotT(3, 3, CV_32F), RRC(3, 3, CV_32F), RRC_(3, 3, CV_32F), RGR(3, 3, CV_32F);
//	float norma, theta;
//
//	pR_1.at<float>(0, 0) = x;
//	pR_1.at<float>(1, 0) = y;
//	pR_1.at<float>(2, 0) = z;
//	pR_2.at<float>(0, 0) = x+a;
//	pR_2.at<float>(1, 0) = y;
//	pR_2.at<float>(2, 0) = z;
//	pR_3.at<float>(0, 0) = x+a;
//	pR_3.at<float>(1, 0) = y+b;
//	pR_3.at<float>(2, 0) = z;
//
//
//	vector< aruco::Marker >  Markers; 
//	//Searching for a marker on a scene
//	do
//	{
//		astra_temp_update();
//		cv::flip(Image, ImageF, 1);
//		cv::imshow("in", ImageF);
//		//printf("Press a key to continue.\n");
//		cv::waitKey();		
//		Markers = MDetector.detect(ImageF, CamParam, MarkerSize);
//	} while (Markers.size() == 0);
//	
//	int n;
//	if (Markers.size() == 1) n = 0;
//	else n = 1;
//
//	//Center of marker on 2D imaeg
//	cv::Mat objectPoints(1, 3, CV_32FC1);
//	objectPoints.at< float >(0, 0) = 0;
//	objectPoints.at< float >(0, 1) = 0;
//	objectPoints.at< float >(0, 2) = 0;
//	vector< cv::Point2f > imagePoints;
//	cv::projectPoints(objectPoints, Markers.at(0).Rvec, Markers.at(0).Tvec, CamParam.CameraMatrix, CamParam.Distorsion, imagePoints);
//	float imgPxX = imagePoints.at(0).x;
//	float imgPxY = imagePoints.at(0).y;
//
//	//Point 1
//	Markers[n].Tvec.copyTo(pC_1);
//	pC_1 = pC_1 * 1000;
//
//	printf("\nPose Point 1 TGC: x = %.f, y = %.f, z = %.f\n",
//		pC_1.at<float>(0), pC_1.at<float>(1), pC_1.at<float>(2) );
//
//	//Point 2
// 	x += a;
//	pValue = PyObject_CallMethod(pInstance, "Move", "(iiii)", x, y, z, speed);
//	Sleep(2000);
//	astra_temp_update();
//	cv::flip(Image, ImageF, 1);
//	cv::imshow("in", ImageF);
//	Markers = MDetector.detect(ImageF, CamParam, MarkerSize);
//	if (Markers.size() == 1) n = 0;
//	else n = 1;
//	Markers[n].Tvec.copyTo(pC_2);
//	pC_2 = pC_2 * 1000;
//
//	printf("\nPose Point 2 TGC: x = %.f, y = %.f, z = %.f\n",
//		pC_2.at<float>(0), pC_2.at<float>(1), pC_2.at<float>(2));
//	
//	//Point 3
//	y += b;
//	pValue = PyObject_CallMethod(pInstance, "Move", "(iiii)", x, y, z, speed);
//	Sleep(2000);
//	astra_temp_update();
//	cv::flip(Image, ImageF, 1);
//	cv::imshow("in", ImageF);
//	Markers = MDetector.detect(ImageF, CamParam, MarkerSize);
//	if (Markers.size() == 1) n = 0;
//	else n = 1;
//	Markers[n].Tvec.copyTo(pC_3);
//	pC_3 = pC_3 * 1000;
//
//	printf("\nPose Point 3 TGC: x = %.f, y = %.f, z = %.f\n",
//		pC_3.at<float>(0), pC_3.at<float>(1), pC_3.at<float>(2));
//
//	
//	//Getting Transformation Matrix
//	
//	norma = norm(pC_2 - pC_1);
//	xRC = (pC_2 - pC_1) / norma; //xRC
//	yRC_ = pC_3 - pC_2;
//	cross = xRC.cross(yRC_);
//	zRC = cross / norm(cross); //zRC
//	yRC = zRC.cross(xRC); //yRC
//
//	RRC.at<float>(0, 0) = xRC.at<float>(0);
//	RRC.at<float>(0, 1) = yRC.at<float>(0);
//	RRC.at<float>(0, 2) = zRC.at<float>(0);
//	RRC.at<float>(1, 0) = xRC.at<float>(1);
//	RRC.at<float>(1, 1) = yRC.at<float>(1);
//	RRC.at<float>(1, 2) = zRC.at<float>(1);
//	RRC.at<float>(2, 0) = xRC.at<float>(2);
//	RRC.at<float>(2, 1) = yRC.at<float>(2);
//	RRC.at<float>(2, 2) = zRC.at<float>(2);
//
//	tMG.at<float>(0) = 0;
//	tMG.at<float>(1) = 0;
//	tMG.at<float>(2) = 31;
//
//	tRC = pC_1 - RRC*(tMG + pR_1);
//
//	TRC.at<float>(0, 0) = xRC.at<float>(0);
//	TRC.at<float>(0, 1) = yRC.at<float>(0);
//	TRC.at<float>(0, 2) = zRC.at<float>(0);
//	TRC.at<float>(0, 3) = tRC.at<float>(0);
//	TRC.at<float>(1, 0) = xRC.at<float>(1);
//	TRC.at<float>(1, 1) = yRC.at<float>(1);
//	TRC.at<float>(1, 2) = zRC.at<float>(1);
//	TRC.at<float>(1, 3) = tRC.at<float>(1);
//	TRC.at<float>(2, 0) = xRC.at<float>(2);
//	TRC.at<float>(2, 1) = yRC.at<float>(2);
//	TRC.at<float>(2, 2) = zRC.at<float>(2);
//	TRC.at<float>(2, 3) = tRC.at<float>(2);
//	TRC.at<float>(3, 0) = 0;
//	TRC.at<float>(3, 1) = 0;
//	TRC.at<float>(3, 2) = 0;
//	TRC.at<float>(3, 3) = 1;
//
//	TCR = TRC.inv();
//	TCR.at<float>(0, 3) += 210.9; //translation from R' to R
//	TCR.at<float>(2, 3) += 238; //translation from R' to R
//	printf("\nCalibration done!\n");
//	//End of calibration	
//	
////#define test1
//#ifdef test1
////Test 1 - matrix movement:
//	a = 165; b = -50; int c = 130; int sign = 1;
//	pValue = PyObject_CallMethod(pInstance, "Move", "(iiii)", a, b, c, speed); //upper corner of matrix
//	Sleep(2000);
//
//	int niz[100][2];
//
//	FILE *robot = fopen("robot.txt", "w");
//	for (i = 0; i < 100; i++)
//	{
//		
//		if ((i+1) % 10 != 0)
//		{
//			b += sign * 10;
//		}
//		if ((i+1) % 10 == 0 && i!=0)
//		{
//			sign *= -1;
//			a += 10;
//		}
//		niz[i][0] = a;
//		niz[i][1] = b;
//		fprintf(robot, "%d %d\n", a,b);
//
//	}
//	fclose(robot);
//
//	printf("\nMatrix movement:\n");
//	FILE *tmc = fopen("tmc.txt", "w");
//	FILE *tgr = fopen("tgr.txt", "w");
//
//	for (i = 0; i < 100; i++)
//	{
//		a = niz[i][0];
//		b = niz[i][1];
//		pValue = PyObject_CallMethod(pInstance, "Move", "(iiii)", a, b, c, speed);
//		cv::waitKey();
//
//		astra_temp_update();
//		cv::flip(Image, ImageF, 1);
//
//		Markers = MDetector.detect(ImageF, CamParam, MarkerSize);
//
//		// for each marker, draw info and its boundaries in the image
//		for (unsigned int i = 0; i < Markers.size(); i++)
//		{
//			for (unsigned int i = 0; i < Markers.size(); i++)
//			{
//				cv::Rodrigues(Markers[i].Rvec, rot); // transformation to rot. matrix 3x3
//
//
//				//rotT = rot.t();
//				//TGC.at<float>(0, 0) = rot.at<float>(0, 0);
//				//TGC.at<float>(0, 1) = rot.at<float>(0, 1);
//				//TGC.at<float>(0, 2) = rot.at<float>(0, 2);
//				//TGC.at<float>(0, 3) = 1000 * Markers[i].Tvec.at<float>(0);
//				//TGC.at<float>(1, 0) = rot.at<float>(1, 0);
//				//TGC.at<float>(1, 1) = rot.at<float>(1, 1);
//				//TGC.at<float>(1, 2) = rot.at<float>(1, 2);
//				//TGC.at<float>(1, 3) = 1000 * Markers[i].Tvec.at<float>(1);
//				//TGC.at<float>(2, 0) = rot.at<float>(2, 0);
//				//TGC.at<float>(2, 1) = rot.at<float>(2, 1);
//				//TGC.at<float>(2, 2) = rot.at<float>(2, 2);
//				//TGC.at<float>(2, 3) = 1000 * Markers[i].Tvec.at<float>(2);
//				//TGC.at<float>(3, 0) = 0;
//				//TGC.at<float>(3, 1) = 0;
//				//TGC.at<float>(3, 2) = 0;
//				//TGC.at<float>(3, 3) = 1;
//
//
//				tMC.at<float>(0) = 1000 * Markers[i].Tvec.at<float>(0);
//				tMC.at<float>(1) = 1000 * Markers[i].Tvec.at<float>(1);
//				tMC.at<float>(2) = 1000 * Markers[i].Tvec.at<float>(2);
//
//				theta = atan2(0, 210);
//
//				RGR.at<float>(0, 0) = cos(theta);
//				RGR.at<float>(0, 1) = -sin(theta);
//				RGR.at<float>(0, 2) = 0;
//				RGR.at<float>(1, 0) = sin(theta);
//				RGR.at<float>(1, 1) = cos(theta);
//				RGR.at<float>(1, 2) = 0;
//				RGR.at<float>(2, 0) = 0;
//				RGR.at<float>(2, 1) = 0;
//				RGR.at<float>(2, 2) = 1;
//
//				//TGR = TCR*TGC;
//				RRC_ = RRC.t();
//				tGR = RRC_*(tMC - tRC) - RGR*tMG;//estimated tGR
//
//				fprintf(tmc, "%f %f %f\n", tMC.at<float>(0), tMC.at<float>(1), tMC.at<float>(2));
//				fprintf(tgr, "%f %f %f\n", tGR.at<float>(0), tGR.at<float>(1), tGR.at<float>(2));
//
//				printf("\nPose TGC: x = %.f, y = %.f, z = %.f\nPose TGR: x = %.f, y = %.f, z = %.f",
//					tMC.at<float>(0), tMC.at<float>(1), tMC.at<float>(2),
//					tGR.at<float>(0), tGR.at<float>(1), tGR.at<float>(2));
//				
//				Markers[i].draw(ImageF, cv::Scalar(0, 0, 255), 2);
//			}
//			// draw a 3d cube in each marker if there is 3d info
//			if (CamParam.isValid() && MarkerSize != -1)
//				for (unsigned int i = 0; i < Markers.size(); i++) {
//					aruco::CvDrawingUtils::draw3dAxis(ImageF, Markers[i], CamParam);
//				}
//
//			//show input with augmented information
//			cv::imshow("in", ImageF);
//
//			/*cv::Rodrigues(Markers[i].Rvec, rot); // transformation to rot. matrix 3x3
//
//			tMC.at<float>(0) = 1000 * Markers[i].Tvec.at<float>(0);
//			tMC.at<float>(1) = 1000 * Markers[i].Tvec.at<float>(1);
//			tMC.at<float>(2) = 1000 * Markers[i].Tvec.at<float>(2);
//
//			theta = atan2(tMC.at<float>(1), tMC.at<float>(0));
//
//			RGR.at<float>(0, 0) = cos(theta);
//			RGR.at<float>(0, 1) = -sin(theta);
//			RGR.at<float>(0, 2) = 0;
//			RGR.at<float>(1, 0) = sin(theta);
//			RGR.at<float>(1, 1) = cos(theta);
//			RGR.at<float>(1, 2) = 0;
//			RGR.at<float>(2, 0) = 0;
//			RGR.at<float>(2, 1) = 0;
//			RGR.at<float>(2, 2) = 1;
//
//			//TGR = TCR*TGC;
//			RRC_ = RRC.t();
//			tGR = RRC_*(tMC-tRC)-RGR*tMG;
//
//			printf("\nPose TGC: x = %.f, y = %.f, z = %.f\nPose TGR: x = %.f, y = %.f, z = %.f",
//				TGC.at<float>(0, 3), TGC.at<float>(1, 3), TGC.at<float>(2, 3),
//				tGR.at<float>(0), tGR.at<float>(1), tGR.at<float>(2));
//			Markers[i].draw(ImageF, cv::Scalar(0, 0, 255), 2);*/
//		}
//
//	}
//	fclose(tmc);
//	fclose(tgr);		
//#endif
////end of Test 1
//
//		/*do{
//		pValue = PyObject_CallMethod(pInstance, "Move", "(iiii)", 210, 0, 238, speed);
//		cv::waitKey();
//		astra_temp_update();
//		cv::flip(Image, ImageF, 1);
//		
//		Markers = MDetector.detect(ImageF, CamParam, MarkerSize);
//
//		// for each marker, draw info and its boundaries in the image
//		for (unsigned int i = 0; i < Markers.size(); i++)
//		{
//			cv::Rodrigues(Markers[i].Rvec, rot); // transformation to rot. matrix 3x3
//
//			
//			//rotT = rot.t();
//			//TGC.at<float>(0, 0) = rot.at<float>(0, 0);
//			//TGC.at<float>(0, 1) = rot.at<float>(0, 1);
//			//TGC.at<float>(0, 2) = rot.at<float>(0, 2);
//			//TGC.at<float>(0, 3) = 1000 * Markers[i].Tvec.at<float>(0);
//			//TGC.at<float>(1, 0) = rot.at<float>(1, 0);
//			//TGC.at<float>(1, 1) = rot.at<float>(1, 1);
//			//TGC.at<float>(1, 2) = rot.at<float>(1, 2);
//			//TGC.at<float>(1, 3) = 1000 * Markers[i].Tvec.at<float>(1);
//			//TGC.at<float>(2, 0) = rot.at<float>(2, 0);
//			//TGC.at<float>(2, 1) = rot.at<float>(2, 1);
//			//TGC.at<float>(2, 2) = rot.at<float>(2, 2);
//			//TGC.at<float>(2, 3) = 1000 * Markers[i].Tvec.at<float>(2);
//			//TGC.at<float>(3, 0) = 0;
//			//TGC.at<float>(3, 1) = 0;
//			//TGC.at<float>(3, 2) = 0;
//			//TGC.at<float>(3, 3) = 1;
//			
//
//			tMC.at<float>(0) = 1000 * Markers[i].Tvec.at<float>(0);
//			tMC.at<float>(1) = 1000 * Markers[i].Tvec.at<float>(1);
//			tMC.at<float>(2) = 1000 * Markers[i].Tvec.at<float>(2);
//
//			theta = atan2(0,210);
//
//			RGR.at<float>(0, 0) = cos(theta);
//			RGR.at<float>(0, 1) = -sin(theta);
//			RGR.at<float>(0, 2) = 0;
//			RGR.at<float>(1, 0) = sin(theta);
//			RGR.at<float>(1, 1) = cos(theta);
//			RGR.at<float>(1, 2) = 0;
//			RGR.at<float>(2, 0) = 0;
//			RGR.at<float>(2, 1) = 0;
//			RGR.at<float>(2, 2) = 1;
//
//			//TGR = TCR*TGC;
//			RRC_ = RRC.t();
//			tGR = RRC_*(tMC - tRC) - RGR*tMG;//estimated tGR
//
//			printf("\nPose TGC: x = %.f, y = %.f, z = %.f\nPose TGR: x = %.f, y = %.f, z = %.f",
//				tMC.at<float>(0), tMC.at<float>(1), tMC.at<float>(2),
//				tGR.at<float>(0), tGR.at<float>(1), tGR.at<float>(2));
//			Markers[i].draw(ImageF, cv::Scalar(0, 0, 255), 2);
//		}
//		// draw a 3d cube in each marker if there is 3d info
//		if (CamParam.isValid() && MarkerSize != -1)
//			for (unsigned int i = 0; i < Markers.size(); i++) {
//				aruco::CvDrawingUtils::draw3dAxis(ImageF, Markers[i], CamParam);
//			}
//			
//		//show input with augmented information
//		cv::imshow("in", ImageF);
//		
//		////Add image to video
//		//vw.write(ImageF);
//
//		
//		//Moving loop
//		x += 1;
//		pValue = PyObject_CallMethod(pInstance, "Move", "(iiii)", x, y, z, speed);
//		//pValue = PyObject_CallMethod(pInstance, "Move", "(iiii)", 230, 0, 238, 50);
//		i++;
//		printf("%d", i);
//		Sleep(1000);
//		
//
//	} while (char(cv::waitKey(1)) != 27); // wait for esc to be pressed
//*/
//
//#define test2
//#ifdef test2
//// Test 2 - visual servoing
//
//	//Transformation:
//	theta = atan2(0, 210);
//
//	RGR.at<float>(0, 0) = cos(theta);
//	RGR.at<float>(0, 1) = -sin(theta);
//	RGR.at<float>(0, 2) = 0;
//	RGR.at<float>(1, 0) = sin(theta);
//	RGR.at<float>(1, 1) = cos(theta);
//	RGR.at<float>(1, 2) = 0;
//	RGR.at<float>(2, 0) = 0;
//	RGR.at<float>(2, 1) = 0;
//	RGR.at<float>(2, 2) = 1;
//	RRC_ = RRC.t();
//	//end of Transformation
//	
//	int toolID, objectID;
//	int xr, yr, zr;	
//	float fi0, fi1, fi2, fi0r, fi1r, fi2r;
//	cv::Mat tMCr(3, 1, CV_32F), tMRr(3, 1, CV_32F);
//	cv::Mat tMR(3, 1, CV_32F);
//	float xMCr, yMCr, zMCr;
//
//	PyObject *pName2, *pModule2, *pDict2, *pClass2, *pInstance2;
//	PyObject *ptemp;
//
//	// Build the name object
//	pName2 = PyString_FromString("DobotKinematics");
//
//	// Load the module object
//	pModule2 = PyImport_Import(pName2);
//
//	// pDict is a borrowed reference 
//	pDict2 = PyModule_GetDict(pModule2);
//	pClass2 = PyDict_GetItemString(pDict2, "DobotKinematics");
//	pInstance2 = PyObject_CallObject(pClass2, NULL);
//
//	Sleep(1000);
//	astra_temp_update();
//	cv::flip(Image, ImageF, 1);
//	cv::imshow("in", ImageF);
//	Markers = MDetector.detect(ImageF, CamParam, MarkerSize);
//
//	
//	//Determine desired position
//	if (Markers.size() == 2)
//	{
//		objectID = Markers[0].id;
//		Markers[0].Tvec.copyTo(tMCr);
//		tMCr *= 1000;
//
//		//tMR = RRC_*(tMC - tRC) - RGR*tMG;
//		tMRr = RRC_*(tMCr - tRC) - RGR*tMG;
//
//		xMCr = tMCr.at<float>(0, 0);
//		yMCr = tMCr.at<float>(1, 0);
//		zMCr = tMCr.at<float>(2, 0);
//
//		xr = tMRr.at<float>(0, 0);
//		yr = tMRr.at<float>(1, 0);
//		zr = tMRr.at<float>(2, 0)+150;
//		printf("\nObject position: (%d, %d, %d), (%f, %f, %f)\n", xr, yr, zr, xMCr, yMCr, zMCr);
//
//		//Get Angles for Object Position
//		pValue = PyObject_CallMethod(pInstance2, "anglelist", "(iii)", xr, yr, zr);
//
//		Sleep(1000);
//
//		ptemp = PyList_GetItem(pValue, 0);
//		fi0r = (float)PyFloat_AsDouble(ptemp);
//
//		ptemp = PyList_GetItem(pValue, 1);
//		fi1r = (float)PyFloat_AsDouble(ptemp);
//
//		ptemp = PyList_GetItem(pValue, 2);
//		fi2r = (float)PyFloat_AsDouble(ptemp);
//
//		Markers[1].Tvec.copyTo(tMC);
//		tMC *= 1000;
//
//		tMR = RRC_*(tMC - tRC) - RGR*tMG;
//		x = tMR.at<float>(0, 0);
//		y = tMR.at<float>(1, 0);
//		z = tMR.at<float>(2, 0);
//	}
//
//	
//
//	int thresh=3;
//	float d, dq0, dq1, dq2;
//	float xMC, yMC, zMC;
//	do
//	{
//		Sleep(1000);
//	
//
//		//Get Angles for Tool Position
//		pValue = PyObject_CallMethod(pInstance2, "anglelist", "(iii)", x, y, z);
//		Sleep(1000);
//
//		ptemp = PyList_GetItem(pValue, 0);
//		fi0 = (float)PyFloat_AsDouble(ptemp);
//
//		ptemp = PyList_GetItem(pValue, 1);
//		fi1 = (float)PyFloat_AsDouble(ptemp);
//
//		ptemp = PyList_GetItem(pValue, 2);
//		fi2 = (float)PyFloat_AsDouble(ptemp);
//		dq0 = fi0r - fi0;
//		dq1 = fi1r - fi1;
//		dq2 = fi2r - fi2;
//		pValue = PyObject_CallMethod(pInstance, "MoveJoints", "(fff)", dq0, dq1, dq2);
//		Sleep(2000);
//
//		astra_temp_update();
//		cv::flip(Image, ImageF, 1);
//		cv::imshow("in", ImageF);
//		Markers = MDetector.detect(ImageF, CamParam, MarkerSize);
//	
//		if (Markers.size() > 1) //if object is visible
//			n = 1;
//		else n = 0;
//
//		Markers[n].Tvec.copyTo(tMC);
//		tMC *= 1000;
//
//		tMR = RRC_*(tMC - tRC) - RGR*tMG;
//		x = tMR.at<float>(0, 0);
//		y = tMR.at<float>(1, 0);
//		z = tMR.at<float>(2, 0);
//
//		xMC = tMC.at<float>(0, 0);
//		yMC = tMC.at<float>(1, 0);
//		zMC = tMC.at<float>(2, 0);
//
//		d = sqrt((x - xr)*(x - xr) + (y - yr)*(y - yr));
//		printf("\nMarker position: (%d, %d, %d), (%f, %f, %f)\n", x, y, z, xMC, yMC, zMC);
//		printf("\n%f", d);
//
//	} while (d > thresh);
//
//#endif
////end of Test 2
//
//	//Initial position
//	pValue = PyObject_CallMethod(pInstance, "InitialPosition", NULL);
//
//	//Communication closure
//	pValue = PyObject_CallMethod(pInstance, "Close", NULL);
//
//	// Clean up
//	Py_DECREF(pArgs);
//	Py_DECREF(pModule);
//	Py_DECREF(pName);
//
//
//	// Finish the Python Interpreter
//	Py_Finalize();
//	
//	//end of Python code
//
//	astra::terminate();
//	
//	////Release res.
//	//vw.release();
//

	rmm.Finish();
	return 0;
}

//#endif
