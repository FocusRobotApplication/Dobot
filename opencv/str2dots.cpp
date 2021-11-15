// edge2path.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace cv;
using namespace std;





int main()
{
	Mat imageSource = imread("D://MyObject//Font2Dots//result1.jpg", 0);
	Mat bw_img;
	threshold(imageSource, bw_img, 100, 255, 1);
	imshow("",bw_img);
	waitKey(0);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(bw_img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
	cout << "findContours has done.\n";
	ofstream head_file("D://MyObject//Font2Dots//ImgEdge_test//0.txt");
	head_file << bw_img.cols<< "\n";
	head_file << bw_img.rows << "\n";
	head_file << contours.size() << "\n";
	head_file.close();


	for (int i = 0; i < contours.size(); i++)
	{
		char file_num[5];
		int order = i + 1;
		//sprintf_s(file_num, "%d", order);
		string fix = ".txt";
		string path = "D://MyObject//Font2Dots//ImgEdge_test//";
		string file_name = path + file_num + fix;
		ofstream edge_file(file_name);
		for (int j = 0; j < contours[i].size(); j++)
		{
			edge_file << contours[i][j].x;
			edge_file << " ";
			edge_file << contours[i][j].y;
			edge_file << "\n";
		}
		edge_file.close();
	}
	cout << "All files has saved!\n";
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
