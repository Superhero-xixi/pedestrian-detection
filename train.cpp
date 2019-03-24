
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
void train() 
{
	HOGDescriptor hog(Size(64, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9);//HOG���������������HOG�����ӵģ���������ά��Ϊ3780

	int DescriptorDim = 0;//HOG�����ӵ�ά������ͼƬ��С����ⴰ�ڴ�С�����С��ϸ����Ԫ��ֱ��ͼbin��������

	Ptr<SVM> svm = SVM::create();//SVM������

	//   svm��������
	svm->setType(ml::SVM::C_SVC);   //����svm�����ͣ���ѵ�������Ƿ����Է����
	svm->setKernel(SVM::LINEAR);      //���ú˺���������
	svm->setTermCriteria(TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 50000, FLT_EPSILON));   //���õ�����ֹ׼�����зֱ�Ϊ׼�����ͣ�������������Ŀ�꾫��
	
	if (TRAIN)
	{
		string ImgName;
		ifstream finPos(PosSamListFile);//������ͼƬ���ļ����б�
		ifstream finNeg(NegSamListFile);//������ͼƬ���ļ����б�

		Mat sampleFeatureMat;//����ѵ������������������ɵľ��������������������ĸ�������������������������������֮�ͣ�����������HOG������ά��  ����������*3780�ľ���
		Mat sampleLabelMat;//ѵ����������������������������������ĸ�������������1��1��ʾ���ˣ�-1��ʾ����  ������������*1�ľ���


		//���ζ�ȡ������ͼƬ������HOG������
		for (int num = 0; num < PosSamNO && getline(finPos, ImgName); num++)
		{
			cout << "����" << ImgName << endl;
			
			ImgName = "dataset/pos/" + ImgName;//������������·����
			Mat src = imread(ImgName);
			if (CENTRAL_CROP)
				if (src.cols >=96 && src.rows >= 160)
					src = src(Rect(16, 16, 64, 128));//��96*160��INRIA������ͼƬ����Ϊ64*128������ȥ�������Ҹ�16������
					//  resize(src,src,Size(64,128));

			vector<float> descriptors;//HOG����������
			hog.compute(src, descriptors, Size(8, 8));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
			//cout<<"������ά����"<<descriptors.size()<<endl;
			cout << descriptors.size() << endl;
			//�����һ������ʱ��ʼ�����������������������Ϊֻ��֪��������������ά�����ܳ�ʼ��������������
			if (0 == num)
			{
				DescriptorDim = descriptors.size();//HOG�����ӵ�ά��
				 //��ʼ������ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��sampleFeatureMat
				sampleFeatureMat = Mat::zeros(PosSamNO + NegSamNO + HardExampleNO, DescriptorDim, CV_32FC1);
				//��ʼ��ѵ����������������������������������ĸ�������������1��1��ʾ���ˣ�0��ʾ����
				sampleLabelMat = Mat::zeros(PosSamNO + NegSamNO + HardExampleNO, 1, CV_32SC1);
			}

			//������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
			for (int i = 0; i < DescriptorDim; i++)
				sampleFeatureMat.at<float>(num, i) = descriptors[i];//��num�����������������еĵ�i��Ԫ��
			sampleLabelMat.at<float>(num, 0) = 1;//���������Ϊ1������
			 //CV_MAT_ELEM(sampleFeatureMat,float,num,0)=1;
		}

		//���ζ�ȡ������ͼƬ������HOG������
		for (int num = 0; num < NegSamNO && getline(finNeg, ImgName); num++)
		{
			cout << "����" << ImgName << endl;
		
			ImgName = "dataset/neg/" + ImgName;
			Mat src = imread(ImgName);//��ȡͼƬ
			
			vector<float> descriptors;//HOG����������
			hog.compute(src, descriptors, Size(8, 8));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
			cout << "������ά����" << descriptors.size() << endl;

			//������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
			for (int i = 0; i < DescriptorDim; i++)
				sampleFeatureMat.at<float>(num + PosSamNO, i) = descriptors[i];//��PosSamNO+num�����������������еĵ�i��Ԫ��
			sampleLabelMat.at<float>(num + PosSamNO, 0) = -1;//���������Ϊ-1������

		}

		//����HardExample������
		if (HardExampleNO > 0)
		{
			ifstream finHardExample(HardExampleListFile);
			 //���ζ�ȡHardExample������ͼƬ������HOG������
			for (int num = 0; num < HardExampleNO && getline(finHardExample, ImgName); num++)
			{
				cout << "����" << ImgName << endl;
				
				ImgName = "dataset/HardExample/" + ImgName;//����HardExample��������·����
				Mat src = imread(ImgName);
				

				vector<float> descriptors;//HOG����������
				hog.compute(src, descriptors, Size(8, 8));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
				cout << "������ά����" << descriptors.size() << endl;

				//������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
				for (int i = 0; i < DescriptorDim; i++)
					sampleFeatureMat.at<float>(num + PosSamNO + NegSamNO, i) = descriptors[i];//��PosSamNO+num�����������������еĵ�i��Ԫ��
				sampleLabelMat.at<float>(num + PosSamNO + NegSamNO, 0) = -1;//���������Ϊ-1������
			}
		}

		//���������HOG�������������ļ�
		ofstream fout("SampleFeatureMat.txt");
		for(int i=0; i<PosSamNO+NegSamNO; i++)
		{
		fout<<i<<endl;
		for(int j=0; j<DescriptorDim; j++)
		{	fout<<sampleFeatureMat.at<float>(i,j)<<"  ";
		}
		fout<<endl;
		}

		cout << "��ʼѵ��SVM������" << endl;
		svm->train(sampleFeatureMat, ROW_SAMPLE, sampleLabelMat);//ѵ��������
		cout << "ѵ�����" << endl;
		svm->save("SVM_HOG.xml");//��ѵ���õ�SVMģ�ͱ���Ϊxml�ļ�

	}
	
	DescriptorDim = svm->getVarCount();//����������ά������HOG�����ӵ�ά��
	cout << "�����ӵ�ά��" << DescriptorDim << endl;

	cv::Mat svecsmat = svm->getSupportVectors();  //��ȡsvecsmat��Ԫ������Ϊfloat
	int svdim = svm->getVarCount();
	int numofsv = svecsmat.rows;

	
	cv::Mat alphamat = cv::Mat::zeros(numofsv, svdim, CV_32F);
	cv::Mat svindex = cv::Mat::zeros(1, numofsv, CV_64F);

	cv::Mat Result;
	double rho = svm->getDecisionFunction(0, alphamat, svindex);
	//��alphamatԪ�ص�������������ת��CV_32F
	alphamat.convertTo(alphamat, CV_32F);
	Result = -1 * alphamat * svecsmat;

	std::vector<float> vec;
	for (int i = 0; i < svdim; ++i)
	{
		vec.push_back(Result.at<float>(0, i));
	}
	vec.push_back(rho);

	//saving HOGDetectorForOpenCV.txt
	std::ofstream fout("HOGDetectorForOpenCV.txt");
	for (int i = 0; i < vec.size(); ++i)
	{
		fout << vec[i] << std::endl;
	}
	cout << vec.size() << endl;
}