// extractObject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


int main()
{
	/// <GrabCut分割算法>

	// 读入输入图像
	cv::Mat image = cv::imread("boldt.jpg");
	if (!image.data)
		return 0;

	// 显示图像
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	// 定义一个带边框的矩形，矩形外部的像素会被标记为背景
	cv::Rect rectangle(50, 25, 210, 180);

	// 分割结果（四种可能的值）
	cv::Mat result; // segmentation (4 possible values)

	// 模型（内部使用）
	cv::Mat bgModel, fgModel;
	

	// GrabCut 分割算法
	cv::grabCut(
		image,					// 输入图像
		result,					// 分割结果，具有四种可能的值
		rectangle,				// 包含前景的矩形
		bgModel, fgModel,		// 模型
		5,						// 迭代次数
		cv::GC_INIT_WITH_RECT); // 使用带边框的矩形模型

	/*  分割结果图像result，具有四种可能的值
		cv::GC_BGD:		该值表示明确属于背景的像素（本例中矩形之外的像素）
		cv::GC_FGD:		该值表示明确属于前景的像素（本例无）
		cv::GC_PR_BGD:	该值表示可能属于背景的像素
		cv::GC_PR_FGD:	该值表示可能属于前景的像素（本例中矩形之内像素的初始值）
	*/

	// 取得标记为“可能属于前景”的像素
	/*
		if result(x, y) == cv::GC_PR_FGD(可能属于前景的像素)
			result(x, y) == 255
		else
			result(x, y) == 0
	*/
	cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ); // result既是输入又是输出，第二个result成为二值图像
	
#pragma region Test

	cv::namedWindow("Test");
	cv::imshow("Test", result); // 掩码图像

#pragma endregion

	// 创建输出图像
	cv::Mat foreground(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));

	// 只有前景部分被复制（result掩码图像中像素值不为0的部分），不复制背景像素
	image.copyTo(foreground, result); // result相当于掩码（其中值为255的是前景，值为0的是背景）

	// 显示画了矩形的输入图像
	cv::rectangle(image, rectangle, cv::Scalar(255, 255, 255), 1);
	cv::namedWindow("Image with rectangle");
	cv::imshow("Image with rectangle", image);

	// 显示分割结果
	cv::namedWindow("Foreground object");
	cv::imshow("Foreground object", foreground);

	cv::waitKey();

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
