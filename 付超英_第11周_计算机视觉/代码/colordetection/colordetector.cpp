#include "colordetector.h"
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp> 
#include <opencv2/highgui.hpp>


cv::Mat ColorDetector::process(const cv::Mat& image) 
{
	// �������ͼ��������ͼ��ĳߴ���ͬ�������ǵ�ͨ���Ҷ�ͼ�񣨶�ֵͼ��
	result.create(image.size(), CV_8U);

	// ������������ͼ��ĵ�������û�г�ʼ������Ϊ��߻����ж����Ǹ�ɫ�ʿռ䣩
	cv::Mat_<cv::Vec3b>::const_iterator it;
	cv::Mat_<cv::Vec3b>::const_iterator itend;

	// ���ͼ��ĵ�����
	cv::Mat_<uchar>::iterator itout = result.begin<uchar>();

#pragma region ת��ɫ�ʿռ�

	// �ж��Ƿ���Ҫ��CLE L*a*bɫ�ʿռ�
	if (useLab)
		// ������ͼ��Ĭ��RGBɫ�ʿռ䣩ת��ΪCLE L*a*bɫ�ʿռ䣨���Labɫ�ʿռ䣩
		cv::cvtColor(image, converted, CV_BGR2Lab);
#pragma endregion

	// get the iterators
	cv::Mat_<cv::Vec3b>::const_iterator it = image.begin<cv::Vec3b>();
	cv::Mat_<cv::Vec3b>::const_iterator itend = image.end<cv::Vec3b>();
	cv::Mat_<uchar>::iterator itout = result.begin<uchar>();

	// ��ʼ������ͼ��ĵ�����
	if (useLab)  // ���ʹ��Labɫ�ʿռ�
	{
		it = converted.begin<cv::Vec3b>();
		itend = converted.end<cv::Vec3b>();
	}
	else
	{
		it = image.begin<cv::Vec3b>();
		itend = image.end<cv::Vec3b>();
	}

	// ��������ͼ��
	for (; it != itend; ++it, ++itout) 
	{
		/// <�Ƚ�����ͼ����ÿ�����ص���ɫ��Ŀ����ɫ֮��Ĳ��>

		  // �������ͼ���е�ǰ���ص���ɫ��Ŀ����ɫ�Ĳ��<������������
		if (getDistanceToTargetColor(*it) < maxDist) // getDistanceToTargetColor ������������Ŀ����ɫ�Ĳ��
		{
			// ���ͼ���е�ǰ���ص���ɫ��ֵΪ255����ɫ��
			*itout = 255; // ��ʾ�����ص���ɫ��Ŀ����ɫ�ӽ�
		}
		else 
		{
			// ���ͼ���е�ǰ���ص���ɫ��ֵΪ255����ɫ��
			*itout = 0;  // ��ʾ�����ص���ɫ��Ŀ����ɫ���ϴ�
		}
	}

	// �������ͼ�񣨶�ֵͼ��
	return result;
}
