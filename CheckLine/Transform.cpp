#include "stdafx.h"
#include "Transform.h"
#include <vector>

const char* CW_IMG_ORIGINAL = "Result";
const char* CW_IMG_EDGE = "Canny Edge Detection";
const char* CW_ACCUMULATOR = "Accumulator";
const char* CW_THRESHOLD = "��ֵ";

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
	cv::imshow("����", img_filer);

	// תΪ�Ҷ�ͼ  
	IplImage * g_pGrayImage = cvCreateImage(cvGetSize(&(IplImage)img_filer), IPL_DEPTH_8U, 1);
	cvCvtColor(&(IplImage)img_filer, g_pGrayImage, CV_BGR2GRAY);
	cv::Mat outGrayImage = cv::cvarrToMat(g_pGrayImage);
	cv::imshow("�Ҷ�", outGrayImage);

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
	opencv ��H��Χ��0~180����ɫ��H��Χ�����(0~8)��(160,180)
	S�Ǳ��Ͷȣ�һ���Ǵ���һ��ֵ,S���;��ǻ�ɫ���ο�ֵS>80)��
	V�����ȣ����;��Ǻ�ɫ�����߾��ǰ�ɫ(�ο�ֵ220>V>50)��
	*/
	int width = image->width;
	int height = image->height;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			CvScalar s_hsv = cvGet2D(image, i, j);//��ȡ���ص�Ϊ��j, i�����HSV��ֵ   
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
	opencv ��H��Χ��0~180����ɫ��H��Χ�����(0~8)��(160,180)
	S�Ǳ��Ͷȣ�һ���Ǵ���һ��ֵ,S���;��ǻ�ɫ���ο�ֵS>80)��
	V�����ȣ����;��Ǻ�ɫ�����߾��ǰ�ɫ(�ο�ֵ220>V>50)��
	*/
	int width = hsv->width;
	int height = hsv->height;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			CvScalar s_hsv = cvGet2D(hsv, i, j);//��ȡ���ص�Ϊ��j, i�����HSV��ֵ   

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
	// ����⵽��ֱ����ͼ�ϻ�����
	vector<Vec4i>::const_iterator it = lines.begin();
	while (it != lines.end())
	{
		Point pt1((*it)[0], (*it)[1]);
		Point pt2((*it)[2], (*it)[3]);
		line(image, pt1, pt2, color, 2); //  �����������Ϊ2
		++it;
	}
	//������
	Point xAxis_start(0, image.rows / 2);
	Point xAxis_end(image.cols, image.rows / 2);
	line(image, xAxis_start, xAxis_end, color, 1); //  �����������Ϊ1

	Point yAxis_start(image.cols / 2, 0);
	Point yAxis_end(image.cols / 2, image.rows);
	line(image, yAxis_start, yAxis_end, color, 1); //  �����������Ϊ1

}

int CTransform::checkLine(string img_path)
{
	Mat image = imread(img_path);
	Mat I;

	cvSubS(&(IplImage)image, cvScalar(100, 100, 100), &(IplImage)image);

	cvtColor(image, I, CV_BGR2GRAY);
	imshow("�Ҷ�", I);

	Mat contours;
	Canny(I, contours, 125, 400);
	threshold(contours, contours, 128, 255, THRESH_BINARY);

	imshow("��Ե", contours);
	vector <Vec4i> lines;
	// ���ֱ�ߣ���СͶƱΪ90������������50����϶��С��10
	HoughLinesP(contours, lines, 1, CV_PI / 180, 50, 30, 1);
	drawDetectLines(image, lines, Scalar(0, 255, 0));

	imshow("ֱ�߼��", image);
	return 0;
}
