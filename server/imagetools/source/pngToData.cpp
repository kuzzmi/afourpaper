#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#define BLACK_WHITE_THRESHOLD 200
#define ROW_VALUE_THRESHOLD 50


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
	
	size = cropped.size();
		
/*
	for(int i=0; i<size.width; i++)
	{
		for(int u=0; u<size.height; u++)
		{
			
		}
	}*/


	Mat pattern = cvCreateImage(cvSize(3,3),8,3);	


	namedWindow("Display Window", WINDOW_AUTOSIZE);
	imshow("Display Window", image);
	
	namedWindow("Image2", WINDOW_AUTOSIZE);
	imshow("Image2", cropped);
	waitKey(0);
	return 0;
	


}
