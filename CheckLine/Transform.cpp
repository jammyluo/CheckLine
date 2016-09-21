#include "stdafx.h"
#include "Transform.h"
#include <vector>

const char* CW_IMG_ORIGINAL = "Result";
const char* CW_IMG_EDGE = "Canny Edge Detection";
const char* CW_ACCUMULATOR = "Accumulator";
const char* CW_THRESHOLD = "二值";

CTransform::CTransform()
{
}


CTransform::~CTransform()
{
}


int CTransform::doTransform(string file_path, int threshold)
{

	cv::Mat img_ori = cv::imread(file_path, 1);


	cv::Mat img_filer;
	FilerImageRgb(img_ori, img_filer);
	cv::imshow("过滤", img_filer);

	// 转为灰度图  
	IplImage * g_pGrayImage = cvCreateImage(cvGetSize(&(IplImage)img_filer), IPL_DEPTH_8U, 1);
	cvCvtColor(&(IplImage)img_filer, g_pGrayImage, CV_BGR2GRAY);
	cv::Mat outGrayImage = cv::cvarrToMat(g_pGrayImage);
	cv::imshow("灰度", outGrayImage);

	cv::Mat img_threshold;
	cv::threshold(outGrayImage, img_threshold, 100, 255, CV_THRESH_BINARY);
	cv::imshow(CW_THRESHOLD, img_threshold);
	//Transform

	cv::Mat img_blur;
	cv::Mat img_edge;
	cv::blur(img_threshold, img_blur, cv::Size(5, 5));
	cv::Canny(img_blur, img_edge, 100, 150, 3);

	int w = img_edge.cols;
	int h = img_edge.rows;

	keymolen::Hough hough;
	hough.Transform(img_edge.data, w, h);

	if (threshold == 0)
		threshold = w > h ? w / 4 : h / 4;


	cv::Mat img_res = img_ori.clone();

	//Search the accumulator
	std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > > lines = hough.GetLines(threshold);

	//Draw the results
	std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > >::iterator it;
	for (it = lines.begin(); it != lines.end(); it++)
	{
		cv::line(img_res, cv::Point(it->first.first, it->first.second), cv::Point(it->second.first, it->second.second), cv::Scalar(0, 0, 255), 2, 8);
	}

	//Visualize all
	int aw, ah, maxa;
	aw = ah = maxa = 0;
	const unsigned int* accu = hough.GetAccu(&aw, &ah);

	for (int p = 0; p < (ah*aw); p++)
	{
		if ((int)accu[p] > maxa)
			maxa = accu[p];
	}
	double contrast = 1.0;
	double coef = 255.0 / (double)maxa * contrast;

	cv::Mat img_accu(ah, aw, CV_8UC3);
	for (int p = 0; p < (ah*aw); p++)
	{
		unsigned char c = (double)accu[p] * coef < 255.0 ? (double)accu[p] * coef : 255.0;
		img_accu.data[(p * 3) + 0] = 255;
		img_accu.data[(p * 3) + 1] = 255 - c;
		img_accu.data[(p * 3) + 2] = 255 - c;
	}


	cv::imshow(CW_IMG_ORIGINAL, img_res);
	cv::imshow(CW_IMG_EDGE, img_edge);
	cv::imshow(CW_ACCUMULATOR, img_accu);


	return 0;
}


int CTransform::FilerImageRgb(cv::Mat& img_ori, cv::Mat& outputImage)
{
	IplImage* image = &(IplImage)img_ori;

	/*
	opencv 的H范围是0~180，红色的H范围大概是(0~8)∪(160,180)
	S是饱和度，一般是大于一个值,S过低就是灰色（参考值S>80)，
	V是亮度，过低就是黑色，过高就是白色(参考值220>V>50)。
	*/
	int width = image->width;
	int height = image->height;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			CvScalar s_hsv = cvGet2D(image, i, j);//获取像素点为（j, i）点的HSV的值   
			//int sum = (s_hsv.val[0] + s_hsv.val[1] + s_hsv.val[2]) / 3;
			if (s_hsv.val[2] > 170 && s_hsv.val[0]<150 && s_hsv.val[1]<150)
			{
				s_hsv.val[0] = s_hsv.val[1] = s_hsv.val[2] = 255;
			}
			else
			{
				s_hsv.val[0] = s_hsv.val[1] = s_hsv.val[2] = 0;
			}
			cvSet2D(image, i, j, s_hsv);
		}
	}
	outputImage = cv::cvarrToMat(image);
	return 0;
}

int CTransform::FilerImageHsv(cv::Mat& img_ori, cv::Mat& outputImage)
{
	IplImage* image = &(IplImage)img_ori;

	IplImage* hsv = cvCreateImage(cvGetSize(image), 8, 3);

	cvCvtColor(image, hsv, CV_BGR2HSV);

	/*
	opencv 的H范围是0~180，红色的H范围大概是(0~8)∪(160,180)
	S是饱和度，一般是大于一个值,S过低就是灰色（参考值S>80)，
	V是亮度，过低就是黑色，过高就是白色(参考值220>V>50)。
	*/
	int width = hsv->width;
	int height = hsv->height;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			CvScalar s_hsv = cvGet2D(hsv, i, j);//获取像素点为（j, i）点的HSV的值   

			if ((s_hsv.val[0]>0) && (s_hsv.val[0] < 8)
				&& (s_hsv.val[1]>67) && (s_hsv.val[1] < 255))
			{
				continue;
			}

			if ((s_hsv.val[0]>150) && (s_hsv.val[0] < 255)
				&& (s_hsv.val[1]>67) && (s_hsv.val[1] < 255))
			{
				continue;
			}

			CvScalar s;
			s.val[0] = 0;
			s.val[1] = 0;
			s.val[2] = 0;
			cvSet2D(hsv, i, j, s);
		}
	}
	outputImage = cv::cvarrToMat(hsv);
	return 0;
}


void CTransform::drawDetectLines(Mat& image, const vector<Vec4i>& lines, Scalar & color)
{
	// 将检测到的直线在图上画出来
	vector<Vec4i>::const_iterator it = lines.begin();
	while (it != lines.end())
	{
		Point pt1((*it)[0], (*it)[1]);
		Point pt2((*it)[2], (*it)[3]);
		line(image, pt1, pt2, color, 2); //  线条宽度设置为2
		++it;
	}
	//画坐标
	Point xAxis_start(0, image.rows / 2);
	Point xAxis_end(image.cols, image.rows / 2);
	line(image, xAxis_start, xAxis_end, color, 1); //  线条宽度设置为1

	Point yAxis_start(image.cols / 2, 0);
	Point yAxis_end(image.cols / 2, image.rows);
	line(image, yAxis_start, yAxis_end, color, 1); //  线条宽度设置为1

}

int CTransform::checkLine(string img_path)
{
	Mat image = imread(img_path);
	Mat I;

	cvSubS(&(IplImage)image, cvScalar(100, 100, 100), &(IplImage)image);

	cvtColor(image, I, CV_BGR2GRAY);
	imshow("灰度", I);

	Mat contours;
	Canny(I, contours, 125, 400);
	threshold(contours, contours, 128, 255, THRESH_BINARY);

	imshow("边缘", contours);
	vector <Vec4i> lines;
	// 检测直线，最小投票为90，线条不短于50，间隙不小于10
	HoughLinesP(contours, lines, 1, CV_PI / 180, 50, 30, 1);
	drawDetectLines(image, lines, Scalar(0, 255, 0));

	imshow("直线检测", image);
	return 0;
}
