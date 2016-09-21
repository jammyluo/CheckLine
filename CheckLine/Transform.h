#pragma once
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>

#include <stdio.h>
#include <string>
#include <map>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <cv.h>
#include <cxcore.h>

#include "hough.h"

using namespace std;
using namespace cv;

class CTransform
{
public:
	CTransform();
	~CTransform();
	int doTransform(string file_path, int threshold);
	int FilerImageHsv(cv::Mat& img_ori, cv::Mat& outputImage);
	int FilerImageRgb(cv::Mat& img_ori, cv::Mat& outputImage);
	int checkLine(string img_path);
	void drawDetectLines(Mat& image, const vector<Vec4i>& lines, Scalar & color);
};

