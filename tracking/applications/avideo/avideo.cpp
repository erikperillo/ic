#include <iostream>
#include <fstream>
#include <pthread.h>
#include <SFML/Audio.hpp>
#include "opencv2/opencv.hpp"
#include "../blob/blob.hpp"
#include "../grid/grid.hpp"
#include "../ptcol/ptcol.hpp"
#include "../.cfgen/cfgen.hpp"

using namespace std;
using namespace sf;
using namespace cv;

#define NSONGSETS  4
#define NBLOBS    32
#define NPOINTS    4
#define SIZE     500
#define DELAY     10
#define PLAYING    2
#define BUF_SIZE 1024
#define NMAX_SONGS 16

void err(const char* errmsg,int errcode=-1)
{
   cout << errmsg << endl;
   exit(errcode);
}

void gride(Mat& img,Scalar color=Scalar(255,0,0))
{
   img.col((int)img.cols/2) = color;
   img.row((int)img.rows/2) = color;
   return;
}

int main(int argc, char** argv)
{   
   if(argc != 2)
      err("Invalid arguments");

   enum {upl,upr,btl,btr};
   int i,j,randind,nsongs,nblobs,cam_index=0,ind=0,last_ind=0,count=0,
       frame_dim[2] = {SIZE, SIZE}, 
       hsv_low[3] = {0, 0, 0}, 
       hsv_upp[3] = {127, 255, 255};
   float duration;
   char wk, *filename = argv[1]; 
   ifstream file;
   //window name and songs array
   const char* winname = "image", *binname = "binary";
   //vectors of chars
   vector<string> songs[4];
   //color definitions
   Scalar red(0,0,255), green(0,255,0), blue(255,0,0), gray(128,128,128);
   //color hsv values
   Range cands[NBLOBS][2],hsv[3];
   //point
   Point pt(0,0),pts[NPOINTS];
   //points for homography transform
   Point2f _dst[] = {Point2f(0,frame_dim[0]),Point2f(frame_dim[0],frame_dim[1]),Point2f(frame_dim[0],0),Point2f(0,0)};
   vector<Point2f> src,dst;
   //images
   Mat frame,bin,H;
   //object for capturing video
   VideoCapture cap;
   //audio files
   Music music[NSONGSETS][NMAX_SONGS];
   //time for when the music will start
   Time start;
   //blobs
   blob bl[NBLOBS];
   bl[0] = blob(Range(10,20),Range(30,40));
   //arguments for point collection
   ptColArgs pca(pts);
   //config struct
   Conf conf(filename);

   //configuring everything
   conf.acquire("nsongs",1,"%d",&nsongs);
   conf.acquire("cam_index",0,"%d",&cam_index);
   conf.acquire("frame_w",1,"%d",&frame_dim[0]);
   conf.acquire("frame_h",1,"%d",&frame_dim[1]);
   conf.acquire("hsv_low",3,"%d %d %d",&hsv[0].start,&hsv[1].start,&hsv[2].start);
   conf.acquire("hsv_upp",3,"%d %d %d",&hsv[0].end,&hsv[1].end,&hsv[2].end);
   conf.vacquire<string>("songs_upl",nsongs,songs[0]);
   conf.vacquire<string>("songs_upr",nsongs,songs[1]);
   conf.vacquire<string>("songs_btl",nsongs,songs[2]);
   conf.vacquire<string>("songs_btr",nsongs,songs[3]);

   ////////////////////////////////debug
   cout << "Everything configured. Summing up:" << endl;
   cout << "nsongs = " << nsongs << endl;
   cout << "cam_index = " << cam_index << endl;
   cout << "frame_w, frame_h = " << frame_dim[0] << ", " << frame_dim[1] << endl;
   cout << "hsv_low = " << hsv[0].start << ", " << hsv[1].start << ", " << hsv[2].start << endl;
   cout << "hsv_upp = " << hsv[0].end << ", " << hsv[1].end << ", " << hsv[2].end << endl;
   cout << "songs_upl: " << endl;
   for(i=0;i<nsongs;i++)
      cout << "   " << i << " - " << songs[0][i] << endl; 
   cout << "songs_upr: " << endl;
   for(i=0;i<nsongs;i++)
      cout << "   " << i << " - " << songs[1][i] << endl;
   cout << "songs_btl: " << endl;
   for(i=0;i<nsongs;i++)
      cout << "   " << i << " - " << songs[2][i] << endl;
   cout << "songs_btr: " << endl;
   for(i=0;i<nsongs;i++)
      cout << "   " << i << " - " << songs[3][i] << endl; 
   /////////////////////////////////debug  

   //setting random
   srand(time(NULL));

   //converting vector to array
   for(i=0;i<NPOINTS;i++)
      dst.push_back(_dst[i]);

   //initializating audio files 
   for(i=0;i<NSONGSETS;i++)
      for(j=0;j<nsongs;j++)      
         if(!music[i][j].openFromFile(songs[i][j]))
            err("ERROR: One of the songs could not be loaded!");

   //creating window
   namedWindow(winname,CV_WINDOW_AUTOSIZE);

   //opening camera
   cap.open(cam_index);
   if(!cap.isOpened())
      err("ERROR: Camera could not be opened!");

   //initiating streaming for collecting points
   cout << "Press 'p' when ready to collect points." << endl;
   while(true)
   {
      cap >> frame;
   
      imshow(winname,frame);
      if((wk=waitKey(30)) == 'p' || wk == 'P')
         break;
      else if(wk == 27)
      {
         cap.release();
         err("key ESC pressed.");
      }
   }
   
   //setting mouse callback function
   setMouseCallback(winname,ptCol,(void*)&pca);
   
   //collecting points
   cout << "Click on the 4 corners of the square in the order of points (0,0), (L,0), (L,L), (0,L)...";
   while(pca.npoints <= NPOINTS)
   {
      if(pca.newclick) //appends point
      {
         src.push_back(pts[pca.npoints-1]);
         circle(frame,pts[pca.npoints-1],2,Scalar(0,0,255),-1);
         pca.newclick = false;
         if(pca.npoints == NPOINTS)
            break;
      }
      imshow(winname,frame);
      waitKey(100);
   }
   cout << "done." << endl;
   //displaying final result      
   imshow(winname,frame);
   waitKey(0);

   //calculating homography
   H = findHomography(src,dst);   
   //creating grid
   grid dirg(Mat::zeros(frame_dim[1],frame_dim[0],CV_8UC3),64);

   //video loop
   while(true)
   {
      //capturing frame
      cap >> frame;
      if(!frame.data)
         err("ERROR: frame capture error");

      //warping image acording to homography
      warpPerspective(frame,frame,H,Size(frame_dim[0],frame_dim[1]));

      //locating object 
      cvtColor(frame,bin,CV_BGR2HSV);
	   inRange(bin,Scalar(hsv[0].start,hsv[1].start,hsv[2].start),Scalar(hsv[0].end,hsv[1].end,hsv[2].end),bin);
      nblobs = dirg.locate(bin,cands);

      if(nblobs>0)
      {
         //converting ranges to blobs
         for(i=0;i<nblobs;i++)
            bl[i] = blob(cands[i][0],cands[i][1]);
         //ordering blobs
         orderBlob(bl,nblobs);
         //marking object in image
         bl[0].mark(frame);

         //determining which music to play
         if(bl[0].point().x < frame.cols/2 && bl[0].point().y < frame.rows/2)
         {
            cout << "upper left corner!\r" << flush;
            ind = upl;
         }
         else if(bl[0].point().x < frame.cols/2)
         {
            cout << "bottom left corner!\r" << flush;
            ind = btl;
         }
         else if(bl[0].point().y < frame.rows/2)
         {
            cout << "upper right corner!\r" << flush;
            ind = upr;
         }
         else
         {
            cout << "bottom right corner!\r" << flush;
            ind = btr;
         }

         if(ind != last_ind)
            for(i=0; i<nsongs; i++)
               music[last_ind][i].stop();
         else
            for(i=0; i<nsongs; i++)
               if(music[ind][i].getStatus() == PLAYING)
                  break;
         if(i==nsongs)
         {
            randind = rand() % nsongs;
            duration = music[ind][randind].getDuration().asSeconds();
            start = seconds((duration > 30.0)?(duration*0.01*(rand() % 90)):0.0);
            music[ind][randind].setPlayingOffset(start);
            music[ind][randind].play();
         }

         last_ind = ind;
      }
      else     
         for(i=0; i<nsongs; i++)
            music[last_ind][i].stop();      

      //displaying images
      gride(frame);
      imshow(winname,frame);
      imshow(binname,bin);

      if(waitKey(10) == 27)
         break;
   }

   return 0;
}
