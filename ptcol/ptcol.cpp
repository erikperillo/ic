#include "ptcol.hpp"
#include <iostream>

using namespace std;

void ptCol(int event,int x,int y,int flags,void* param)
{
   ptColArgs * args = (ptColArgs*)param;
   
   if(event == CV_EVENT_LBUTTONDOWN && !args->fini)
   {
      args->newclick = true;
      args->points[args->npoints] = cv::Point(x,y);
      args->npoints ++;
   }
}

