// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp> 
#include <opencv2/imgproc.hpp>   
#include <opencv2/highgui.hpp>


///<summary> 
///处理鼠标事件的回调函数 
///</summary> 
///<param name="event">触发回调函数的鼠标事件的类型</param> 
///<param name="x">事件发生时鼠标的x位置</param> 
///<param name="x">事件发生时鼠标的y位置</param> 
///<param name="flags">事件发生时按下了鼠标的哪个键</param> 
///<param name="param">指向任意对象的指针，作为附加的参数传给函数</param> 
///<returns>无</returns> 


void onMouse(int event, int x, int y, int flags, void* param)
{
	// void*类型指针强制转换为cv::Mat*类型
	cv::Mat* im = reinterpret_cast<cv::Mat*>(param); // reinterpret_cast是C++里的强制类型转换符
	
	switch (event) // 调度事件
	{
	case cv::EVENT_LBUTTONDOWN: // 鼠标左键按下事件
		// display pixel value at (x,y)
		std::cout << "At (" << x << ", " << y << ") value is: "
			<< static_cast<int>(im->at<uchar>(cv::Point(x, y)))
			<< std::endl;
		break;
	case cv::EVENT_RBUTTONDOWN: // 鼠标右键按下事件
		std::cout << "The right button is clicked." << std::endl;
		break;
	case cv::EVENT_MOUSEMOVE: // 鼠标移动事件
	//display pixel value at (x,y)
	std::cout << "At (" << x << ", " << y << ") value is: "
		<< static_cast<int>(im->at<uchar>(cv::Point(x, y)))
		<< std::endl;
	break;
	
	}
}


int main()
{
#pragma region 装载图像

	// 创建一个空图像
	cv::Mat image;
	// 检查图像尺寸
	std::cout << "This empty image is " << image.rows << " x " << image.cols << std::endl;
	std::cout << "=========================================================" << std::endl;

	// 读取输入图像
	image = cv::imread("1.JPG", cv::IMREAD_GRAYSCALE); // 读入为单通道灰度图像
	// image = cv::imread("1.JPG", cv::IMREAD_COLOR); // 读入为三通道彩色图像
	// image = cv::imread("1.JPG"); // 第二个参数为空，按图像的原本情况读入
	// 图像的默认路径就是源代码所在位置

	// 错误处理
	if (image.empty()) // 如果图像是空，image.empty()返回1
	{
		// 未创建图像
		// 可能显示一个错误消息并退出程序
		std::cout << "Error reading image..." << std::endl;
		return 0;
	}
	// 检查图像尺寸
	std::cout << "This empty image is " << image.rows << " x " << image.cols << std::endl;
	// 检查图像通道数
	std::cout << "This empty image has " << image.channels() << " channel(s) " << std::endl;
	std::cout << "================================================================" << std::endl;

#pragma endregion

#pragma region 显示，存储图像

	// 定义窗口
	cv::namedWindow("original Image"); //窗口名，为了区分不同窗口
	// 显示图像
	cv::imshow("original Image", image);

	// 创建另一个空的图像（用作输出图像）
	cv::Mat result;

	// 翻转图像
	cv::flip(image, result, 1); // image:输入图像；result:输出图像；int：翻转水平 正数-水平翻转，0-垂直翻转，负数-水平和垂直都翻转

	// 输出窗口
	cv::namedWindow("output Image");
	cv::imshow("output Image", result);

	// 保存图像到本地磁盘，默认路径即源代码所在位置
	cv::imwrite("output.JPG", result);

	//控制台窗口，会在main函数结束时关闭（图像直接关闭），下面的函数可以等待用户按下任意键后再结束程序
	cv::waitKey(0); // 0--表示永远地等待按键（默认值），直到用户按下键盘任意键
					// 键入正数表示等待的毫秒数

#pragma endregion

#pragma region 在图像上点击
	// 注册鼠标点击的回调函数
	// 左上角是原点
	cv::setMouseCallback(
		"original Image",
		onMouse,
		reinterpret_cast<void*>(&image));
	 cv::waitKey(); // 默认值为0
#pragma endregion

#pragma region 在图像上绘图
	// 创建一个新的窗口
	cv::namedWindow("Drawing on an Image");

	// 画圆
	cv::circle(
		image,					// 目标图像
		cv::Point(155, 110),	// 圆心坐标
		65,						// 半径
		0,						// 颜色（此处黑色）
		3						// 厚度
	);

	// 图像上标注文本
	cv::putText(
		image,					// 目标图像
		"This is a dog.",		// 文本
		cv::Point(40, 200),		// 文本位置（左上角）
		cv::FONT_HERSHEY_PLAIN,	// 字体类型
		2.0,					// 字体大小
		255,					// 字体颜色（此处白色）
		2						// 文本厚度
	);

	// 显示图像
	cv::imshow("Drawing on an Image", image);

	cv::waitKey(0);

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