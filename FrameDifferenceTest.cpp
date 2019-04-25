#include <iostream>
#include <cv.h>
#include <highgui.h>

#include "./package_bgs/FrameDifference.h"

using namespace bgslibrary::algorithms;

struct myfillandw
{
	myfillandw( char f, int w ) : fill(f), width(w) {}

	char fill;
	int width;
};

std::ostream& operator<<( std::ostream& o, const myfillandw& a )
{
	o.fill( a.fill );
	o.width( a.width );
	return o;
}

int main(int argc, char **argv)
{
	auto frame_counter = 1;

	IBGS *bgs;
	bgs = new FrameDifference;

	while(1) {
		std::stringstream ss;
		ss << myfillandw('0',6) <<frame_counter;
		std::string fileName = "/home/ubuntu/datasets/CD2014/dataset/baseline/highway/input/in" + ss.str() + ".jpg";
		std::cout << "reading: " << fileName << std::endl;
		frame_counter++;

		cv::Mat img_input = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);

		if (img_input.empty()) {
			std::cout << "No hay imagen: " << fileName << std::endl;
			break;
		}

//		cv::imshow("input", img_input);
//
//		cv::Mat img_mask;
//		cv::Mat img_bkgmodel;
//
//		// by default, it shows automatically the foreground mask image
//		bgs->process(img_input, img_mask, img_bkgmodel);
//
//		//if(!img_mask.empty())
//		//  cv::imshow("Foreground", img_mask);
//		//  do something
//
//		if(cvWaitKey(33) >= 0)
//			break;
	}

	delete bgs;

	cvDestroyAllWindows();

	return 0;
}
