#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;


int col, row;

void sharpen(Mat src, Mat dst)
{
	const int nChannels = src.channels();
	dst.create(src.size(), src.type());

	for (int j = 1; j < src.rows - 1; ++j)
	{
		const uchar*pre = src.ptr<uchar>(j - 1);
		const uchar*cur = src.ptr<uchar>(j);
		const uchar*next = src.ptr<uchar>(j + 1);
		uchar* output = dst.ptr<uchar>(j);

		for (int i = nChannels; i < nChannels*(src.cols - 1); ++i)
		{
			*output++ = saturate_cast<uchar>(5 * cur[i] - cur[i - nChannels] - cur[i + nChannels] - pre[i] - next[i]);
		}

	}

	dst.row(0).setTo(Scalar(0));
	dst.row(dst.rows - 1).setTo(Scalar(0));
	dst.col(0).setTo(Scalar(0));
	dst.col(dst.cols - 1).setTo(Scalar(0));
}


void rot(Mat img)
{
	Mat rot;
	
	Point center = Point(257, 410);
	double angle = 8.5;
	double scale = 1;
	
	Mat rot_mat = getRotationMatrix2D(center, angle, scale);
	
	warpAffine(img, rot, rot_mat, img.size());

	imshow("rot", rot);

	Mat roi(rot, Rect(37, 29, 689, 398));
	imshow("roi", roi);
}

void affine(Mat img)
{
	Point2f src[3];
	Point2f dst[3];

	Mat warp_dst = Mat::zeros(img.rows, img.cols, img.type());

	src[0] = Point2f(104, 21);
	src[1] = Point2f(776, 103);
	src[2] = Point2f(37, 394);

	dst[0] = Point2f(0, 0);
	dst[1] = Point2f(677, 0);
	dst[2] = Point2f(0, 379);

	Mat warp_mat = getAffineTransform(src, dst);
	warpAffine(img, warp_dst, warp_mat, warp_dst.size());

	imshow("affine", warp_dst);


}

void perspect(Mat img)
{
	Point2f src[4];
	Point2f dst[4];

	src[0] = Point2f(109, 22);
	src[1] = Point2f(773, 105);
	src[2] = Point2f(39, 394);
	src[3]= Point2f(712, 493);

	dst[0] = Point2f(0, 0);
	dst[1] = Point2f(670, 0);
	dst[2] = Point2f(0, 379);
	dst[3] = Point2f(670, 379);

	Mat warp_mat = getPerspectiveTransform(src, dst);
	Mat warped;
	warpPerspective(img, warped, warp_mat, warped.size(), INTER_LINEAR, BORDER_CONSTANT);
	
	Mat opt,shp_1,shp_2;
	//medianBlur(warped, opt, 5);
	bilateralFilter(warped, opt, 20, 200, 50);
	shp_1 = Mat::zeros(opt.rows, opt.cols, opt.type());
	sharpen(opt, shp_1);
	
	Mat roi(shp_1, Rect(0, 0, 670, 379));
	//imshow("perspect", warped);
	//imshow("opt", opt);
	//imshow("shp_1", shp_1);
	imshow("roi", roi);
	 
}

void onMouse(int Event, int x, int y, int flags, void* param) {
	

	if (Event == CV_EVENT_LBUTTONDOWN) {
		cout <<"("<<y<<" , "<<x<<") " << endl;
	}
} 




int main()
{
	Mat img;
	img=imread("hw2_original_image.jpg",IMREAD_COLOR);
	
	int flag = 0;

	//affine(img);
	perspect(img);

	imshow("img", img);
	
	setMouseCallback("perspect", onMouse, NULL); //change window!!!
	
	waitKey(1);

	std::cout << "hello!!!" << std::endl;
	waitKey(0);
	return 0;

}

