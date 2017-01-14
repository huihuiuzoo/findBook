#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

int lowThreshold = 80;
int ratio = 3.5;
int kernel_size = 3;

void preProcess(const Mat &img_in,Mat &img_out)
{

    Mat src;
    if(img_in.channels()==3)
          cvtColor(img_in, src, CV_RGB2GRAY);//把图片转化为灰度图
    else
        src  =  img_in.clone();
    //cout<<"preprocess the image"<<endl;
    //Mat src = imread("/home/hui/study/findPolygon/build/test3.jpg",0);

    //blur(src, src, Size(9, 9));
    GaussianBlur(src, src, Size(3,3), 0, 0 );
    Mat dst;
    equalizeHist(src, dst);  //hist
    //imshow("src",src);
    //imshow("after blur",dst);

    //canny
    Canny( dst, dst, lowThreshold, lowThreshold*ratio, kernel_size );
    Mat dst_copy;
    dst_copy = Scalar::all(0);
    src.copyTo( dst_copy, dst);

    img_out = dst_copy.clone();
    imshow( "canny", dst_copy );
    waitKey(30);

}

void find(const Mat &in,Mat &out )
{

    /// 得到轮廓
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours(in, contours, hierarchy, RETR_TREE, CV_CHAIN_APPROX_TC89_L1);
    cout<<"contours size is : "<<contours.size()<<endl;
    Mat result_image = Mat ::zeros(in.size(),CV_8U);
    drawContours(result_image, contours, -1, Scalar(255, 0, 255));
    imshow( "find contours", result_image );

    double tmp = 0.0;
    for (int i = 0; i < (int)contours.size(); i++)
    {

             cout << "contours size is : "<<(int)contours[i].size()<<endl;

             //cout << i  << "面积：  " << contourArea(contours[i]) << "周长：  " <<arcLength(contours[i], true) << endl;
             if(arcLength(contours[i], true) >tmp)
                 tmp = arcLength(contours[i], true);


    }
    cout<<"the max Perimeter is : "<<tmp<<endl;




}

/** @function main */
int main( int argc, char** argv )
{

    Mat img;
    VideoCapture cap(1); // read from file
    int num = 0;
    //VideoCapture cap(0); // read from camera

    // Check if video is open
    if(!cap.isOpened())
    {
        cout << "Cannot open video/camera!" << endl;
        //return -1;
    }

    // Set up window
    const char* WINDOW = "frame";
    namedWindow(WINDOW, WINDOW_AUTOSIZE);

    // Read frames
    Mat frame;
    int frameNum = 0;


    while(1)
    {


        num++;
        cap >> frame;

        if(num%100==0)
        {
            char s[100];
            sprintf(s, "%d.jpg", 1); //产生"123"
            cout<<"save the image name "<<s<<endl;


            //sprintf(s, "%d.jpg", num); //产生"123"
            //cout<<"save the image name "<<s<<endl;
            imwrite(s,frame);

        }

        if(frame.empty())
        {
            cout << "frame " << frameNum << " is empty!" << endl;
            break;
        }

        imshow(WINDOW, frame);
        int delay = 30; // ms
        int key = waitKey(delay);
        if(27 == key || 'Q' == key || 'q' == key)
            break;

        Mat img_preprocess;
        cout<<"prepare ......"<<endl;
        preProcess(frame,img_preprocess);
        find(img_preprocess,img_preprocess );







    }

}


