#if !defined COLORDETECT
#define COLORDETECT

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp> 
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/types_c.h"

class ColorDetector 
{
private:

	// ����Ҫ̽�����ɫ��Ŀ����ɫ�������
	int maxDist;

	// Ŀ����ɫ��BGR��
	cv::Vec3b target;

	// �洢Labɫ�ʿռ��µ�����ͼ��
	cv::Mat converted;
	bool useLab; // �Ƿ�ʹ��Labɫ�ʿռ�

	// �洢�����ͼ��(��ֵͼ��)
	cv::Mat result;

public:

#pragma region ���캯��

	// �չ��캯�����ڴ˳�ʼ��Ĭ�ϲ���
	ColorDetector() : maxDist(100), target(0, 0, 0), useLab(false) {}

	// ����Ĺ��캯����ʹ��Labɫ�ʿռ�
	ColorDetector(bool useLab) : maxDist(100), target(0, 0, 0), useLab(useLab) {}

	// �����Ĺ��캯��
	ColorDetector(uchar blue, uchar green, uchar red, int mxDist = 100, bool useLab = false) : maxDist(mxDist), useLab(useLab) 
	{
		// ����Ŀ����ɫ
		setTargetColor(blue, green, red);
	}

#pragma endregion

	// ͼ������������һ����ͨ����ֵͼ��
	cv::Mat process(const cv::Mat& image);

	// ������Ŀ����ɫ���
	int getDistanceToTargetColor(const cv::Vec3b& color) const 
	{
		return getColorDistance(color, target);
	}

	// ����������ɫ֮��ĳ������루RGBֵ���ľ���ֵ��
	// cv::Vec3b�洢�����޷����ַ��ͣ�����ɫ��RGBֵ
	// color1: Ҫ������ص���ɫֵ
	// color2: Ŀ����ɫֵ
	int getColorDistance(const cv::Vec3b& color1, const cv::Vec3b& color2) const
	{
		// |��B|+|��G|+|��R|
		return abs(color1[0] - color2[0]) +
			abs(color1[1] - color2[1]) +
			abs(color1[2] - color2[2]);
	}
	
#pragma region ʹ��CIE L*a*bɫ�ʿռ�ͷº���
	// ����������������ڶ��ּ�����ɫ������ľ���ķ���
	cv::Mat operator()(const cv::Mat& image) 
	{
		cv::Mat input;

		// �����Ҫת����Labɫ�ʿռ�
		if (useLab) 
		{ 
			cv::cvtColor(image, input, CV_BGR2Lab);
		}
		else 
		{
			input = image;
		}

		cv::Mat output;
		// ��������ͼ����Ŀ����ɫ�ľ���ľ���ֵ
		cv::absdiff(input, cv::Scalar(target), output);
		// �õ����ǣ�ÿ��ͨ����Ŀ����ɫ��Ӧͨ���Ĳ�ֵ��|��B|��|��G|��|��R|��

		// ��������ͼ���ͨ����ֳ�3����ͨ��ͼ�񣬷ֱ��ӦB,G,R
		std::vector<cv::Mat> images;
		cv::split(output, images);

		// ����ͨ���Ĳ�ֵ��ӣ�������ܳ��ֱ��͵������
		output = images[0] + images[1] + images[2]; // |��B|+|��G|+|��R|
												
		// ��ͼ�������ֵ������Ϊ��ֵͼ��
		cv::threshold(
			output,  // ����ͼ��
			output,  // ���ͼ��
			maxDist, // Ҫ̽�����ɫ��Ŀ����ɫ�������
			255,     // Ҫ�������ֵ
			cv::THRESH_BINARY_INV); // ��ֵ��ģʽ
		//				  | 0		if output(x, y) > maxDist	����������ɫ�������ض���ֵΪ0����ɫ��
		// output(x, y) = {
		//				  | 255		otherwise					С��������ɫ�������ض���ֵΪ255����ɫ��

		// ������ֵ����Ķ�ֵͼ��
		return output;
	}

#pragma endregion

#pragma region Getters and setters

	// Sets the color distance threshold.
	// Threshold must be positive, otherwise distance threshold
	// is set to 0.
	void setColorDistanceThreshold(int distance) {

		if (distance < 0)
			distance = 0;
		maxDist = distance;
	}

	// Gets the color distance threshold
	int getColorDistanceThreshold() const {

		return maxDist;
	}

	// Sets the color to be detected
	// given in BGR color space
	void setTargetColor(uchar blue, uchar green, uchar red) {

		// BGR order
		target = cv::Vec3b(blue, green, red);

		if (useLab) {
			// Temporary 1-pixel image
			cv::Mat tmp(1, 1, CV_8UC3);
			tmp.at<cv::Vec3b>(0, 0) = cv::Vec3b(blue, green, red);

			// Converting the target to Lab color space 
			cv::cvtColor(tmp, tmp, CV_BGR2Lab);

			target = tmp.at<cv::Vec3b>(0, 0);
		}
	}

	// Sets the color to be detected
	void setTargetColor(cv::Vec3b color) 
	{

		target = color;
	}

	// Gets the color to be detected
	cv::Vec3b getTargetColor() const {

		return target;
	}
};
#endif