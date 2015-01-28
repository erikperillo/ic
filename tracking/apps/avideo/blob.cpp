#include "opencv2/opencv.hpp"

class blob
{
   public:
   //vars
   int width,height;
   cv::Point point;
   //methods
   blob(const cv::Point& pt, const int& w, const int& h): point(pt), width(w), height(h) {;}
   int area();
   void mark(const cv::Mat&);
};

int main()
{
   return 0;
}

