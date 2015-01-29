#ifndef _GRID_HEADER_
#define _GRID_HEADER_

#include <opencv2/opencv.hpp>

#define GRID_NOTOBJ -1

/*struct to divide the frame and find objects*/
class grid
{
   public:
	int divx,                                 //number of vertical divisions of frame
		 divy,											//number of horizontal divisions 
	    secs,                                 //size of the sides of the square
       maxwp;                                //number of whitepixels of a sector for the particle to be considered 

	grid(const cv::Mat&,int nmax=32);                 //constructor
   int mark(cv::Mat&,const cv::Scalar color=cv::Scalar(0,0,255));
   int locate(const cv::Mat&,cv::Range[][2],int nmax=32);
};

//AUXILIAR CLASSES
/*candidate to be an object*/
struct cand
{
	cv::Range x,y; 			                        //borders of the object
	cv::Point pos; 			                        //center of the object
};

/*struct used to find object shapes*/
struct Line
{
	cand* idate;                              //pointer to candidate
	cv::Range line;			                  //range of whitepixels line
};

#endif
