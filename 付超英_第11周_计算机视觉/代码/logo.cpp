// logo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp> 
#include <opencv2/highgui.hpp>


int main()
{
#pragma region 定义感兴趣的区域(Region of interest, ROI)

	/*
	要把一个小图像复制到一个大图像上，例如把一个logo插入到测试图像中
	*/

	// 读入图像
	cv::Mat image = cv::imread("1.JPG");
	// 读入logo图像
	cv::Mat logo = cv::imread("logo.jpg");

	// 在图像的右下角定义一个ROI（初始化），放置logo的位置
	cv::Mat imageROI(
		image,														// 读入的图像
		cv::Rect(image.cols - logo.cols, image.rows - logo.rows, 	// ROI左上角坐标
			logo.cols, logo.rows));									// ROI大小（宽度和高度）
	/* ROI 实际上就是一个cv::Mat对象，它与它的父图像指向同一个数据缓冲区，并在头部指明ROI的坐标 */

	// 插入logo，将logo这幅图像拷贝到image上定义的一个ROI（imageROI）中去
	logo.copyTo(imageROI);
	cv::namedWindow("Image"); // 定义窗口
	cv::imshow("Image", image);
	cv::waitKey(0);

#pragma region 使用图像掩码

	// 重新读入原始图像
	image = cv::imread("1.JPG");

	// 定义ROI1
	// 初始化的时候直接赋值
	//cv::Mat imageROI(
	//	image,														// 读入的图像
	//	cv::Rect(image.cols - logo.cols, image.rows - logo.rows, 	// ROI左上角坐标
	//		logo.cols, logo.rows));									// ROI大小（宽度和高度）

	// 定义ROI2
	// 在图像的右下角定义一个ROI（赋值）
	imageROI = image(
		cv::Rect(image.cols - logo.cols, image.rows - logo.rows, 	// ROI左上角坐标
				 logo.cols, logo.rows));							// ROI大小（宽度和高度）

	// 定义ROI3
	// ROI 还可以用行和列的值域来描述
	// imageROI = image(cv::Range(image.rows - logo.rows, image.rows)    // ROI 行（高度）范围
	// 				    cv::Range(image.cols - logo.cols, image.cols));  // ROI 列（宽度）范围

	// 把标志作为掩码（必须是灰度图像）
	cv::Mat mask(logo); // 浅拷贝，mask和logo指向同一幅图像

	// 插入标志，只复制掩码mask中不为0的位置
	logo.copyTo(imageROI, mask);
	// logo/mask是同一幅图像，都是二值图像，即像素不是0就是255，只由黑白构成
	// 这里相当于只复制logo中值为255的像素

	cv::imshow("Image", image);
	cv::waitKey(0);

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
