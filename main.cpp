#include<opencv2/opencv.hpp>
#include<opencv2/video/background_segm.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>
#include<iostream>
#include<string>
#include"dataset.h"
using namespace cv;
using namespace std;
using namespace cv::ml;
template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}
void display(Mat, vector<Rect>&);
void Crop_picture();
void train();
void save_hard_example();
void pic_test();
void video_test();
int main()
{
	


	////////////////////////ѵ��
	//Crop_picture();     //���и�����ͼƬ��ÿ�Ÿ�����ͼƬ����ó�10��
	//train();          //ѵ����������
	//save_hardexample()     //�������������õ��ļ���ӣ���INRIAPerson/Train/neg/�е�ͼƬ���в��ԣ���������������
	//train();               //����������������������ѵ��

	//pic_test();    //��ͼƬ�������˼��
	video_test();       //����Ƶ�������˼��
	system("pause");
	return 0;
}