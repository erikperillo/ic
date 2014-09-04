#ifndef _BLOB_HEADER_
#define _BLOB_HEADER_

#include "opencv2/opencv.hpp"
#include <iostream>

//blob class
class blob
{
   public:
   //demarks where the object starts and ends
   cv::Range x,y;
   //methods
   blob(): x(cv::Range(0,0)), y(cv::Range(0,0)){;}
   blob(const cv::Range& _x,const cv::Range& _y): x(_x), y(_y) {;}        //constructor
   cv::Point point();                                                     //returns central point
   int area();                                                            //calculates area of object
   int mark(cv::Mat&,const cv::Scalar& color=cv::Scalar(0,0,255));        //marks object in image
   friend std::ostream& operator<<(std::ostream&,blob&);                  //operator
};

void orderBlob(blob[],const int&);

#endif
