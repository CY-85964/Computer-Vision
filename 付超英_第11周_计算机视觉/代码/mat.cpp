// mat.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp> 
#include <opencv2/imgproc.hpp>   
#include <opencv2/highgui.hpp>


cv::Mat getImg()
{
	// 创建图像
	cv::Mat ima(500, 500, CV_8U, 50);
	//返回图像
	return ima;
}




int main()
{
#pragma region 图像的创建

	/* 灰度图像的创建 */
	// 创建一个240行 *320列的新灰度图像
	cv::Mat image1(240, 320, CV_8U, 100); // 所有点像素都为100
	// 或者
	// cv::Mat image1(240, 320, CV_8U, cv::Scalar(100)); // scalar--定义最多四个元素的向量值
	// 或者
	// cv::Mat image1(240, 320, CV_8U);
	// image1 = 100;

	cv::namedWindow("Image"); // 定义窗口
	cv::imshow("Image", image1); // 显示图像
	cv::waitKey(0); // 等待按键

	/* 彩色图像的创建 */

	// 创建一个红色的彩色图像，用cv::size(320, 240)以提供图像的尺寸信息
	cv::Mat image2(cv::Size(320, 240), CV_8UC3, cv::Scalar(0, 0, 255)); // 通道次序为BGR  Size-->可有两个参数(平面)也可有三个(三维立体) 常用
	// 或者
	// cv::Mat image2(cv::Size(320, 240), CV_8UC3);
	// image2 = cv::Scalar(0, 0, 255);

	cv::imshow("Image", image2); // 显示图像
	cv::waitKey(0); // 等待按键
	cv::destroyAllWindows(); //销毁所有窗口

#pragma endregion

#pragma region 图像类型

	// U: 表示无符号整数 unsigned int
	// S: 表示有符号整数 signed int
	// F: 表示浮点数 float

	// C: 表示通道数 channel
	// 无C/C1: 单通道，灰度图像
	// C3: 三通道，彩色图像（BGR）

	// 8bit(位) = 1Byte(字节)
	// 8bit: char
	// 16/32bit: int
	// 32/64bit: float

#pragma endregion

#pragma region 图像的重分配

	// 重新分配一个新的图像（原来的内容会先被释放）
	// 如果新的尺寸和类型与原来相同，就不会重新分配内存
	// 相当于修改原图像大小或类型
	image1.create(200, 200, CV_8U);
	image1 = 200; // 初始化图像
	
	cv::namedWindow("Image"); // 定义窗口
	cv::imshow("Image", image1); // 显示图像
	cv::waitKey(0); // 等待按键
	cv::destroyWindow("Image"); // 销毁“Image”窗口

#pragma endregion

#pragma region 图像的浅拷贝与深拷贝

	// 读入一幅图像
	cv::Mat image3 = cv::imread("1.JPG");

	// 浅拷贝，所有图像都指向一个数据块
	cv::Mat image4(image3); // 浅拷贝1
	image1 = image3; // 浅拷贝2

	// 深拷贝，这些图像是源图像的副本
	image3.copyTo(image2); // 深拷贝1
	cv::Mat image5 = image3.clone(); // 深拷贝2

	// 转换图像进行检测，水平翻转image3
	cv::flip(image3, image3, 1);

	// 定义窗口
	cv::namedWindow("Image 1");
	cv::namedWindow("Image 2");
	cv::namedWindow("Image 3");
	cv::namedWindow("Image 4");
	cv::namedWindow("Image 5");

	cv::imshow("Image 3", image3); // 受影响

	// 检查哪些图像在处理过程中受到了影响
	cv::imshow("Image1", image1); // 受影响
	cv::imshow("Image4", image4); // 受影响

	cv::imshow("Image2", image2); // 不受影响
	cv::imshow("Image5", image5); // 不受影响

	cv::waitKey(0); // 等待按键
	cv::destroyAllWindows(); // 销毁所有窗口
#pragma endregion

#pragma region 从函数中获取图像

	// 从函数中获取一个灰度图
	cv::Mat gray = getImg(); // 调用自己定义的函数

	// 显示图像
	cv::namedWindow("Image from function"); // 定义窗口
	cv::imshow("Image from function", gray); 
	cv::waitKey(0);
	cv::destroyWindow("Image from function");

#pragma endregion

#pragma region 图像的转换

	//作为灰度图读入
	cv::Mat image6 = cv::imread("1.JPG", cv::IMREAD_GRAYSCALE);
	cv::Mat image7;

	//转换成浮点型图像，像素强度范围[0, 1], 两幅图像的通道数量必须相同
	image6.convertTo(
		image7, // 输出图像 如果是image6，则在原始图像上直接修改
		CV_32F, // 图像类型 rType
		1 / 255.0, // 缩放比例 α
		0.0 // 偏移量 β
	);
	// image7(x, y) = saturate_cast<rType>(α * image6(x, y) + β)

	cv::namedWindow("Image 7"); // 定义窗口
	cv::imshow("Image 7", image7);
	cv::waitKey(0);
	cv::destroyWindow("Image 7");

#pragma endregion

#pragma region 处理小矩阵

	// 3*3双精度型矩阵1
	cv::Matx33d matrix( // 并不是所有矩阵都可以这样定义
		3.0, 2.0, 1.0,
		2.0, 1.0, 3.0,
		1.0, 2.0, 3.0
	);

	// 或者 
	// 3*3双精度型矩阵2  最原始的定义方式
	// cv::Matx<double, 3, 3> matrix1(
	// 	3.0, 2.0, 1.0,
	// 	2.0, 1.0, 3.0,
	// 	1.0, 2.0, 3.0
	// );

	// 3*1矩阵（即列向量）
	cv::Matx31d vector(5.0, 1.0, 3.0); // cv::Matx<double, 3, 1>

	// 矩阵相乘
	cv::Matx31d result = matrix * vector; //(3 x 3) * (3 x 1)

	std::cout << result;

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
