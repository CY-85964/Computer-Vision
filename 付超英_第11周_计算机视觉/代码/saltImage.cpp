// saltImage.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <random>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp> 
#include <opencv2/highgui.hpp>


//在图像中加入椒盐噪声（salt-and-pepper noise）
void salt(cv::Mat image, int n)
{
	// C++11 的默认随机数生成器
	// 调用默认的构造函数，因此会用默认种子来设置初始状态
	// 在不同时刻执行从generator产生的随机数字序列总是相同的，因为种子保持不变
	// 也可以自己提供种子
	std::default_random_engine generator; // 定义一个默认随机数生成器

	// 离散均匀分布的随机数范围（上边界和下边界）
	// 返回均匀分布在闭合范围 [a, b] 内的随机整数
	std::uniform_int_distribution<int> randomRow(0, image.rows - 1); // 椒盐噪声的y坐标的随机数范围
	std::uniform_int_distribution<int> randomCol(0, image.cols - 1); // 椒盐噪声的x坐标的随机数范围

	int i, j;
	for (int k = 0; k < n; k++) // 产生n个椒盐噪声
	{
		// 随机生成图像位置
		i = randomCol(generator); // 将随机数生成器对象传给随机数范围对象，生成椒盐噪声的x坐标
		j = randomRow(generator); // 生成椒盐噪声的x坐标

		if (image.type() == CV_8UC1) // 如果是灰度图像
		{
			// 单通道8位图像
			image.at<uchar>(j, i) = 255; // 颜色替换成白色，增加椒盐噪声
		}
		else if (image.type() == CV_8UC3)
		{
			// 方式一
			// 3 通道图像，下标0，1，2是通道
			// Vec3b: 含3个无符号字符（uchar）类型的数据，即Vec<uchar, 3>
			// image.at<cv::Vec3b>(j, i)[0] = 255; // B
			// image.at<cv::Vec3b>(j, i)[1] = 255; // G
			// image.at<cv::Vec3b>(j, i)[2] = 255; // R

			// 方式二
			// 直接使用短向量: 颜色替换成白色，增加椒盐噪声
			image.at<cv::Vec3b>(j, i) = cv::Vec3b(255, 255, 255); // Vec<uchar, 3>
		}
	}
}


// Mat_使用举例
// 此处只对单通道有用 
void salt2(cv::Mat image, int n)
{
	// 必须是灰度图像
	CV_Assert(image.type() == CV_8UC1);  // 判断是否是灰度图像，如果是则报错退出

	// C++11 的默认随机数生成器
	std::default_random_engine generator; // 定义一个默认随机数生成器
	std::uniform_int_distribution<int> randomRow(0, image.rows - 1); // 椒盐噪声的y坐标的随机数范围
	std::uniform_int_distribution<int> randomCol(0, image.cols - 1); // 椒盐噪声的x坐标的随机数范围

	// 用Mat_模板操作图像（cv::Mat -> cv::Mat_）
	cv::Mat_<uchar> img(image); // 将cv::Mat_类的对象img初始化为图像image，数据类型为uchar

	// 或使用引用：
	// 创建一个cv::Mat_类的引用，但需要进行数据类型的转换，将cv::Mat类型转换为cv::Mat_<uchar>&引用类型
	cv::Mat_<uchar>& im2 = reinterpret_cast<cv::Mat_<uchar>&>(image);
	
	int i, j;
	for (int k = 0; k < n; k++)
	{
		// 随机生成图像位置
		i = randomCol(generator);
		j = randomRow(generator);

		// 此处只考虑灰度图像
		// 颜色替换成白色，增加椒盐噪声
		// 不需要写数据类型，也不需要调用at
		img(j, i) = 255;
	}
}


int main()
{
#pragma region 访问像素值 
	
	// 读入图像（彩色）
	cv::Mat image = cv::imread("1.JPG", cv::IMREAD_COLOR);

	// 调用函数以添加噪声
	salt(image, 3000); // 白色的像素（噪声）数量为3000个

	// 显示图像
	cv::namedWindow("Image");
	cv::imshow("Image", image);
	cv::waitKey();

#pragma endregion

#pragma region 用cv::Mat_操作图像

	// 读入图像（灰度）
	image = cv::imread("1.JPG", cv::IMREAD_GRAYSCALE);

	// 调用函数以添加噪声
	salt2(image, 500); // 白色的像素（噪声）数量为500个

	// 显示图像
	cv::namedWindow("Image");
	cv::imshow("Image", image);
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
