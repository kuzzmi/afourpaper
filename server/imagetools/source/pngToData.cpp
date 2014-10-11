/**
   aForPaper
   pngToData.cpp
   Purpose: Provides functions to calculate pixel offset and crop image.


*/




#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include <stdio.h>
#include <cmath>


#define BLACK_WHITE_THRESHOLD 240
#define ROW_VALUE_THRESHOLD 50
#define INITIAL_LOWEST_ERROR_AMOUNT 10000
#define ERROR_TOLERANCE 1
#define OFFSET_CHECK_AMOUNT 10
#define PATTERN_PIXEL_DIM 4


using namespace cv;
using namespace std;





int main(int argc, char** argv)
{
	if(argc != 2)
	{
		cout<<"No image data"<<endl;
		return -1;
	}
	
	Mat image;
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	if(!image.data)
	{
		cout<<"Could not open or find image"<<endl;
		return -1;
	}
	


	//Crop the image
	Size size = image.size();

	int xOffset=0;
	int yOffset=0;
	int uOffset=size.width;
	int vOffset= size.height;		

	

	for(int i=0; i<size.height-1; i++)
	{	

		float rowValue=0.0f;
		int rowSamples=1;


		for(int u=0; u<size.width-1; u++)
		{
			Vec3b pixelValue = image.at<Vec3b>(i,u);
			uchar b = pixelValue.val[0];
			uchar g = pixelValue.val[1];
			uchar r = pixelValue.val[2];
			int p = 255-(r+g+b)/3.;
			if(p>BLACK_WHITE_THRESHOLD)
			{
				rowValue+=p;
				rowSamples++;
			}	
		}
		if(rowValue/rowSamples>ROW_VALUE_THRESHOLD)
		{
			cout<<"Position X: "<<i<<" Value: "<<rowValue/rowSamples<<endl;
			yOffset = i;
			break;
		}
	}


	for(int i=0; i<size.width-1; i++)
	{	

		float colValue=0.0f;
		int colSamples=1;

		for(int u=0; u<size.height-1; u++)
		{
			Vec3b pixelValue = image.at<Vec3b>(u,i);
			uchar b = pixelValue.val[0];
			uchar g = pixelValue.val[1];
			uchar r = pixelValue.val[2];
			int p = 255-(r+g+b)/3.;
			if(p>BLACK_WHITE_THRESHOLD)
			{
				colValue+=p;
				colSamples++;
			}	
		}
		if(colValue/colSamples>ROW_VALUE_THRESHOLD)
		{
			cout<<"Position Y: "<<i<<" Value: "<<colValue/colSamples<<endl;
			xOffset = i;
			break;
		}
	}




for(int i=size.width-1; i>0; i--)
	{	

		float colValue=0.0f;
		int colSamples=1;

		for(int u=size.height-1; u>0; u--)
		{
			Vec3b pixelValue = image.at<Vec3b>(u,i);
			uchar b = pixelValue.val[0];
			uchar g = pixelValue.val[1];
			uchar r = pixelValue.val[2];
			int p = 255-(r+g+b)/3.;
			if(p>BLACK_WHITE_THRESHOLD)
			{
				colValue+=p;
				colSamples++;
			}	
		}
		if(colValue/colSamples>ROW_VALUE_THRESHOLD)
		{
			cout<<"Position U: "<<i<<" Value: "<<colValue/colSamples<<endl;
			uOffset = i;
			break;
		}
	}


for(int i=size.height-1; i>0; i--)
	{	

		float colValue=0.0f;
		int colSamples=1;

		for(int u=size.width-1; u>0; u--)
		{
			Vec3b pixelValue = image.at<Vec3b>(i,u);
			uchar b = pixelValue.val[0];
			uchar g = pixelValue.val[1];
			uchar r = pixelValue.val[2];
			int p = 255-(r+g+b)/3.;
			if(p>BLACK_WHITE_THRESHOLD)
			{
				colValue+=p;
				colSamples++;
			}	
		}
		if(colValue/colSamples>ROW_VALUE_THRESHOLD)
		{
			cout<<"Position V: "<<i<<" Value: "<<colValue/colSamples<<endl;
			vOffset = i;
			break;
		}
	}


	//Do the actual cropping
	Rect myROI(xOffset, yOffset, uOffset-xOffset, vOffset-yOffset);
	Mat croppedRef(image, myROI);
		
	Mat cropped;
	croppedRef.copyTo(cropped);

	

	//Detect pixel size
/*	int Match = 0;
	int counter = 1;

	Mat result;

//while(!Match)
//{	
	Mat pattern = cvCreateImage(cvSize(3*counter,3*counter),8,3);
	size = pattern.size();
	for(int i=0; i<size.width; i++)
	{
		for(int u=0; u<size.height; u++)
		{
			if(i>=size.width/3.&&i<size.width*2./3.){
				if(u>=size.height/3.&&u<size.height*2./3.)
				{
					pattern.at<Vec3b>(Point(i,u)) = Vec3b(255,255,255);
				}
			}else{
				
				pattern.at<Vec3b>(Point(i,u))=Vec3b(0,0,0);
			}
		}
	}
*/	
	//Pattern matching	
	
	size = cropped.size();


	

	
	int lowestError=INITIAL_LOWEST_ERROR_AMOUNT;
	int lowestErrorOffset=0;
	float pixelDistances[OFFSET_CHECK_AMOUNT];


	for(int u=0;u<OFFSET_CHECK_AMOUNT;u++){

		int amount_of_distances=0;	
		int lastDistance=0;	
		int distance=0;
		bool inPattern=true;
		int lastVal = 255;	
		bool inWhite=false;
		bool lastInWhite=false;
	
		int error_count=0;
		

		for(int i=0; i<size.width; i++){
			Vec3b pixelData = cropped.at<Vec3b>(u,i);
			int val = (pixelData[0] + pixelData[1] + pixelData[2])/3;
			
			//cout<<"i: "<<i<<" val: "<<val<<endl;
			distance++;
	
			if((lastVal+val)/2>128)
			{
				inWhite=true;
				//cout<<"inwhite: "<<i<<endl;
			}	
			else{
				inWhite=false;
				//cout<<"inBlack: "<<i<<endl;
			}
	
			if(lastInWhite!=inWhite)
			{	
				int err = abs(lastDistance-distance);
				if(err>1)
					error_count+=err;		
				amount_of_distances++;
				lastDistance=distance;
				distance=0;
			}
				
			lastInWhite=inWhite;
			lastVal = val;

		}
	
		if(error_count<lowestError)
		{
			lowestError=error_count;
			lowestErrorOffset = u;
		}
		pixelDistances[u] = (float)size.width/(amount_of_distances+PATTERN_PIXEL_DIM);
		cout<<"Pixel distance: "<<pixelDistances[u]<<endl;
		//cout<<"Distance: "<<size.width/amount_of_distances<<endl;
		//cout<<"Error count: "<<error_count<<endl;
	}

	cout<<"Lowest error offset: "<<lowestErrorOffset<<endl;

	float  yPixelOffset = pixelDistances[lowestErrorOffset];	

	lowestError=INITIAL_LOWEST_ERROR_AMOUNT;
	lowestErrorOffset=0;



	for(int u=0;u<OFFSET_CHECK_AMOUNT;u++){

		int amount_of_distances=0;	
		int lastDistance=0;	
		int distance=0;
		bool inPattern=true;
		int lastVal = 255;	
		bool inWhite=false;
		bool lastInWhite=false;
	
		int error_count=0;
		

		for(int i=0; i<size.height; i++){
			Vec3b pixelData = cropped.at<Vec3b>(i,u);
			int val = (pixelData[0] + pixelData[1] + pixelData[2])/3;
			
			//cout<<"i: "<<i<<" val: "<<val<<endl;
			distance++;
	
			if((lastVal+val)/2>128)
			{
				inWhite=true;
				//cout<<"inwhite: "<<i<<endl;
			}	
			else{
				inWhite=false;
				//cout<<"inBlack: "<<i<<endl;
			}
	
			if(lastInWhite!=inWhite)
			{	
				int err = abs(lastDistance-distance);
				if(err>1)
					error_count+=err;		
				amount_of_distances++;
				lastDistance=distance;
				distance=0;
			}
				
			lastInWhite=inWhite;
			lastVal = val;

		}
	
		if(error_count<lowestError)
		{
			lowestError=error_count;
			lowestErrorOffset = u;
		}
		pixelDistances[u] = (float)size.height/(amount_of_distances+PATTERN_PIXEL_DIM);
		cout<<"Pixel distance: "<<pixelDistances[u]<<endl;

		//cout<<"Distance: "<<size.width/amount_of_distances<<endl;
		//icout<<"Error count: "<<error_count<<endl;
	}

	cout<<"Lowest error offset: "<<lowestErrorOffset<<endl;


	float  xPixelOffset=pixelDistances[lowestErrorOffset];

	

	cout<<"Horizontal pixels: "<<size.width/xPixelOffset<<endl;
	cout<<"Vertical pixels: "<<size.height/yPixelOffset<<endl;	


/*
	for(int i=0; i<cropped.width; i++)
	{
		for(int u=0; u<cropped.height; u++)
		{

		}
	}
	
/*







/*
	int result_cols = image.cols - pattern.cols+1;
	int result_rows = image.rows - pattern.rows+1;

	result.create(result_cols, result_rows, CV_32FC1);

	matchTemplate(image, pattern, result, 0);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	double minVal;
	double maxVal;
	Point minLoc;
	Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	matchLoc = maxLoc;

	cout<<"matchings:"<<endl;
	cout<<"x: "<<matchLoc.x<<endl;
	cout<<"y: "<<matchLoc.y<<endl;



	counter++;
//}


	imwrite("output.png", pattern);		
*/	

/*	namedWindow("Display Window", WINDOW_AUTOSIZE);
	imshow("Display Window", image);
	
	namedWindow("Image2", WINDOW_AUTOSIZE);
	imshow("Image2", cropped);
*/
	waitKey(0);
	return 0;
	


}
