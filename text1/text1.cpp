// text1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <cv.h>  
#include <highgui.h>  
#include <opencv2/imgproc/imgproc.hpp>    
#include <opencv2/highgui/highgui.hpp>    
#include <iostream>    
#include <vector>    


using namespace cv;
using namespace std;

int Angle(double x1, double y1, double x2, double y2);  //以任意点为中心的投票域
int Angle(double x, double y);            //以原点为中心的投票域
double θ = 0;
double scale = 10;
double π = 3.1415;
double DF = 0;
int main()
{
	Mat SrcImage(50, 50, CV_8UC1, Scalar(255));
	Mat SrcImage1(500, 500, CV_8UC1, Scalar(255));

	int height = SrcImage.rows;
	int width = SrcImage.cols;
	for (int i = 0; i < height; i++)
	{
		uchar *p = SrcImage.ptr<uchar>(i);
		for (int j = 0; j < width; j++)
		{
			//Angle(j, i);      //遍历图片时，按列遍历，所以j对应横坐标

			Angle(25, 25, j, i);
			//Angle(5, 5, 5, 4);
			//Angle(5, 5, 5, 6);
			p[j] = p[j] * DF;
			//cout <<"("<<i <<j<< ") "<<endl;
			//Angle(i, j);

		}
	}
	namedWindow("线性投票域", 2);
	imshow("线性投票域", SrcImage);
	waitKey(0);
	return 0;
}
//计算显著性衰减函数(以原点为中心)
int Angle(double x, double y)    //x代表横坐标，y代表纵坐标
{
	double l = sqrt(x*x + y * y);        //两点之间的距离
	double l1 = 0;                         //y轴上的点与原点的距
	double a = 0;                          //sinθ的值
	double s = 0;                         //弧长
	double p = 0;     //曲率
	double c;//控制曲率的退化程度
	if (y == 0)
	{
		l1 = 0;
		a = 0;
		θ = 0;
	}
	else
	{
		l1 = y / 2 + x * x / y / 2;
		a = l / 2 / l1;
		θ = asin(a);
	}

	if (a == 0)
	{
		s = l;             //当θ为0时，弧长s与l相等
	}
	else
	{
		s = θ * l / a;    //弧长
	}

	if (l == 0)           //l为0，曲率为0
	{
		p = 0;
	}
	else
	{
		p = 2 * sin(θ) / l;     //曲率
	}
	c = -16 * log(0.1)*(scale - 1) / π / π;
	DF = exp(-(s*s + c * p*p) / scale / scale);
	cout << θ << " " << l << " " << s << " " << p << " " << c << " " << DF << endl;

	return DF;
}



//计算显著性衰减函数（以任意点为中心）
int Angle(double x1, double y1, double x2, double y2)
{
	double m = abs(x1 - x2);
	double n = abs(y1 - y2);
	double l = sqrt(m*m + n * n);          //两点之间的距离
	double l1 = 0;                         //y轴上的点与原点的距
	double a = 0;                          //存储sinθ的值
	double s = 0;                          //弧长
	double p = 0;     //曲率
	double c;//控制曲率的退化程度

	if (x1 == x2)     //两点x坐标相同 相当与在y轴上的点
	{
		l1 = abs(y2 - y1);
		a = 1;
		θ = π / 2;
	}
	else
	{
		if (y1 == y2)        //两点y坐标相同，相当于在x轴上的点
		{
			a = 0;
			θ = 0;
		}
		//图像坐标系与直角坐标系的差异
		else if (y2 < y1)   //当以投票点为中心，接受点在第一、第二象限时
		{
			l1 = (y1 - y2) / 2 + (x2 - x1) / (y1 - y2)*(x2 - x1) / 2;
			a = l / 2 / l1;
			θ = asin(a);
		}
		else if (y2 > y1)   //当以投票点为中心，接受点在第三、四象限时
		{
			l1 = (y2 - y1) / 2 + (x2 - x1) / (y2 - y1)*(x2 - x1) / 2;
			a = l / 2 / l1;
			θ = asin(a);
		}
	}
	if (a == 0)
	{
		s = l;             //当θ为0时，弧长s与l相等
	}
	else
	{
		s = θ * l / a;    //弧长
	}
	if (a == 0)
	{
		p = 0;
	}

	else
	{
		p = 2 * sin(θ) / l;     //曲率
	}
	c = -16 * log(0.1)*(scale - 1) / π / π;
	if (θ > π / 4)
	{
		DF = 0;
		//DF = exp(-(s*s + c * p*p) / scale / scale);
	}
	else
	{
		DF = exp(-(s*s + c * p*p) / scale / scale);
	}
	//cout <<l1<<" "<<a<<" "<< θ << " " << l << " " << s << " " << p << " " << DF << endl;
	return DF;
}