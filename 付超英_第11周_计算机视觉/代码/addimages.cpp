angjia// addimages.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp> 
#include <opencv2/highgui.hpp>


int main()
{
	cv::Mat image1;
	cv::Mat image2;

	// 打开两幅图像
	image1 = cv::imread("boldt.jpg");
	image2 = cv::imread("rain.jpg");
	if (!image1.data || !image2.data)
		return 0;

	// 显示两幅输入图像
	cv::namedWindow("Image 1");
	cv::imshow("Image 1", image1);
	cv::namedWindow("Image 2");
	cv::imshow("Image 2", image2);

#pragma region 图像相加

	cv::Mat result;

	// 两幅图像相加（使用函数）
	cv::addWeighted(image1, 0.7, image2, 0.9, 0., result); // result = 0.7 * image1 + 0.9 * image2 + 0.
	// 类似的还有 cv::add(), cv::subtract, cv::multiply(), cv::divide()

	cv::namedWindow("result");
	cv::imshow("result", result);
	cv::waitKey();

	// 两幅图像相加（使用重载运算符）
	result = 0.7 * image1 + 0.9 * image2;

	cv::namedWindow("result with operators");
	cv::imshow("result with operators", result);
	cv::waitKey();
	cv::destroyAllWindows();

#pragma endregion

#pragma region 分割图像通道

	image2 = cv::imread("rain.jpg", cv::IMREAD_GRAYSCALE);

	// 创建三幅图像的向量
	std::vector<cv::Mat> planes; // 每幅图像对应一个通道

	// 将一个三通道的图像分割为三个单通道图像
	cv::split(image1, planes);

	// 将下雨图像（灰度，单通道）加到boldt图像的蓝色通道上去
	planes[0] += image2;

	// 将三个通道图像合并为一个三通道图像
	cv::merge(planes, result);

	cv::namedWindow("Result on blue channel");
	cv::imshow("Result on blue channel", result);

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
		