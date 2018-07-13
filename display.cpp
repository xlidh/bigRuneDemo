#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <X11/Xlib.h>
#include <time.h>
#include <vector>

using namespace std;
using namespace cv;

Mat fire[9];
Mat segment[10];
Mat blank;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
}
 


class Board{
private:
	Mat bg;
	int height, width;
	Rect indicator[2][5];
	Rect five[5];
	Rect nine[3][3];
public:
	Board(int h, int w){
		namedWindow("Board", CV_WINDOW_NORMAL);
		setWindowProperty("Board",CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
		setMouseCallback("Board", CallBackFunc, NULL);
		height = h;
		width = w;
		bg = Mat::zeros(height, width, CV_8UC3);
		//Indicator
		for (int a = 0; a < 2; a++)
		{
			for (int b = 0; b < 5; b++)
			{
				indicator[a][b] = Rect(round((100+1600*a)/1960.0*w), round((235+160*b)/1080.0*h),
										round(170/1960.0*w), round(80/1080.0*h));
				//rectangle(bg, indicator[a][b], Scalar(255,255,255), 2);
				//cout << indicator[a][b].width << "   "<<indicator[a][b].height << endl;
			}
		}
		//Fireimg
		for (int a = 0; a < 3; a++)
		{
			for (int b = 0; b < 3; b++)
			{
				nine[a][b] = Rect(round((360+450*a)/1960.0*w), round((235+260*b)/1080.0*h),
										round(350/1960.0*w), round(200/1080.0*h));
				//rectangle(bg, nine[a][b], Scalar(0,255,255), 2);
				//cout << nine[a][b].width << "   "<<nine[a][b].height << endl;
			}
		}
		//7Segment
		for (int a = 0; a < 5; a++)
		{
			five[a] = Rect(round((630+140*a)/1960.0*w), 30,
									round(120/1960.0*w), round(160/1080.0*h));
			//rectangle(bg, five[a], Scalar(255,0,0), 2);
			//cout << five[a].width << "   "<<five[a].height << endl;
		}
		showIndicator(blank);

	}

	void randNine(Mat input[9])
	{
		vector<int> sub;
		for(int i = 0; i < 9; i++)
			sub.push_back(i);
		//A faster method
		random_shuffle(sub.begin(), sub.end());

		for(int i = 0; i < 9; i++)
		{
			// srand(time(0));
			// int r = rand() % 9;
			// while (sub[r] == -1)
			// {
			// 	r = (r*2) % 9;
			// 	cout << "Looping"<<endl;
			// }

			//then

			Mat temp;
			resize(input[sub[i]], temp, nine[i/3][i%3].size());
			temp.copyTo(bg(nine[i/3][i%3]));

		}
	}

	void randFive(Mat input[10])
	{
		vector<int> sub;
		for(int i = 0; i < 10; i++)
			sub.push_back(i);
		//A faster method
		random_shuffle(sub.begin(), sub.end());

		for(int i = 0; i < 5; i++)
		{
			//then
			Mat temp;
			resize(input[sub[i]], temp, five[i].size());
			temp.copyTo(bg(five[i]));

		}
	}

	void showIndicator(Mat input)
	{	
		Mat temp;
		resize(input, temp, indicator[0][0].size());
		for(int a = 0; a < 2; a++)
		{
			for(int b = 0; b < 5; b++)
			{
				temp.copyTo(bg(indicator[a][b]));
			}
		}
	}

	void update(int time){
		randNine(fire);
		randFive(segment);
		imshow("Board", bg);
		waitKey(time);
		cout << "Update" << endl;
	}

};


void getScreenInfo(int &height, int &width)
{
	Display* d = XOpenDisplay(NULL);
	Screen*  s = DefaultScreenOfDisplay(d);
	height = s->height;
	width  = s->width;
}

bool loadImg()
{
	for (int f = 0; f < 9; f++)
	{	
		string folder = "./fire/";
		string target = to_string(f+1).append(".jpg");
		fire[f] = imread(folder.append(target));
	}

	cout << "-- FireImg loaded" << endl;

	for (int s = 0; s < 10; s++)
	{
		string folder = "./segment/";
		string target = to_string(s).append(".png");
		segment[s] = imread(folder.append(target));
	}
	cout << "-- SegmentImg loaded" << endl;

	blank = imread("indicator.png");
	cout << "-- Indicator loaded" << endl;
	return true;
}


int main(int argc, char* argv[])
{
	int time = 1500;
	if (argc == 2)
	{
		istringstream ( argv[1] ) >> time;
	}
	int height, width;
	getScreenInfo(height, width);
	loadImg();
	Board board(height, width);
	while(true)
		board.update(time);
	
	return 0;
}

