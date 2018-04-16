#include "map_PS.h"

int main()
{
	cv::Mat srcMapImg;
	cv::namedWindow("srcImg", CV_WINDOW_NORMAL);
	if (readSrcMapImg("../data/map.pgm", srcMapImg))//读入地图
	{

		std::cout << "The result image save as data/result.png\nEnter 'ESC' exit\n";
		cv::imshow("srcImg", srcMapImg);
		cv::waitKey(20);
		//形态学操作
		cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
		//开操作 
		//cv::morphologyEx(binaryImg, binaryImg, cv::MORPH_OPEN, element);
		//闭操作 
		cv::morphologyEx(srcMapImg, srcMapImg, cv::MORPH_CLOSE, element);

		//进行二值化处理 
		/*cv::Mat binaryImg = cv::Mat::zeros(srcMapImg.size(), CV_8UC1);*/
		cv::Mat binaryImg;
		cv::threshold(srcMapImg, binaryImg, 240, 255, CV_THRESH_BINARY);
		//cv::threshold(srcMapImg, binaryImg, 30, 255, CV_THRESH_BINARY_INV);//得到墙
		//cv::imshow("binary", binaryImg);

		//得到墙体轮廓
		std::vector< std::vector < cv::Point> > contours;
		std::vector< cv::Vec4i > hierarchy;
		cv::findContours(binaryImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//只获取外部轮廓，并存储所有轮廓点
		cv::Mat contoursImage(srcMapImg.rows, srcMapImg.cols, CV_8UC1, cv::Scalar(255));
		if (contours.size() != 0)//找到轮廓
		{
			for (int i = 0; i<contours.size(); i++)
			{
				//绘制墙体轮廓
				//通过修改drawContours()的第五个参数可以修改墙的厚度，此处设置为2个像素宽度
				cv::drawContours(srcMapImg, contours, i, cv::Scalar(0),2);
				//std::cout << "draw Contours over" << std::endl;
				//填充地图灰色区域
				//fillMap(srcMapImg, srcMapImg,contours[i], true);//遍历图像进行像素操作
				floodFillonMouse(srcMapImg,contours[i], true);//用鼠标进行像素操作
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
