// remapping.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp> 
#include <opencv2/highgui.hpp>


void wave(const cv::Mat& image, cv::Mat& result)
{
	// 映射矩阵（浮点数型cv::Mat）
	cv::Mat srcX(image.rows, image.cols, CV_32F);
	cv::Mat srcY(image.rows, image.cols, CV_32F);
	
	// 创建映射参数
	for (int i = 0; i < image.rows; i++)	// 行循环
	{
		for (int j = 0; j < image.cols; j++)	// 列循环
		{
			/// <第(i, j)个像素的新位置>

			// 保持在同一列，原来在第j列的像素，现在仍在第j列
			srcX.at<float>(i, j) = j;	// 第j列

			// 原来在第i行的像素，现在根据一个正弦曲线移动
			srcY.at<float>(i, j) = i + 3 * sin(j / 6.0);	// 第i行加上第j列的一个正弦函数
		}
	}

	// 应用映射参数
	cv::remap(
		image,				// 输入图像
		result,				// 输出图像
		srcX,				// x方向的映射规则
		srcY,				// y方向的映射规则
		cv::INTER_LINEAR);	// 插值方法
}



int main()
{
    // 读入图像
	cv::Mat image = cv::imread("boldt.jpg", cv::IMREAD_GRAYSCALE);

	cv::namedWindow("Image");
	cv::imshow("Image", image);

	// 图像重映射
	cv::Mat result;
	wave(image, result);

	cv::namedWindow("Remapped image");
	cv::imshow("Remapped image", result);

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
