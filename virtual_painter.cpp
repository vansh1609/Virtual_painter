#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
using namespace std;
using namespace cv;

//PROJECT 1 VIRTUAL PAINTER//


vector<vector<int>> myColors{ {124,48,117,143,170,255},//purple
							{68,72,156,102,126,255} };//green

vector<Scalar> myColorValues{ {255,0,255},//purple
							{0,255,0} };//green

vector<vector<int>> mypoints;
Point getContours(Mat imgDil, Mat img)
{
	vector<vector<Point>> contours;

	vector<Vec4i> heirarchy;
	findContours(imgDil, contours, heirarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundingrect(contours.size());
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 5);
	Point mypoint(0, 0);
	for (int i = 0;i < contours.size();i++)
	{

		int objcorner;
		string type;
		int area = contourArea(contours[i]);
		if (area > 1000)
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			cout << conPoly[i].size() << endl;
			boundingrect[i] = boundingRect(conPoly[i]);
			mypoint.x = boundingrect[i].x + boundingrect[i].width / 2;
			mypoint.y = boundingrect[i].y;
			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			rectangle(img, boundingrect[i].tl(), boundingrect[i].br(), Scalar(0, 255, 0), 2);

		}
	}
	return mypoint;
}

void findColor(Mat img)
{
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	for (int i = 0;i < myColors.size();i++)
	{
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);
		imshow(to_string(i), mask);
		Point mypoint = getContours(mask, img);
		if (mypoint.x != 0 && mypoint.y != 0)
			mypoints.push_back({ mypoint.x,mypoint.y,i });
	}
}
void drawoncanvas(Mat img)
{
	for (int i = 0;i < mypoints.size();i++)
		circle(img, Point(mypoints[i][0], mypoints[i][1]), 10, myColorValues[mypoints[i][2]], FILLED);
}
int main()
{
	VideoCapture vc(0);
	Mat img;
	while (true)
	{
		vc.read(img);
		findColor(img);
		drawoncanvas(img);
		imshow("image", img);
		waitKey(1);
	}
	return 0;
}
