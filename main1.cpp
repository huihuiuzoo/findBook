#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

int lowThreshold = 50;
int ratio = 3.5;
int kernel_size = 3;

/** @function main */
int main( int argc, char** argv )
{

  Mat src = imread("/home/hui/study/findPolygon/build/test4.jpg",0);
  imshow("orign",src);

  //blur(src, src, Size(9, 9));
  GaussianBlur(src, src, Size(3,3), 0,0 );
  Mat dst;
  equalizeHist(src, dst);
  imshow("src",src);
  imshow("after blur",dst);

  //canny
  Canny( dst, dst, lowThreshold, lowThreshold*ratio, kernel_size );
  Mat dst_copy;
  dst_copy = Scalar::all(0);
  src.copyTo( dst_copy, dst);
  imshow( "canny", dst_copy );

//hough

  //adaptive threshold
  int blockSize = 25;
  int constValue = 10;
  Mat src_copy = src.clone();
  cv::adaptiveThreshold(src, src_copy, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);

  Mat dst_Open = src.clone();
  //膨胀 腐蚀 opening operation
  morphologyEx( src_copy, dst_Open ,MORPH_OPEN,NULL,Point(-1,-1),1,BORDER_CONSTANT,morphologyDefaultBorderValue() );
  imshow("after open operation",dst_Open );

  /// 得到轮廓
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  findContours(src_copy, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

  cout<<"the size of contours is : "<<(int)contours.size()<<endl;
  cout<<"the size of contours[0] is : "<<(int)contours[0].size()<<endl;

  for(int j = 0; j < 48;j++)
  for(int i = 0; i < 4;i++)
  {

  cout<<contours[j][i]<<endl;
  }

  /// 计算轮廓的距离
  Mat raw_dist( src.size(), CV_32FC1 );

  for( int j = 0; j < src.rows; j++ )
  for( int i = 0; i < src.cols; i++ )
  {
  raw_dist.at<float>(j,i) = pointPolygonTest( contours[0], Point2f(i,j), true );
  }

  double minVal;
  double maxVal;
  minMaxLoc( raw_dist, &minVal, &maxVal, 0, 0, Mat() );
  minVal = abs(minVal);
  maxVal = abs(maxVal);

  /// 图形化的显示距离
  Mat drawing = Mat::zeros( src.size(), CV_8UC3 );

  for( int j = 0; j < src.rows; j++ )
  for( int i = 0; i < src.cols; i++ )
  {
  if( raw_dist.at<float>(j,i) < 0 )
  {
  drawing.at<Vec3b>(j,i)[0] = 255 - (int) abs(raw_dist.at<float>(j,i))*255/minVal;
  }
  else if( raw_dist.at<float>(j,i) > 0 )
  {
  drawing.at<Vec3b>(j,i)[2] = 255 - (int) raw_dist.at<float>(j,i)*255/maxVal;
  }
  else
  {
  drawing.at<Vec3b>(j,i)[0] = 255;
  drawing.at<Vec3b>(j,i)[1] = 255;
  drawing.at<Vec3b>(j,i)[2] = 255;
  }
  }

  /// 创建窗口显示结果
  char* source_window = "Source";
  //namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  //imshow( source_window, src );
  namedWindow( "Distance", CV_WINDOW_AUTOSIZE );
  imshow( "Distance", drawing );

  waitKey(0);/**/
  return(0);
}
