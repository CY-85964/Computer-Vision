// huesaturation.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <vector>

#include "opencv2/imgproc/types_c.h"

void detectHScolor(
	const cv::Mat& image,			// 输入图像 
	double minHue, double maxHue,	// 色调区间 
	double minSat, double maxSat,	// 饱和度区间
	cv::Mat& mask) {				// 输出掩码（二值图像）

	// 从RGB色彩空间转换成HSV色彩空间
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);

	// 将HSV色彩空间的三个通道分割进三个图像中
	std::vector<cv::Mat> channels;
	cv::split(hsv, channels);

	// channels[0] 是色调H
	// channels[1] 是饱和度S
	// channels[2] 是亮度V

	/// <筛选出色调 minHue < Hue < maxHue 的像素>
	
	// 筛选出色调 < minHue的像素
	cv::Mat mask1; 
	cv::threshold(channels[0], mask1, maxHue, 255, cv::THRESH_BINARY_INV);
	// 筛选出色调 > minHue的像素
	cv::Mat mask2; 
	cv::threshold(channels[0], mask2, minHue, 255, cv::THRESH_BINARY);
	// 合并色调掩码
	cv::Mat hueMask;
	if (minHue < maxHue)
		hueMask = mask1 & mask2;
	/*
		255 & 255 = 255
		255 &  0  = 0
		0   &  0  = 0
	*/
	else // 如果区间穿越0度中轴线
		hueMask = mask1 | mask2;
	/*
		255 | 255 = 255
		255 |  0  = 255
		0   |  0  = 0
	*/

	// 筛选出饱和度 < maxSat的像素
	cv::threshold(channels[1], mask1, maxSat, 255, cv::THRESH_BINARY_INV);
	// 筛选出饱和度 > maxSat的像素
	cv::threshold(channels[1], mask2, minSat, 255, cv::THRESH_BINARY);
	// 合并饱和度掩码
	cv::Mat satMask; // saturation mask
	satMask = mask1 & mask2;

	// 组合掩码（二值图像）
	mask = hueMask & satMask;
}

int main()
{
	// 读入原始图像（RGB色彩空间）
	cv::Mat image = cv::imread("boldt.jpg");
	if (!image.data)
		return 0;

	// 显示原始图像
	cv::namedWindow("Original image");
	cv::imshow("Original image", image);

#pragma region 用色调，饱和度和亮度表示颜色

	// 从RGB色彩空间转换成HSV色彩空间
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);

	// 把三个通道分割进3幅图像中
	std::vector<cv::Mat> channels;
	cv::split(hsv, channels);
	// channels[0] 是色调H
	// channels[1] 是饱和度S
	// channels[2] 是亮度V

	// 显示色调通道图
	cv::namedWindow("Hue");
	cv::imshow("Hue", channels[0]);

	// 显示饱和度通道图
	cv::namedWindow("Saturation");
	cv::imshow("Saturation", channels[1]);

	// 显示亮度通道图
	cv::namedWindow("Value");
	cv::imshow("Value", channels[2]);
	
	cv::waitKey();
	cv::destroyWindow("Hue");
	cv::destroyWindow("Saturation");
	cv::destroyWindow("Value");

#pragma endregion

#pragma region HSV颜色测试

	// 创建一个原图像的亮度通道的副本
	cv::Mat tmp(channels[2].clone()); // 保存源图像的亮度通道，因为后边要修改，但也原来的值也要用

	/// <固定亮度的图像>

	// 修改原图像亮度通道内所有像素的值为255
	channels[2] = 255;
	// 重新合并通道
	cv::merge(channels, hsv);
	// 转换回BGR色彩空间
	cv::Mat newImage;
	cv::cvtColor(hsv, newImage, CV_HSV2BGR);
	// 显示结果
	cv::namedWindow("Fixed Value Image");
	cv::imshow("Fixed Value Image", newImage);

	/// <固定饱和度的图像>

	// 还原亮度通道
	channels[2] = tmp;
	// 修改原图像饱和度通道内所有像素的值为255
	channels[1] = 255;
	// 重新合并通道
	cv::merge(channels, hsv);
	// 转换回BGR色彩空间
	cv::cvtColor(hsv, newImage, CV_HSV2BGR);
	// 显示结果
	cv::namedWindow("Fixed saturation");
	cv::imshow("Fixed saturation", newImage);

	/// <固定亮度和饱和度的图像>
	// 修改原图像饱和度通道内所有像素的值为255
	channels[1] = 255;
	// 修改原图像亮度通道内所有像素的值为255
	channels[2] = 255;
	// 重新合并通道
	cv::merge(channels, hsv);
	// 转换回BGR色彩空间
	cv::cvtColor(hsv, newImage, CV_HSV2BGR);
	// 显示结果
	cv::namedWindow("Fixed saturation/value");
	cv::imshow("Fixed saturation/value", newImage);

	cv::waitKey();
	cv::destroyAllWindows();

#pragma endregion

#pragma region 色调/饱和度组合

	// 人为生成一幅图像，用来说明各种色调/饱和度组合
	cv::Mat hs(128, 360, CV_8UC3);

	for (int h = 0; h < 360; h++)			// 列对应色调
	{
		for (int s = 0; s < 128; s++)		// 行对应饱和度
		{
			hs.at<cv::Vec3b>(s, h)[0] = h / 2;			// 色调通道：所有色调角度
			hs.at<cv::Vec3b>(s, h)[1] = 255 - s * 2;	// 饱和度通道：饱和度从高到低
			hs.at<cv::Vec3b>(s, h)[2] = 255;			// 亮度通道：常数
		}
	}

	// 从HSV色彩空间转换成RGB色彩空间
	cv::cvtColor(hs, newImage, CV_HSV2BGR);

	// 显示图像
	cv::namedWindow("Hue/Saturation");
	cv::imshow("Hue/Saturation", newImage);
	cv::waitKey();
	cv::destroyWindow("Hue/Saturation");

#pragma endregion

#pragma region 肤色检测

	// 读入原始图像
	image = cv::imread("girl.jpg");
	if (!image.data)
		return 0;

	// 显示原始图像
	cv::namedWindow("Original image");
	cv::imshow("Original image", image);

	/// <检测肤色>
	cv::Mat mask;

	// 调用我们自己定义的肤色检测函数
	detectHScolor(
		image,		// 输入图像
		160, 10,	// 色调为320°~20°
		25, 166,	// 饱和度为0.1~0.65
		mask);		// 输入的二值掩码图像

	/*
		8位图像：
		色调范围：0~180°，因此OpenCV会把角度除以2，以适合单字节的存储范围
		饱和度范围：0~255°
	*/

	/// <显示使用掩码后的图像>

	// 创建一个黑色3通道彩色图像
	cv::Mat detected(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	// mask是检测到的肤色掩码，这里只复制原始图像中掩码不为0的部分
	image.copyTo(detected, mask);

	// 显示结果
	cv::imshow("Detection result", detected);
	cv::waitKey();

#pragma endregion

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
