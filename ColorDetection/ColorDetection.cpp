#include "stdafx.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>


IplImage* GetGambarThreshold(IplImage* gbrHSV){
	IplImage* gbrThresh=cvCreateImage(cvGetSize(gbrHSV),IPL_DEPTH_8U,1);
	cvInRangeS(gbrHSV,cvScalar(170,160,60),cvScalar(180,256,256),gbrThresh);
	return gbrThresh;
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

	cvNamedWindow("input", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("output", CV_WINDOW_AUTOSIZE);

	while(true){

		frame = cvQueryFrame(kamera);
		if(!frame) 
			break;

		frame = cvCloneImage(frame);
		cvSmooth(frame, frame, CV_GAUSSIAN,3,3);

		IplImage* gbrHSV = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,3);
		cvCvtColor(frame,gbrHSV,CV_BGR2HSV); 
		IplImage* gbrThresh = GetGambarThreshold(gbrHSV);

		cvSmooth(gbrThresh,gbrThresh,CV_GAUSSIAN,3,3);

		cvShowImage("output",gbrThresh);
		cvShowImage("input",frame);

		cvReleaseImage(&gbrHSV);
		cvReleaseImage(&gbrThresh);
		cvReleaseImage(&frame);
		
		int c = cvWaitKey(10);

		if((char)c==27)
			break;
	}

	cvDestroyAllWindows();	
	cvReleaseCapture(&kamera);

	return 0;
}


