#include "map_PS.h"

int main()
{
	cv::Mat srcMapImg;
	cv::namedWindow("srcImg", CV_WINDOW_NORMAL);
	if (readSrcMapImg("../data/map.pgm", srcMapImg))//�����ͼ
	{

		std::cout << "The result image save as data/result.png\nEnter 'ESC' exit\n";
		cv::imshow("srcImg", srcMapImg);
		cv::waitKey(20);
		//��̬ѧ����
		cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
		//������ 
		//cv::morphologyEx(binaryImg, binaryImg, cv::MORPH_OPEN, element);
		//�ղ��� 
		cv::morphologyEx(srcMapImg, srcMapImg, cv::MORPH_CLOSE, element);

		//���ж�ֵ������ 
		/*cv::Mat binaryImg = cv::Mat::zeros(srcMapImg.size(), CV_8UC1);*/
		cv::Mat binaryImg;
		cv::threshold(srcMapImg, binaryImg, 240, 255, CV_THRESH_BINARY);
		//cv::threshold(srcMapImg, binaryImg, 30, 255, CV_THRESH_BINARY_INV);//�õ�ǽ
		//cv::imshow("binary", binaryImg);

		//�õ�ǽ������
		std::vector< std::vector < cv::Point> > contours;
		std::vector< cv::Vec4i > hierarchy;
		cv::findContours(binaryImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//ֻ��ȡ�ⲿ���������洢����������
		cv::Mat contoursImage(srcMapImg.rows, srcMapImg.cols, CV_8UC1, cv::Scalar(255));
		if (contours.size() != 0)//�ҵ�����
		{
			for (int i = 0; i<contours.size(); i++)
			{
				//����ǽ������
				//ͨ���޸�drawContours()�ĵ�������������޸�ǽ�ĺ�ȣ��˴�����Ϊ2�����ؿ��
				cv::drawContours(srcMapImg, contours, i, cv::Scalar(0),2);
				//std::cout << "draw Contours over" << std::endl;
				//����ͼ��ɫ����
				//fillMap(srcMapImg, srcMapImg,contours[i], true);//����ͼ��������ز���
				floodFillonMouse(srcMapImg,contours[i], true);//�����������ز���
			}
		}
		else
		{
			std::cout << "not find contour" << std::endl;
		}
	}
		
	cv::waitKey(0);
	return 0;

}
