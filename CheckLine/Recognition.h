#pragma once

#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <cv.h>
#include <cxcore.h>
using namespace std;
using namespace cv;

class Recognition
{
public:
	Recognition();
	~Recognition();
	void RecognitionSquares(string img_path);
	void RecognitionSquares(Mat& matImage);

private:
	double angle(CvPoint* pt1, CvPoint* pt2, CvPoint* pt0);
	CvSeq* findSquares4(IplImage* img, CvMemStorage* storage, CvSeq* squares);
	void drawSquares(IplImage* dest_img, CvSeq* squares);
	void drawTexts(IplImage* dest_img, string arrayText[]);

private:
	int thresh;
	const char * wndname;
};

