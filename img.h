#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>

#ifndef _img_h_
#define _img_h_


class pixel {
public:
	int R;
	int G;
	int B;
	int bestMX;
	int bestMY;
};

#endif