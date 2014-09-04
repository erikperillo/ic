#include <iostream>
#include "blob.hpp"

using namespace std;
using namespace cv;

int main()
{
   blob bl0(Range(1,2),Range(1,7)),bl1(Range(50,150),Range(80,190)),bl2(Range(2,5),Range(8,11)),bl3(Range(15,21),Range(18,41));
   blob array[] = {bl0,bl1,bl2,bl3};

   Mat img = imread("test8.jpg");

   for(int i=0;i<4;i++)
      cout << array[i] << "," << array[i].area() << endl;
   orderBlob(array,4);
   cout << "ord..." << endl;
   for(int i=0;i<4;i++)
      cout << array[i] << "," << array[i].area() << endl;

   return 0;  

   namedWindow("img",CV_WINDOW_AUTOSIZE);

   imshow("img",img);
   waitKey(0);

   cout << bl2 << endl;

   bl2.mark(img,Scalar(0,0,255));
   imshow("img",img);
   waitKey(0);

   return 0;
}
