// colorDetection.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp> 
#include <opencv2/highgui.hpp>

#include "colordetector.h"


int main()
{
#pragma region 用策略设计模式比较颜色

	/// <探测出图像中与目标颜色相近的颜色>

	// 1.创建图像处理器对象
	ColorDetector cdetect;  // 类是自己写的

	// 2.读取输入的图像
	cv::Mat image = cv::imread("boldt.jpg");
	if (image.empty()) 
		return 0;
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	// 3.设置输入参数
	cdetect.setTargetColor(230, 190, 130); // 这里颜色 (B = 230, G = 190, R = 130) 表示蓝天的颜色

	// 4.处理图像
	cv::Mat result = cdetect.process(image);  // 类中自己写的函数

	// 5.显示结果
	cv::namedWindow("result");
	cv::imshow("result", result);
	cv::waitKey();

#pragma endregion

#pragma region 使用CIE L*a*b色彩空间

	// 使用CIE L*a*b色彩空间来进行颜色的距离计算
	ColorDetector colordetector(
		230, 190, 130,	// 要探测的目标颜色
		45,				// 允许的最大差距
		true);			// 使用CIE L*a*b色彩空间

	//处理图像并显示二值结果
	result = colordetector(image);  // 调用仿函数

	cv::namedWindow("result (functor)");
	cv::imshow("result (functor)", result);
	cv::waitKey();

#pragma endregion

#pragma region floodFill函数

	/*  从起始点/种子点开始填充一个连接域。
		连通性由像素值的接近程度来衡量
	*/

	// 测试 floodFill 函数
	cv::floodFill(
		image,						// 输入/输出图像
		cv::Point(100, 50),         // 起始点/种子点（天空的位置，起始点的颜色为目标颜色）
		cv::Scalar(255, 255, 255),  // 填充颜色（白色，与目标颜色相近的颜色都被填充为白色）
		(cv::Rect*)0,				// 填充区域的边界矩形（这里设置为无边界）
		cv::Scalar(35, 35, 35),     // 当前选定像素与其连通区中相邻像素中的一个像素，或者与加入该连通区的一个种子像素，二者之间的最大下行差异值
		cv::Scalar(35, 35, 35),     // 当前选定像素与其连通区中相邻像素中的一个像素，或者与加入该连通区的一个种子像素，二者之间的最大上行差异值
		cv::FLOODFILL_FIXED_RANGE); // 固定范围模式，即所有像素都与起始点像素比较

	// 显示结果
	cv::namedWindow("Flood Fill result");
	cv::imshow("Flood Fill result", image);
	// 这里是把天空画成白色。即使其他地方有颜色接近的像素（例如水面），除非与天空相连，否则不会识别出来
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
