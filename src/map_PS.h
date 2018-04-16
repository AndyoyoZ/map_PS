#ifndef _MAP_PS_H
#define _MAP_PS_H

#include <opencv2/opencv.hpp>//包含opencv头文件
#include <iostream>
#include <fstream>


//-----------------------------readSrcMapImg()---------------------------
//                             读取地图文件
//filename: 地图文件所在路径和文件名
//srcMapImg:用来存储读取的地图文件
bool readSrcMapImg(const std::string& filename, cv::Mat& srcMapImg)
{
	srcMapImg = cv::imread(filename,0);//载入灰度图
	if (!srcMapImg.data)
	{
		std::cout << "loading map error" << std::endl;
		return false;
	}
	else
	{
		std::cout << "loading map success" << std::endl;
		return true;
	}
		
}


//--------------------------saveMapValue()-----------------------------------
//保存像素值到TXT文件
//MapImg：输入图像
//filename：输出文件名
//mode: 方式
void saveMapValue(cv::Mat MapImg, const std::string& filename, int mode = 0)
{

	std::ofstream fout(filename);
	char buf[20];
	if (mode == 0)//用IplImage类型按行列遍历像素
	{
		IplImage tempImg;
		tempImg = IplImage(MapImg);//Mat转IplImage
		uchar* ptr;
		for (int i = 0; i < tempImg.height; i++)
		{
			for (int j = 0; j < tempImg.width; j++)
			{
				ptr = cvPtr2D(&tempImg, i, j, NULL);//获取像素坐标点的像素值
				sprintf(buf, "%d\t", ptr[0]);//通过格式控制符转为十进制数存放在buf中
				fout << buf;//写入文件
			}
		}
	}
	else
		if (mode == 1)//对Mat类型用指针遍历像素
		{
			cv::Mat outputImage;
			// 创建与原图像等尺寸的图像
			outputImage.create(MapImg.size(), MapImg.type());
			int nr = MapImg.rows;
			// 将3通道转换为1通道
			int nl = MapImg.cols*MapImg.channels();
			for (int k = 0; k<nr; k++)
			{
				// 每一行图像的指针
				const uchar* inData = MapImg.ptr<uchar>(k);
				uchar* outData = outputImage.ptr<uchar>(k);
				for (int i = 0; i<nl; i++)
				{
					//if (inData[i] == 0)//将白色区域赋值为205
					//{
					//	outData[i] = 205;
					//}
					sprintf(buf, "%x\t", inData[i]);//通过格式控制符转为十进制数存放在buf中
					fout << buf;//写入文件
				}
			}
		 }
	std::cout << "save txt over" << std::endl;
}


//-------------------------------------fillMap()-------------------------------------------------
//填充地图内像素点
//inputImg： 输入图像
//outputImg：输出图像
//contour：  输入轮廓
//measureDist:测量点到轮廓的距离
//			  当measureDist设置为true 时，若返回值为正，表示点在轮廓内部，返回值为负，表示在轮廓外部，返回值为0，表示在轮廓上。 
//            当measureDist设置为false时，若返回值为 + 1，表示点在轮廓内部，返回值为 - 1，表示在轮廓外部，返回值为0，表示在轮廓上。
void fillMap(cv::Mat &inputImg, cv::Mat &outputImg, std::vector < cv::Point> contour, bool measureDist)
{
	if (inputImg.channels() == 1)//单通道图像
	{
		std::cout << "fill map ..." << std::endl;
		int nr = inputImg.rows;
		int nl = inputImg.cols;
		for (int k = 0; k<nr; k++)
		{
			// 每一行图像的指针
			const uchar* inData = inputImg.ptr<uchar>(k);
			uchar* outData = outputImg.ptr<uchar>(k);
			for (int i = 0; i<nl; i++)
			{
				if (cv::pointPolygonTest(contour, cv::Point(i, k), measureDist)>0 && inData[i] < 230)//将灰色区域赋值为255
				{
					outData[i] = 255;
				}
			}
		}
		
		cv::imshow("result",outputImg);//显示处理结果
		cv::imwrite("../data/result.pgm", outputImg);//保存处理结果
		std::cout << "fill map success" << std::endl;
	}
	else
		if (inputImg.channels() == 3)
		{
			std::cout << "please trans srcImage to 1 channal" << std::endl;
		}
}



