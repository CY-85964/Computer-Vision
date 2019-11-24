// colorReduce.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp> 
#include <opencv2/highgui.hpp>


#define NTESTS 3
#define NITERATIONS 10


#pragma region 用指针扫描图像

void colorReduce(cv::Mat image, int div = 64) // 在原始图像上修改，div-->减色因子
{
	// 图像的行数
	int nl = image.rows;
	// 每行的元素数量
	int nc = image.cols * image.channels();

	for (int j = 0; j < nl; j++) // 行循环
	{
		// 获取行j的地址
		uchar* data = image.ptr<uchar>(j); // ptr模板方法可以直接访问图像中一行的起始地址

		for (int i = 0; i < nc; i++)
		{
			// 处理每个像素 ------------------------

			// 减色算法，三个通道一起处理，指向数组的指针和数组名用法互通
			data[i] = data[i] / div * div + div / 2; // 三个通道同时被处理

			// 像素处理结束 ------------------------
		} // 一行结束
	}
}

#pragma endregion

#pragma region 用迭代器扫描图像

void colorReduce2(cv::Mat image, int div = 64)
{
	// 获取迭代器（两种写法）
	cv::Mat_<cv::Vec3b>::iterator it = image.begin<cv::Vec3b>();	// 使用cv::Mat_类获取迭代器  开始
	cv::MatIterator_<cv::Vec3b> itend = image.end<cv::Vec3b>();		// 使用cv::MatIterator_类获取迭代器  结束

	// 不需要知道行数和列数
	for (; it != itend; ++it) // 迭代器就类似于一个指针，遍历整个图像
	{
		// 处理每个像素 ------------------------

		// 减色算法，每个通道单独处理 
		// it指针指向三个元素（每个位置的三个通道）
		(*it)[0] = (*it)[0] / div * div + div / 2;
		(*it)[1] = (*it)[1] / div * div + div / 2;
		(*it)[2] = (*it)[2] / div * div + div / 2;

		// 像素处理结束 ------------------------
	}
}

#pragma endregion

#pragma region 用at方法扫描图像

void colorReduce3(cv::Mat image, int div = 64)
{
	int nl = image.rows; // 图像的行数
	int nc = image.cols; // 图像的列数

	for (int j = 0; j < nl; j++)
	{
		for (int i = 0; i < nc; i++)
		{
			// 处理每个像素 ------------------------

			// 减色算法，用at方法访问像素，每个通道单独处理 
			image.at<cv::Vec3b>(j, i)[0] = image.at<cv::Vec3b>(j, i)[0] / div * div + div / 2;
			image.at<cv::Vec3b>(j, i)[1] = image.at<cv::Vec3b>(j, i)[1] / div * div + div / 2;
			image.at<cv::Vec3b>(j, i)[2] = image.at<cv::Vec3b>(j, i)[2] / div * div + div / 2;

			// 像素处理结束 ------------------------
		}
	}
}

#pragma endregion

#pragma region 对原始图像或者非原始图像进行修改
// 使用输入和输出参数
// 有的程序不希望对原始图像进行修改
void colorReduceIO(const cv::Mat& image,	// 输入图像，const表示这幅图像不会在函数中修改
	cv::Mat& result,						// 输出图像
	int div = 64)				
{
	int nl = image.rows;
	int nc = image.cols;
	int nchannels = image.channels();

	// 创建一个新图像，如果新图像大小类型与原图像相同，例如：调用时，colorReduceIO(image, image)
	// 这个方法就不会执行任何操作，也不会修改实例，只是直接返回，相当于在原图上修改
	result.create(image.rows, image.cols, image.type());

	for (int j = 0; j < nl; j++)
	{
		// 获取输入图像的行j的地址
		const uchar* data_in = image.ptr<uchar>(j);
		// 获取输出图像的行j的地址
		uchar* data_out = result.ptr<uchar>(j);

		for (int i = 0; i < nc * nchannels; i++)
		{
			// 处理每个像素 ------------------------

			data_out[i] = data_in[i] / div * div + div / 2;

			// 像素处理结束 ------------------------
		} // 一行结束
	}
}


int main()
{
#pragma region 减色测试

	/// <用指针扫描图像>

	// 读取图像
	cv::Mat image = cv::imread("1.JPG");

	// 处理图像
	colorReduce(image, 64); // 减色因子N=64

	// 显示图像
	cv::namedWindow("Image Scan with Pointers");
	cv::imshow("Image Scan with Pointers", image);
	cv::waitKey(0);

	/// <用迭代器扫描图像>

	// 读取图像
	cv::Mat image2 = cv::imread("1.JPG");

	// 处理图像
	colorReduce2(image2, 64); // 减色因子N=64

	// 显示图像
	cv::namedWindow("Image Scan with Iterators");
	cv::imshow("Image Scan with Iterators", image2);
	cv::waitKey(0);

	/// <用at方法扫描图像>

	// 读取图像
	cv::Mat image3 = cv::imread("1.JPG");

	// 处理图像
	colorReduce3(image3, 64); // 减色因子N=64

	// 显示图像
	cv::namedWindow("Image Scan with at Method");
	cv::imshow("Image Scan with at Method", image3);
	cv::waitKey(0);

	/// <用输入输出的指针方法扫描图像>

	// 读取图像
	cv::Mat image4 = cv::imread("1.JPG");
	cv::Mat image5;

	// 处理图像
	colorReduceIO(image4, image5, 64); // 减色因子N=64

	// 显示图像
	cv::namedWindow("Image Scan with original");
	cv::namedWindow("Image Scan with IO");
	cv::imshow("Image Scan with original", image4);
	cv::imshow("Image Scan with IO", image5);
	cv::waitKey(0);

	cv::destroyAllWindows();

#pragma endregion

#pragma region 运行时间测试

	int64 t[NTESTS], tinit;

	// 每个函数对应的运行时间初始化位0
	for (int i = 0; i < NTESTS; i++)
	{
		t[i] = 0;
	}

	cv::Mat images[NTESTS];
	cv::Mat result;

	// 用来测试的函数指针数组
	typedef void(*FunctionPointer)(cv::Mat, int);
	FunctionPointer functions[NTESTS] = { colorReduce, colorReduce2, colorReduce3 };
	// 或者直接
	// void(*Functions[NTESTS])(cv::Mat, int) = { colorReduce, colorReduce2, colorReduce3 };

	// 重复测试这些函数NITERATIONS次
	int n = NITERATIONS;
	for (int k = 0; k < n; k++)
	{
		std::cout << k << " of " << n << std::endl;

		// 测试每个用不同扫描方式实现的减色函数
		for (int c = 0; c < NTESTS; c++)
		{
			// 读入图像
			images[c] = cv::imread("1.JPG");

			// 设置计时器
			tinit = cv::getTickCount();
			// 调用减色函数
			functions[c](images[c], 64);
			// 计算每次函数运行时间
			t[c] += cv::getTickCount() - tinit;

			std::cout << ".";
		}

		std::cout << std::endl;
	}

	// 每个函数的简短描述
	std::string descriptions[NTESTS] = {
		"用指针扫描图像:",
		"用迭代器扫描图像:",
		"用at方法扫描图像:"
	};

	// 打印出平均执行时间
	std::cout << std::endl << "------------------------------------------------" << std::endl << std::endl;
	for (int i = 0; i < NTESTS; i++)
	{
		std::cout << i << "." << descriptions[i] << 1000. * t[i] / cv::getTickFrequency() / n
			<< "ms" << std::endl;
	}

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
