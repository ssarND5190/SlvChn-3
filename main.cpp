#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <cmath>
#include <ctime>
#include "img.h"

using namespace cv;
using namespace std;

pixel srcIng1024[1024][1024];
pixel dstIng1024[1024][1024];

int main() {
	//-----------------------------SETTINGS------------------------------------
	int SHOWSIZEX = 512;
	int SHOWSIZEY = 512;
	//-----------------------------INPUT------------------------------------
    Mat src = imread("./test/src.png");
    Mat dst = imread("./test/dstMD.png");
	if (src.empty()) {
		cout << "ERROR: NO IMAGE" << endl;
		return -1;
	}
	if (dst.empty()) {
		cout << "ERROR: NO IMAGE" << endl;
		return -1;
	}

	namedWindow("Display Image", WINDOW_AUTOSIZE);

	//-----------------------------Resize 128-------------------------------
    Mat src128;
    cv::resize(src, src128, Size(128, 128), INTER_LINEAR);
    pixel srcIng128[128][128];
    for (int i = 0; i < 128; i++) {
        for (int j = 0; j < 128; j++) {
            Vec3b px = src128.at<Vec3b>(i, j);
            srcIng128[i][j].B = (int)px[0];
            srcIng128[i][j].G = (int)px[1];
            srcIng128[i][j].R = (int)px[2];
        }
    }
	Mat dst128;
	cv::resize(dst, dst128, Size(128, 128), INTER_LINEAR);
	pixel dstIng128[128][128];
	for (int i = 0; i < 128; i++) {
		for (int j = 0; j < 128; j++) {
			Vec3b px = dst128.at<Vec3b>(i, j);
			dstIng128[i][j].B = (int)px[0];
			dstIng128[i][j].G = (int)px[1];
			dstIng128[i][j].R = (int)px[2];
		}
	}

	//-----------------------------INIT Matching-------------------------------
	for (int sy = 0; sy < 128; sy++) {
		for (int sx = 0; sx < 128; sx++) {
			srcIng128[sx][sy].bestMX = sx;
			dstIng128[sx][sy].bestMX = sx;
			srcIng128[sx][sy].bestMY = sy;
			dstIng128[sx][sy].bestMY = sy;
		}
	}

	//-----------------------------PROCESSING 128-------------------------------
	Mat output128 = src128.clone();
	Mat outshow;
	cv::resize(output128, outshow, Size(SHOWSIZEX, SHOWSIZEY), INTER_LINEAR);

    int iteration128 = 1500;
	while (iteration128 > 0) {
		int x1 = rand() % 128;
		int y1 = rand() % 128;
		for (int x2 = 0; x2 < 128; x2++) {
			for (int y2 = 0; y2 < 128; y2++) {
				pixel* srcPX1 = &srcIng128[dstIng128[x1][y1].bestMX][dstIng128[x1][y1].bestMY];
				pixel* srcPX2 = &srcIng128[dstIng128[x2][y2].bestMX][dstIng128[x2][y2].bestMY];
				int old = ((*srcPX1).R - dstIng128[x1][y1].R) * ((*srcPX1).R - dstIng128[x1][y1].R)
					+ ((*srcPX1).G - dstIng128[x1][y1].G) * ((*srcPX1).G - dstIng128[x1][y1].G)
					+ ((*srcPX1).B - dstIng128[x1][y1].B) * ((*srcPX1).B - dstIng128[x1][y1].B)
					+ ((*srcPX2).R - dstIng128[x2][y2].R) * ((*srcPX2).R - dstIng128[x2][y2].R)
					+ ((*srcPX2).G - dstIng128[x2][y2].G) * ((*srcPX2).G - dstIng128[x2][y2].G)
					+ ((*srcPX2).B - dstIng128[x2][y2].B) * ((*srcPX2).B - dstIng128[x2][y2].B);
				int nww = ((*srcPX2).R - dstIng128[x1][y1].R) * ((*srcPX2).R - dstIng128[x1][y1].R)
					+ ((*srcPX2).G - dstIng128[x1][y1].G) * ((*srcPX2).G - dstIng128[x1][y1].G)
					+ ((*srcPX2).B - dstIng128[x1][y1].B) * ((*srcPX2).B - dstIng128[x1][y1].B)
					+ ((*srcPX1).R - dstIng128[x2][y2].R) * ((*srcPX1).R - dstIng128[x2][y2].R)
					+ ((*srcPX1).G - dstIng128[x2][y2].G) * ((*srcPX1).G - dstIng128[x2][y2].G)
					+ ((*srcPX1).B - dstIng128[x2][y2].B) * ((*srcPX1).B - dstIng128[x2][y2].B);

				int tempX = dstIng128[x1][y1].bestMX; int tempY = dstIng128[x1][y1].bestMY;
				if (nww < old) {
					dstIng128[x1][y1].bestMX = dstIng128[x2][y2].bestMX; dstIng128[x1][y1].bestMY = dstIng128[x2][y2].bestMY;
					dstIng128[x2][y2].bestMX = tempX; dstIng128[x2][y2].bestMY = tempY;
					output128.at<Vec3b>(x2, y2) = Vec3b((*srcPX2).B, (*srcPX2).G, (*srcPX2).R);
					output128.at<Vec3b>(x1, y1) = Vec3b((*srcPX1).B, (*srcPX1).G, (*srcPX1).R);
				}
			}
		}
		for (int y2 = 0; y2 < 128; y2++) {
			for (int x2 = 0; x2 < 128; x2++) {
				pixel* srcPX1 = &srcIng128[dstIng128[x1][y1].bestMX][dstIng128[x1][y1].bestMY];
				pixel* srcPX2 = &srcIng128[dstIng128[x2][y2].bestMX][dstIng128[x2][y2].bestMY];
				int old = ((*srcPX1).R - dstIng128[x1][y1].R) * ((*srcPX1).R - dstIng128[x1][y1].R)
					+ ((*srcPX1).G - dstIng128[x1][y1].G) * ((*srcPX1).G - dstIng128[x1][y1].G)
					+ ((*srcPX1).B - dstIng128[x1][y1].B) * ((*srcPX1).B - dstIng128[x1][y1].B)
					+ ((*srcPX2).R - dstIng128[x2][y2].R) * ((*srcPX2).R - dstIng128[x2][y2].R)
					+ ((*srcPX2).G - dstIng128[x2][y2].G) * ((*srcPX2).G - dstIng128[x2][y2].G)
					+ ((*srcPX2).B - dstIng128[x2][y2].B) * ((*srcPX2).B - dstIng128[x2][y2].B);
				int nww = ((*srcPX2).R - dstIng128[x1][y1].R) * ((*srcPX2).R - dstIng128[x1][y1].R)
					+ ((*srcPX2).G - dstIng128[x1][y1].G) * ((*srcPX2).G - dstIng128[x1][y1].G)
					+ ((*srcPX2).B - dstIng128[x1][y1].B) * ((*srcPX2).B - dstIng128[x1][y1].B)
					+ ((*srcPX1).R - dstIng128[x2][y2].R) * ((*srcPX1).R - dstIng128[x2][y2].R)
					+ ((*srcPX1).G - dstIng128[x2][y2].G) * ((*srcPX1).G - dstIng128[x2][y2].G)
					+ ((*srcPX1).B - dstIng128[x2][y2].B) * ((*srcPX1).B - dstIng128[x2][y2].B);

				int tempX = dstIng128[x1][y1].bestMX; int tempY = dstIng128[x1][y1].bestMY;
				if (nww < old) {
					dstIng128[x1][y1].bestMX = dstIng128[x2][y2].bestMX; dstIng128[x1][y1].bestMY = dstIng128[x2][y2].bestMY;
					dstIng128[x2][y2].bestMX = tempX; dstIng128[x2][y2].bestMY = tempY;
					output128.at<Vec3b>(x2, y2) = Vec3b((*srcPX2).B, (*srcPX2).G, (*srcPX2).R);
					output128.at<Vec3b>(x1, y1) = Vec3b((*srcPX1).B, (*srcPX1).G, (*srcPX1).R);
				}
			}
		}
		cv::resize(output128, outshow, Size(SHOWSIZEX, SHOWSIZEY), INTER_LINEAR);
		imshow("Display Image", outshow);
		waitKey(1);
		iteration128--;
	}

	//-----------------------------HSV Preparing 1024-------------------------------
	
	Mat dst1024;
	cv::resize(dst, dst1024, Size(1024, 1024), INTER_LINEAR);
	Mat output1024;
	cv::resize(output128,output1024, Size(1024, 1024), INTER_LINEAR);
	//Mat output1024 = dst1024.clone();
	/*
	Mat output1024HSV;
	cvtColor(output1024, output1024HSV, COLOR_BGR2HSV);
	Mat output128UP1024;
	cv::resize(output128, output128UP1024, Size(1024, 1024), INTER_CUBIC);
	cvtColor(output128UP1024, output128UP1024, COLOR_BGR2HSV);
	for (int i = 0; i < 1024; i++) {
		for (int j = 0; j < 1024; j++) {
			Vec3b upPX = output128UP1024.at<Vec3b>(i, j);
			output1024HSV.at<Vec3b>(i, j)[0] = upPX[0];
			output1024HSV.at<Vec3b>(i, j)[1] = upPX[1];
		}
	}
	cvtColor(output1024HSV, output1024, COLOR_HSV2BGR);*/

	//pixel srcIng1024[1024][1024];
	for (int i = 0; i < 1024; i++) {
		for (int j = 0; j < 1024; j++) {
			Vec3b px = output1024.at<Vec3b>(i, j);
			srcIng1024[i][j].B = (int)px[0];
			srcIng1024[i][j].G = (int)px[1];
			srcIng1024[i][j].R = (int)px[2];
		}
	}
	//pixel dstIng1024[1024][1024];
	for (int i = 0; i < 1024; i++) {
		for (int j = 0; j < 1024; j++) {
			Vec3b px = dst1024.at<Vec3b>(i, j);
			dstIng1024[i][j].B = (int)px[0];
			dstIng1024[i][j].G = (int)px[1];
			dstIng1024[i][j].R = (int)px[2];
		}
	}
	for (int sy = 0; sy < 1024; sy++) {
		for (int sx = 0; sx < 1024; sx++) {
			srcIng1024[sx][sy].bestMX = sx;
			dstIng1024[sx][sy].bestMX = sx;
			srcIng1024[sx][sy].bestMY = sy;
			dstIng1024[sx][sy].bestMY = sy;
		}
	}
	//imshow("Display Image", output1024);

	//-----------------------------PROCESSING 1024-------------------------------
	//*
	cv::resize(output1024, outshow, Size(SHOWSIZEX, SHOWSIZEY), INTER_LINEAR);

	int iteration1024 = 2048;
	while (iteration1024 > 0) {
		int x1 = rand() % 1024;
		int y1 = rand() % 1024;
		for (int x2 = 0; x2 < 1024; x2++) {
			for (int y2 = 0; y2 < 1024; y2++) {
				pixel* srcPX1 = &srcIng1024[dstIng1024[x1][y1].bestMX][dstIng1024[x1][y1].bestMY];
				pixel* srcPX2 = &srcIng1024[dstIng1024[x2][y2].bestMX][dstIng1024[x2][y2].bestMY];
				int old = ((*srcPX1).R - dstIng1024[x1][y1].R) * ((*srcPX1).R - dstIng1024[x1][y1].R)
					+ ((*srcPX1).G - dstIng1024[x1][y1].G) * ((*srcPX1).G - dstIng1024[x1][y1].G)
					+ ((*srcPX1).B - dstIng1024[x1][y1].B) * ((*srcPX1).B - dstIng1024[x1][y1].B)
					+ ((*srcPX2).R - dstIng1024[x2][y2].R) * ((*srcPX2).R - dstIng1024[x2][y2].R)
					+ ((*srcPX2).G - dstIng1024[x2][y2].G) * ((*srcPX2).G - dstIng1024[x2][y2].G)
					+ ((*srcPX2).B - dstIng1024[x2][y2].B) * ((*srcPX2).B - dstIng1024[x2][y2].B);
				int nww = ((*srcPX2).R - dstIng1024[x1][y1].R) * ((*srcPX2).R - dstIng1024[x1][y1].R)
					+ ((*srcPX2).G - dstIng1024[x1][y1].G) * ((*srcPX2).G - dstIng1024[x1][y1].G)
					+ ((*srcPX2).B - dstIng1024[x1][y1].B) * ((*srcPX2).B - dstIng1024[x1][y1].B)
					+ ((*srcPX1).R - dstIng1024[x2][y2].R) * ((*srcPX1).R - dstIng1024[x2][y2].R)
					+ ((*srcPX1).G - dstIng1024[x2][y2].G) * ((*srcPX1).G - dstIng1024[x2][y2].G)
					+ ((*srcPX1).B - dstIng1024[x2][y2].B) * ((*srcPX1).B - dstIng1024[x2][y2].B);

				int tempX = dstIng1024[x1][y1].bestMX; int tempY = dstIng1024[x1][y1].bestMY;
				if (nww < old) {
					dstIng1024[x1][y1].bestMX = dstIng1024[x2][y2].bestMX; dstIng1024[x1][y1].bestMY = dstIng1024[x2][y2].bestMY;
					dstIng1024[x2][y2].bestMX = tempX; dstIng1024[x2][y2].bestMY = tempY;
					output1024.at<Vec3b>(x2, y2) = Vec3b((*srcPX2).B, (*srcPX2).G, (*srcPX2).R);
					output1024.at<Vec3b>(x1, y1) = Vec3b((*srcPX1).B, (*srcPX1).G, (*srcPX1).R);
				}
			}
		}
		for (int y2 = 0; y2 < 1024; y2++) {
			for (int x2 = 0; x2 < 1024; x2++) {
				pixel* srcPX1 = &srcIng1024[dstIng1024[x1][y1].bestMX][dstIng1024[x1][y1].bestMY];
				pixel* srcPX2 = &srcIng1024[dstIng1024[x2][y2].bestMX][dstIng1024[x2][y2].bestMY];
				int old = ((*srcPX1).R - dstIng1024[x1][y1].R) * ((*srcPX1).R - dstIng1024[x1][y1].R)
					+ ((*srcPX1).G - dstIng1024[x1][y1].G) * ((*srcPX1).G - dstIng1024[x1][y1].G)
					+ ((*srcPX1).B - dstIng1024[x1][y1].B) * ((*srcPX1).B - dstIng1024[x1][y1].B)
					+ ((*srcPX2).R - dstIng1024[x2][y2].R) * ((*srcPX2).R - dstIng1024[x2][y2].R)
					+ ((*srcPX2).G - dstIng1024[x2][y2].G) * ((*srcPX2).G - dstIng1024[x2][y2].G)
					+ ((*srcPX2).B - dstIng1024[x2][y2].B) * ((*srcPX2).B - dstIng1024[x2][y2].B);
				int nww = ((*srcPX2).R - dstIng1024[x1][y1].R) * ((*srcPX2).R - dstIng1024[x1][y1].R)
					+ ((*srcPX2).G - dstIng1024[x1][y1].G) * ((*srcPX2).G - dstIng1024[x1][y1].G)
					+ ((*srcPX2).B - dstIng1024[x1][y1].B) * ((*srcPX2).B - dstIng1024[x1][y1].B)
					+ ((*srcPX1).R - dstIng1024[x2][y2].R) * ((*srcPX1).R - dstIng1024[x2][y2].R)
					+ ((*srcPX1).G - dstIng1024[x2][y2].G) * ((*srcPX1).G - dstIng1024[x2][y2].G)
					+ ((*srcPX1).B - dstIng1024[x2][y2].B) * ((*srcPX1).B - dstIng1024[x2][y2].B);

				int tempX = dstIng1024[x1][y1].bestMX; int tempY = dstIng1024[x1][y1].bestMY;
				if (nww < old) {
					dstIng1024[x1][y1].bestMX = dstIng1024[x2][y2].bestMX; dstIng1024[x1][y1].bestMY = dstIng1024[x2][y2].bestMY;
					dstIng1024[x2][y2].bestMX = tempX; dstIng1024[x2][y2].bestMY = tempY;
					output1024.at<Vec3b>(x2, y2) = Vec3b((*srcPX2).B, (*srcPX2).G, (*srcPX2).R);
					output1024.at<Vec3b>(x1, y1) = Vec3b((*srcPX1).B, (*srcPX1).G, (*srcPX1).R);
				}
			}
		}
		cv::resize(output1024, outshow, Size(SHOWSIZEX, SHOWSIZEY), INTER_LINEAR);
		imshow("Display Image", outshow);
		waitKey(1);
		//iteration1024--;
	}//*/

    //imshow("Display Image", output);
    waitKey(0);
    destroyAllWindows();

    return 0;
}