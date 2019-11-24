// contrast.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp> 
#include <opencv2/highgui.hpp>


void sharpen(const cv::Mat& image, cv::Mat& result)
{
	// 这里不适合调用sharpen(image, image);
	// 因为下面计算锐化时，其相邻元素用的是原始图像像素的值
	// 如果在原始图像上修改，计算时会用到修改后的值，不符合预期
	result.create(image.size(), image.type());

	// 获得通道数
	int nchannels = image.channels();

	// 遍历所有行（除第一行和最后一行）
	// 因为第一行没有上一行，最后一行没有下一行，所以要跳过
	for (int j = 1; j < image.rows - 1; j++)
	{
		const uchar* previous = image.ptr<const uchar>(j - 1);	// 上一行
		const uchar* current = image.ptr<const uchar>(j);		// 当前行
		const uchar* next = image.ptr<const uchar>(j + 1);		// 下一行

		uchar* output = result.ptr<uchar>(j); // 输出图像的当前行

		// 遍历所有列（除第一列和最后一列）
		// 因为第一列没有左边一列，最后一列没有右边一列，所以要跳过
		for (int i = nchannels; i < (image.cols - 1) * nchannels; i++)
		{
			// 应用锐化算法 sharpened_pixel=5*current-left-right-up-down；
			// cv::saturate_cast<uchar>()函数在类型转换的同时，会把小于0的数值调整为0，大于255的数值调整为255
			output[i] = cv::saturate_cast<uchar>(5 * current[i]
				- current[i - nchannels]	// left
				- current[i + nchannels]	// right
				- previous[i]				// up
				- next[i]);					// down
		}
	}

	// 把未处理的像素设为0
	result.row(0).setTo(cv::Scalar(0));					// 第一行
	result.row(result.rows - 1).setTo(cv::Scalar(0));	// 最后一行
	result.col(0).setTo(cv::Scalar(0));					// 第一列
	result.col(result.cols - 1).setTo(cv::Scalar(0));	// 最后一列
}


void sharpenIterator(const cv::Mat& image, cv::Mat& result)
{
	// 只在灰度图像下工作
	CV_Assert(image.type() == CV_8UC1); // CV_Assert() 若括号中的表达式为false，则返回一个错误信息，终止程序 执行

	// 初始化迭代器
	cv::Mat_<uchar>::const_iterator it = image.begin<uchar>() + image.cols;			// 当前行从第二行开始
	cv::Mat_<uchar>::const_iterator itend = image.end<uchar>() - image.cols;		// 倒数第二行结束
	cv::Mat_<uchar>::const_iterator itup = image.begin<uchar>();					// 当前行的上一行
	cv::Mat_<uchar>::const_iterator itdown = image.begin<uchar>() + image.cols;		// 当前行的下一行

	// 创建输出图像
	result.create(image.size(), image.type());
	// 初始化输出图像的迭代器
	cv::Mat_<uchar>::iterator itout = result.begin<uchar>() + result.cols;

	// 遍历整个图像
	for (; it != itend; ++it, ++itout, ++itup, ++itdown)
	{
		*itout = cv::saturate_cast<uchar>(*it * 5 - *(it - 1) - *(it + 1) - *itup - *itdown);
	}

	// 把未处理的像素设为0
	result.row(0).setTo(cv::Scalar(0));					// 第一行
	result.row(result.rows - 1).setTo(cv::Scalar(0));	// 最后一行
	result.col(0).setTo(cv::Scalar(0));					// 第一列
	result.col(result.cols - 1).setTo(cv::Scalar(0));	// 最后一列
}


void sharpen2D(const cv::Mat& image, cv::Mat& result)
{
	// Construct kernel (all entries initialize to 0)
	cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));
	// assigns kernel values
	kernel.at<float>(1, 1) = 5.0;
	kernel.at<float>(0, 1) = -1.0;
	kernel.at<float>(2, 1) = -1.0;
	kernel.at<float>(1, 0) = -1.0;
	kernel.at<float>(1, 2) = -1.0;

	// filter the image
	cv::filter2D(image, result, image.depth(), kernel);
}


int main()
{
#pragma region 使用指针

	cv::Mat image = cv::imread("1.JPG");
	if (!image.data)
	{
		return 0;
	}

	cv::Mat result;

	double time = static_cast<double>(cv::getTickCount());

	// 锐化图像
	sharpen(image, result);
	time = static_cast<double>(cv::getTickCount() - time) / cv::getTickFrequency();
	std::cout << "time = " << time << std::endl;

	cv::namedWindow("Image sharpen with Pointers");
	cv::imshow("Image sharpen with Pointers", result);
	cv::waitKey(0);

#pragma endregion

#pragma region 使用迭代器

	// 读入图像（灰度）
	image = cv::imread("1.JPG", cv::IMREAD_GRAYSCALE);

	time = static_cast<double>(cv::getTickCount());

	// 锐化图像
	sharpenIterator(image, result);
	time = (static_cast<double>(cv::getTickCount()) - time) / cv::getTickFrequency();
	std::cout << "time 3 = " << time << std::endl;

	cv::namedWindow("Image Sharpen with Iterators");
	cv::imshow("Image Sharpen with Iterators", result);

	cv::waitKey();

#pragma endregion

#pragma region 使用滤波器

	image = cv::imread("1.JPG", cv::IMREAD_GRAYSCALE);

	time = static_cast<double>(cv::getTickCount());

	// 锐化图像
	sharpen2D(image, result);
	time = (static_cast<double>(cv::getTickCount()) - time) / cv::getTickFrequency();
	std::cout << "time 2D = " << time << std::endl;

	cv::namedWindow("Image Sharpen with Kernel");
	cv::imshow("Image Sharpen with Kernel", result);

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
