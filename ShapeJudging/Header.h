#pragma once
#include<vector>
#include<opencv2\opencv.hpp>
using namespace std;
using namespace cv;

class Item {
public:
	Item(const Mat inImage);
	~Item();
	bool IsShirtFolded();
	bool IsTrousersFolded();
private:
	void FindContours(vector<vector<Point>>& contours, int& index, double& itemArea);
	void FindRotatedRect(const vector<vector<Point>> contours, const int index, double& rectArea);
	void FindRotatedRect(const vector<vector<Point>> contours, const int index, float& angle);
	void FindConvexHull(const vector<vector<Point>> contours, const int index, double& rectArea);
	Mat mImage;
};

Item::Item(const Mat inImage) 
	:mImage(inImage) {}

Item::~Item() {}

void Item::FindContours(vector<vector<Point>>& contours, int& index, double& itemArea) {
	Mat grayImage, binaryImage;
	cvtColor(mImage, grayImage, CV_RGB2GRAY);
	threshold(grayImage, binaryImage, 240, 255, CV_THRESH_BINARY_INV);
	findContours(binaryImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	int size = 0;
	for (int i = 0; i < contours.size(); i++)
		if (contours[i].size() > size) size = contours[i].size(), index = i;
	itemArea = contourArea(contours[index]);
}

void Item::FindRotatedRect(const vector<vector<Point>> contours, const int index, double& rectArea) {
	RotatedRect rect = minAreaRect(contours[index]);
	rectArea = rect.size.area();
}

void Item::FindRotatedRect(const vector<vector<Point>> contours, const int index, float& angle) {
	RotatedRect rect = minAreaRect(contours[index]);
	angle = fabs(rect.angle);
}

void Item::FindConvexHull(const vector<vector<Point>> contours, const int index, double& hullArea) {
	vector<Point> hull;

	convexHull(contours[index], hull);
	hullArea = contourArea(hull);
}

bool Item::IsShirtFolded() {
	vector<vector<Point>> contours;
	int index = 0;
	double itemArea = 0.0, rectArea = 0.0;

	FindContours(contours, index, itemArea);
	FindRotatedRect(contours, index, rectArea);
 	if (itemArea / rectArea > 0.9) return true;
	else return false;
}

bool Item::IsTrousersFolded() {
	vector<vector<Point>> contours;
	int index = 0;
	double itemArea = 0.0, hullArea = 0.0;
	float angle;

	FindContours(contours, index, itemArea);
	FindRotatedRect(contours, index, angle);
	FindConvexHull(contours, index, hullArea);
	if (itemArea / hullArea > 0.9 && angle > 5.0) return true;
	else return false;
}