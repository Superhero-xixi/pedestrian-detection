#include<opencv2/opencv.hpp>
#include<opencv2/video/background_segm.hpp>
#include<iostream>
#include<vector>
#include"dataset.h"
using namespace std;
using namespace cv;

void display(Mat gray_diff, vector<Rect>& rect)
{
	//Mat res = src.clone();
	vector<vector<Point>> cts;  //������������
	findContours(gray_diff, cts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //��������,��ģʽΪֻ��������������洢���е�������
																		  //vector<Rect> rect; //������α߿�
	for (int i = 0; i < cts.size(); i++)
	{
		if (contourArea(cts[i])>th_area)       //����������������ų�С�ĸ�������
			
			  //�����ⲿ���α߽�  
			rect.push_back(boundingRect(cts[i]));   //���������Ĵ�ֱ�߽���С����

	}
	cout << rect.size() << endl;     //�����������
}