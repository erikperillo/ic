#ifndef _PTCOL_HEADER_
#define _PTCOL_HEADER_

#include <opencv2/opencv.hpp>

#define _PTCOL_LEN(x) (sizeof(x)/sizeof(x[0]))

struct ptColArgs
{
   int npoints;
   bool newclick,fini;
   cv::Point* points;
   ptColArgs(cv::Point* pt=NULL): points(pt), npoints(0), newclick(false), fini(false){;}
};

void ptCol(int event,int x,int y,int flags,void* param);

#endif
