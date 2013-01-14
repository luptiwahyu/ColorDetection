#include "stdafx.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>

IplImage* GetGambarThreshold(IplImage* gbrHSV){
	IplImage* gbrThresh=cvCreateImage(cvGetSize(gbrHSV),IPL_DEPTH_8U,1);
	cvInRangeS(gbrHSV,cvScalar(170,160,60),cvScalar(180,256,256),gbrThresh);
	return gbrThresh;
}

int main(){
	CvCapture* gambar = cvCaptureFromFile("3d-09-5balls.jpg");
	
	IplImage* frame=0;

	cvNamedWindow("input", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("output", CV_WINDOW_AUTOSIZE);

	while(true){

		frame = cvQueryFrame(gambar);
		if(!frame) 
			break;

		frame = cvCloneImage(frame);
		cvSmooth(frame, frame, CV_GAUSSIAN,3,3);

		IplImage* gbrHSV = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,3);
		cvCvtColor(frame,gbrHSV,CV_BGR2HSV); 
		IplImage* gbrThresh = GetGambarThreshold(gbrHSV);
//iki
		cvSmooth(gbrThresh,gbrThresh,CV_GAUSSIAN,3,3);

		cvShowImage("output",gbrThresh);
		cvShowImage("input",frame);

		cvReleaseImage(&gbrHSV);
		cvReleaseImage(&gbrThresh);
		cvReleaseImage(&frame);
		
		cvWaitKey();
	}

	cvDestroyAllWindows();	
	cvReleaseCapture(&gambar);

	return 0;
}