//----------------------colorReduce()-------------------------------
//降色彩
//inputImage：输入图像
//outputImage：输出图像
//div：  等分
void colorReduce(const cv::Mat& inputImage, cv::Mat& outputImage, int div)
 {
	// 创建与原图像等尺寸的图像
	 outputImage.create(inputImage.size(), inputImage.type());
	 int nr = inputImage.rows;
	// 将3通道转换为1通道
	 int nl = inputImage.cols*inputImage.channels();
	for (int k = 0; k<nr; k++)
	{
		// 每一行图像的指针
		const uchar* inData = inputImage.ptr<uchar>(k);
		uchar* outData = outputImage.ptr<uchar>(k);
		for (int i = 0; i<nl; i++)
		{
			 outData[i] = inData[i] / div*div + div / 2;//降色彩
		}
	}
 }


//------------------------------mouseHandler()-------------------------------------
//鼠标回调函数，获取像素值
//
//
void mouseHandler(int event, int x, int y, int flags, void* param)
{
	IplImage *img0, *img1;
	img0 = (IplImage*)param;
	img1 = cvCreateImage(cvSize(img0->width, img0->height), img0->depth, img0->nChannels);
	cvCopy(img0, img1, NULL);

	CvFont font;
	uchar* ptr;
	char label[20];
	char label2[20];

	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1, 1, 0, 1, 1);    //初始化字体

	if (event == CV_EVENT_LBUTTONDOWN)
	{
		//读取像素
		ptr = cvPtr2D(img0, y, x, NULL);

		sprintf(label, "Grey Level:%d", ptr[0]);
		sprintf(label2, "Pixel: (%d, %d)", x, y);
		//调整显示位置
		if (img0->width - x <= 180 || img0->height - y <= 20)
		{
			cvRectangle(img1, cvPoint(x - 180, y - 40), cvPoint(x - 10, y - 10), CV_RGB(255, 0, 0), CV_FILLED, 8, 0);
			cvPutText(img1, label, cvPoint(x - 180, y - 30), &font, CV_RGB(255, 255, 255));
			cvPutText(img1, label2, cvPoint(x - 180, y - 10), &font, CV_RGB(255, 255, 255));
		}
		else
		{
			cvRectangle(img1, cvPoint(x + 10, y - 12), cvPoint(x + 180, y + 20), CV_RGB(255, 0, 0), CV_FILLED, 8, 0);
			cvPutText(img1, label, cvPoint(x + 10, y), &font, CV_RGB(255, 255, 255));
			cvPutText(img1, label2, cvPoint(x + 10, y + 20), &font, CV_RGB(255, 255, 255));
		}
		//以鼠标为中心画点
		CvPoint centerPoint;
		centerPoint.x = x;
		centerPoint.y = y;
		cvCircle(img1, centerPoint, 3, CV_RGB(0, 0, 0), 1, 8, 3);

		cvShowImage("img", img1);
	}
}

void getPixelValue(IplImage* img)
{
	int exit = 0;
	int c;
	assert(img);//检验参数img的合法性，img为空，则终止程序并返回错误值

	if (img->nChannels != 1) //判断是否为灰度图像，若不是，先转换为灰度图像
	{
		//转换为灰度图像
		IplImage* gray = cvCreateImage(cvGetSize(img), img->depth, 1);
		cvCvtColor(img, gray, CV_BGR2GRAY);
	}
	cvNamedWindow("img", 1);
	cvSetMouseCallback("img", mouseHandler, (void*)img);
	cvShowImage("img", img);
	//检测键盘输入，按Q键退出程序
	while (!exit)
	{
		c = cvWaitKey(0);
		switch (c)
		{
		case 'q':
			exit = 1;
			break;
		default:
			continue;
		}
	}
	cvDestroyAllWindows();
	cvReleaseImage(&img);
}


