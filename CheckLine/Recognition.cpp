#include "stdafx.h"
#include "Recognition.h"


Recognition::Recognition()
{
	thresh = 50;
	wndname = "正方形检测 demo";
}


Recognition::~Recognition()
{
}

double Recognition::angle(CvPoint* pt1, CvPoint* pt2, CvPoint* pt0)
{
	double dx1 = pt1->x - pt0->x;
	double dy1 = pt1->y - pt0->y;
	double dx2 = pt2->x - pt0->x;
	double dy2 = pt2->y - pt0->y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

// 返回图像中找到的所有轮廓序列，并且序列存储在内存存储器中
CvSeq* Recognition::findSquares4(IplImage* img, CvMemStorage* storage, CvSeq* squares)
{
	CvSeq* contours;
	int i, c, l, N = 11;
	CvSize sz = cvSize(img->width & -2, img->height & -2);

	IplImage* timg = cvCloneImage(img);
	IplImage* gray = cvCreateImage(sz, 8, 1);
	IplImage* pyr = cvCreateImage(cvSize(sz.width / 2, sz.height / 2), 8, 3);
	IplImage* tgray;
	CvSeq* result;
	double s, t;

	cvSetImageROI(timg, cvRect(0, 0, sz.width, sz.height));
	// 过滤噪音
	cvPyrDown(timg, pyr, 7);
	cvPyrUp(pyr, timg, 7);
	tgray = cvCreateImage(sz, 8, 1);

	// 红绿蓝3色分别尝试提取
	for (c = 2; c < 3; c++)
	{
		// 提取 the c-th color plane
		cvSetImageCOI(timg, c + 1);
		cvCopy(timg, tgray, 0);

		// 尝试各种阈值提取得到的（N=11）
		for (l = 0; l < N; l++)
		{
			// apply Canny. Take the upper threshold from slider
			// Canny helps to catch squares with gradient shading  
			if (l == 0)
			{
				cvCanny(tgray, gray, 0, thresh, 5);
				//使用任意结构元素膨胀图像
				cvDilate(gray, gray, 0, 1);
			}
			else
			{
				// apply threshold if l!=0:
				cvThreshold(tgray, gray, (l + 1) * 255 / N, 255, CV_THRESH_BINARY);
			}

			// 找到所有轮廓并且存储在序列中
			cvFindContours(gray, storage, &contours, sizeof(CvContour),
				CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

			// 遍历找到的每个轮廓contours
			while (contours)
			{
				//用指定精度逼近多边形曲线
				result = cvApproxPoly(contours, sizeof(CvContour), storage,
					CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0);


				if (result->total == 4 &&
					fabs(cvContourArea(result, CV_WHOLE_SEQ)) > 500 &&
					fabs(cvContourArea(result, CV_WHOLE_SEQ)) < 100000 &&
					cvCheckContourConvexity(result))
				{
					s = 0;

					for (i = 0; i < 5; i++)
					{
						// find minimum angle between joint edges (maximum of cosine)
						if (i >= 2)
						{
							t = fabs(angle(
								(CvPoint*)cvGetSeqElem(result, i),
								(CvPoint*)cvGetSeqElem(result, i - 2),
								(CvPoint*)cvGetSeqElem(result, i - 1)));
							s = s > t ? s : t;
						}
					}

					// if 余弦值 足够小，可以认定角度为90度直角
					//cos0.05=87度，能较好的趋近直角
					if (s < 0.1)
						for (i = 0; i < 4; i++)
							cvSeqPush(squares, (CvPoint*)cvGetSeqElem(result, i));
				}
				// 继续查找下一个轮廓
				contours = contours->h_next;
			}
		}
	}
	cvReleaseImage(&gray);
	cvReleaseImage(&pyr);
	cvReleaseImage(&tgray);
	cvReleaseImage(&timg);

	return squares;
}

//drawSquares函数用来画出在图像中找到的所有正方形轮廓
void Recognition::drawSquares(IplImage* dest_img, CvSeq* squares)
{
	CvSeqReader reader;
	int i;
	cvStartReadSeq(squares, &reader, 0);
	//画坐标轴
	Point xAxis_start(0, dest_img->height / 2);
	Point xAxis_end(dest_img->width, dest_img->height / 2);
	cvLine(dest_img, xAxis_start, xAxis_end, CV_RGB(255, 0, 0), 1); //  线条宽度设置为1

	Point yAxis_start(dest_img->width / 2, 0);
	Point yAxis_end(dest_img->width / 2, dest_img->height);
	cvLine(dest_img, yAxis_start, yAxis_end, CV_RGB(255, 0, 0), 1); //  线条宽度设置为1

	// read 4 sequence elements at a time (all vertices of a square)
	for (i = 0; i < squares->total; i += 4)
	{
		CvPoint pt[4], *rect = pt;
		int count = 4;

		// read 4 vertices
		CV_READ_SEQ_ELEM(pt[0], reader);
		CV_READ_SEQ_ELEM(pt[1], reader);
		CV_READ_SEQ_ELEM(pt[2], reader);
		CV_READ_SEQ_ELEM(pt[3], reader);

		//过滤无效矩形宽度大于整张图片一半的 TODO, 
		if (pt[3].x - pt[1].x > dest_img->width / 2)
		{
			continue;
		}
		// draw the square as a closed polyline
		cvPolyLine(dest_img, &rect, &count, 1, 1, CV_RGB(0, 0, 255), 1, CV_AA, 0);

		//画中线
		int x_pos = int((pt[3].x - pt[1].x) / 2.0 + 0.5 + pt[1].x);
		Point yAxis_start(x_pos , 0);
		Point yAxis_end(x_pos, dest_img->height);

		cvLine(dest_img, yAxis_start, yAxis_end, CV_RGB(0, 255, 0), 1); //  线条宽度设置为1
		break;
	}
}

//drawSquares函数用来画出在图像中找到的所有正方形轮廓
void Recognition::drawTexts(IplImage* dest_img, string arrayText[])
{
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_TRIPLEX, 0.35f, 0.7f, 0, 1, CV_AA);

	for (size_t i = 0; i < sizeof(arrayText); i++)
	{
		cvPutText(dest_img, arrayText[i].c_str(), cvPoint(20, 20+i*20), &font, CV_RGB(255, 255, 0));
	}

}

void Recognition::RecognitionSquares(Mat& matImage)
{
	IplImage* pImg = &IplImage(matImage);
	if (!pImg)
	{
		return;
	}

	CvMemStorage* storage = cvCreateMemStorage(0);
	// 创建一个空序列用于存储轮廓角点
	CvSeq* squares = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint), storage);
	findSquares4(pImg, storage, squares);
	drawSquares(pImg, squares);
	cvClearSeq(squares);
	cvClearMemStorage(storage);
}

void Recognition::RecognitionSquares(string img_path)
{
	IplImage* src_img = cvLoadImage(img_path.c_str(), 1);
	if (!src_img)
	{
		return ;
	}
	IplImage* dst_img = cvCloneImage(src_img);

	CvMemStorage* storage = cvCreateMemStorage(0);
	// 创建一个空序列用于存储轮廓角点
	CvSeq* squares = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint), storage);
	findSquares4(src_img, storage, squares);
	drawSquares(dst_img, squares);
	//string arrayText[] = { "标志信息：", "经度:", "纬度:", "位移：" };
	string arrayText[] = { "flag:", "Latitude:", "Longitude:", "offset:" };
	drawTexts(dst_img, arrayText);
	cvClearSeq(squares);
	cvClearMemStorage(storage);

	cvShowImage(wndname, dst_img);
	cvSaveImage("c:\\test.jpg", dst_img);
	cvReleaseImage(&dst_img);

}
