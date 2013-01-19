#include "stdafx.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <ctime>

using namespace std;

clock_t start;	
	
IplImage* imgTracking;	
int lastX = -1;	
int lastY = -1;	

// mengubah gambar menjadi threshold
IplImage* GetGambarThreshold(IplImage* gbrHSV){
	IplImage* gbrThresh=cvCreateImage(cvGetSize(gbrHSV),IPL_DEPTH_8U,1);
	cvInRangeS(gbrHSV,cvScalar(170,160,60),cvScalar(180,256,256),gbrThresh); //scalar untuk menunjukan nilai atas dan nilai bawah
	return gbrThresh;
}

void trackObject(IplImage* gbrThresh){	
	// calculate the moments of 'gbrThresh'

	CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));	
	cvMoments(gbrThresh, moments, 1);	
	double moment10 = cvGetSpatialMoment(moments,1,0);	
	double moment01 = cvGetSpatialMoment(moments,0,1);	
	double area = cvGetCentralMoment(moments,0,0);	

	// if the area <1000, i consider that the there are no object in the image and it's because of the noise, the area is not zero
	if(area>1000){	
		// calculate the position of the ball
		int posX = moment10/area;	
		int posY = moment01/area;	

		if(lastX>=0 && lastY>=0 && posX>=0 && posY>=0){	
			// draw a red line from the previous point to the current point
			cvLine(imgTracking,cvPoint(posX,posY),cvPoint(lastX,lastY),cvScalar(0,0,255),4);	
		}	

		lastX = posX;	
		lastY = posY;	
	}	

	free(moments);	
}

int main(){
	// CvCapture* gambar = cvCaptureFromFile("3d-09-5balls.jpg");
	
	CvCapture* kamera=0;
	kamera = cvCaptureFromCAM(0);
	if(!kamera){
		printf("kamera gagal");
		return -1;
	}

	IplImage* frame=0;
	
	int i=1;
	
	frame = cvQueryFrame(kamera);	
	if(!frame) return -1;	
	// create a blank image and assigned to 'imgTracking' which has the same size of original video 
	imgTracking = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,3);	
	cvZero(imgTracking); // covert the image, 'imgTracking' to black

	cvNamedWindow("input", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("output", CV_WINDOW_AUTOSIZE);

	while(1){
		
		start = clock();

		frame = cvQueryFrame(kamera);
		if(!frame) 
			break;

		frame = cvCloneImage(frame);
		cvSmooth(frame, frame, CV_GAUSSIAN,3,3);

		IplImage* gbrHSV = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,3);
		cvCvtColor(frame,gbrHSV,CV_BGR2HSV); 
		IplImage* gbrThresh = GetGambarThreshold(gbrHSV);

		cvSmooth(gbrThresh,gbrThresh,CV_GAUSSIAN,3,3);
		
		// track the position of the ball 
		trackObject(gbrThresh);	

		// add the tracking image and the frame
		cvAdd(frame,imgTracking,frame);	

		cvShowImage("output",gbrThresh);
		cvShowImage("input",frame);

		cvReleaseImage(&gbrHSV);
		cvReleaseImage(&gbrThresh);
		cvReleaseImage(&frame);
		
		cout << i++ << " " << ((clock()-start) / 16)<< endl;
		
		int c = cvWaitKey(10);

		if((char)c==27)
			break;
	}

	cvDestroyAllWindows();	
	cvReleaseCapture(&kamera);
	cvReleaseImage(&imgTracking);	

	return 0;
}