//-----------------------------------【onMouse( )函数】--------------------------------------    
//鼠标onMouse回调函数，漫水填充
//---------------------------------------------------------------------------------------------  
cv::Mat dst;//漫水填充输出图像
int g_nFillMode = 1;//漫水填充的模式  
int g_nLowDifference = 10, g_nUpDifference = 10;//负差最大值、正差最大值  
int g_nConnectivity = 8;//表示floodFill函数标识符低八位的连通值  
int g_bIsColor  = false;//是否为彩色图的标识符布尔值  
bool g_bUseMask = false;//是否显示掩膜窗口的布尔值  
int g_nNewMaskVal = 255;//新的重新绘制的像素值  
std::vector < cv::Point> contour;
bool measureDist;

static void onMouse(int event, int x, int y, int, void*)
{
	// 若鼠标左键没有按下，便返回  
	if (event != CV_EVENT_LBUTTONDOWN)
		return;

	//-------------------【<1>调用floodFill函数之前的参数准备部分】---------------  
	cv::Point seed = cv::Point(x, y);
	if(cv::pointPolygonTest(contour, seed, measureDist)>0)//种子点在轮廓内
	{
	uchar pixelValue=dst.at<uchar>(y,x);
	printf("Point(%d, %d):%d\n",x,y,pixelValue);
	if(pixelValue==205)//只处理灰色区域(205)
	{
	int LowDifference = g_nFillMode == 0 ? 0 : g_nLowDifference;//空范围的漫水填充，此值设为0，否则设为全局的g_nLowDifference  
	int UpDifference = g_nFillMode == 0 ? 0 : g_nUpDifference;//空范围的漫水填充，此值设为0，否则设为全局的g_nUpDifference  
	int flags = g_nConnectivity + (g_nNewMaskVal << 8) +
		(g_nFillMode == 1 ? CV_FLOODFILL_FIXED_RANGE : 0);//标识符的0~7位为g_nConnectivity，8~15位为g_nNewMaskVal左移8位的值，16~23位为CV_FLOODFILL_FIXED_RANGE或者0。  

	cv::Rect ccomp;//定义重绘区域的最小边界矩形区域  

	cv::Scalar newVal = 254;//在重绘区域像素的新值(254)
	 
	int area;

	//--------------------【<2>正式调用floodFill函数】-----------------------------  

		area = floodFill(dst, seed, newVal, &ccomp, cv::Scalar(LowDifference, LowDifference, LowDifference),
			cv::Scalar(UpDifference, UpDifference, UpDifference), flags);

	//cv::imshow("效果图", dst);
	cv::imwrite("../data/result.pgm", dst);//保存效果图
	//std::cout << area << " 个像素被重绘\n";
	}
	else
	{
		;
	}
	}
	else
	{
		std::cout <<"Please select the points in the coutour!"<<std::endl;
	}
}


//-----------------------------------【floodFillonMouse( )函数】--------------------------------------    
//漫水填充,进行像素操作
//---------------------------------------------------------------------------------------------  
int floodFillonMouse(cv::Mat inputImg,std::vector < cv::Point> _contour, bool _measureDist)
{
	inputImg.copyTo(dst);//输入图像拷贝
	contour=_contour;
	measureDist=_measureDist;
	cv::namedWindow("resultImg", CV_WINDOW_NORMAL);
	//鼠标回调函数  
	cv::setMouseCallback("resultImg", onMouse, 0);
	
	while (1)
	{
		cv::imshow("resultImg", dst);
		if (cv::waitKey(10) == 27)
		{
			return 0;
		}
	}
}

#endif
