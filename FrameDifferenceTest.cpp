#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <pthread.h>

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
	int debug_mode = 1;
	int found_algo = 0;

	if(argc != 4 && argc != 5) {
		std::cout << "Invalid arguments: " <<argc <<" argv[0]:" <<argv[0] << std::endl;
		exit(1);
	}

	if(argc == 5 && strcmp(argv[4], "--debug")) {
		std::cout << "Debug mode: ON " << std::endl;
		debug_mode = 0;
	}

	int img_channels = 1;
	auto algorithmsName = BGS_Factory::Instance()->GetRegisteredAlgorithmsName();
	std::string algo;
	for (const std::string& algorithmName : algorithmsName) {
		if(algorithmName == argv[3]) {
			if(algorithmName == "IndependentMultimodal" || algorithmName == "T2FMRF_UM" || algorithmName == "LBSimpleGaussian" || algorithmName == "FuzzySugenoIntegral"
					|| algorithmName == "DPWrenGA"
					|| algorithmName == "LBAdaptiveSOM"
					|| algorithmName == "Amber"
					|| algorithmName == "MixtureOfGaussianV2"
					|| algorithmName == "LBFuzzyAdaptiveSOM"
					|| algorithmName == "LBMixtureOfGaussians"
					|| algorithmName == "DPEigenbackground"
					|| algorithmName == "LBFuzzyGaussian"
					|| algorithmName == "FuzzyChoquetIntegral"
					|| algorithmName == "VuMeter"
					|| algorithmName == "T2FMRF_UV"
					|| algorithmName == "MultiCue"
					|| algorithmName == "TwoPoints"
					|| algorithmName == "MultiLayer"
					|| algorithmName == "DPTexture"
					|| algorithmName == "ViBe"
					|| algorithmName == "PAWCS"
					|| algorithmName == "SuBSENSE"
					)
				img_channels = 3;
			algo = algorithmName;
			found_algo = 1;
			break;
		}
	}
	if(!found_algo) {
		std::cout << "Invalid arguments: " <<argv[3] <<" not found" << std::endl;
		exit(1);
	}
	std::cout << "Running " << algo << std::endl;
	auto bgs = BGS_Factory::Instance()->Create(algo);

	std::string input_path = argv[1];
	std::string output_path = argv[2];
	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(60);

	while(1) {
		std::stringstream ss;
		ss << myfillandw('0',6) <<frame_counter;
		std::string fileName = input_path + "/input/in" + ss.str() + ".jpg";
		//if(!debug_mode)
			//std::cout << "reading: " << fileName << std::endl;
		frame_counter++;

		cv::Mat img_input = cv::imread(fileName, img_channels == 1 ? CV_LOAD_IMAGE_GRAYSCALE : CV_LOAD_IMAGE_COLOR);

		if (img_input.empty()) {
			std::cout << "No hay imagen: " << fileName << std::endl;
			break;
		}

		if(!debug_mode)
			cv::imshow("input", img_input);

		cv::Mat img_mask;
		cv::Mat img_bkgmodel;

		bgs->process(img_input, img_mask, img_bkgmodel);

		if(!img_mask.empty()) {
			if(!debug_mode)
				cv::imshow("Foreground", img_mask);
			std::string outFileName = output_path + "/bin" + ss.str() + ".png";
			cv::imwrite(outFileName, img_mask);
		} else {
			std::cout << "No hay mascara: " << fileName << std::endl;
		}

		if(!debug_mode)
			cvWaitKey(0);
	}

	//XXX TODO FIXME Check how to free 
//	delete bgs;

	if(!debug_mode)
		cvDestroyAllWindows();

	return 0;
}
