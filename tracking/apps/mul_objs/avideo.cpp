#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include "../../blob/blob.hpp"
#include "../../grid/grid.hpp"
#include "../../ptcol/ptcol.hpp"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
using namespace sf;

#define NSONGS		4
#define NOTOBJ		-1
#define NBLOBS		32
#define NPOINTS	4
#define SIZE		500
#define DELAY		10
#define PLAYING	2

#define LEN(x) (sizeof(x)/sizeof(x[0]))

///////////////////////////////TEST
#define HUE 0
#define SAT 1
#define VAL 2

#define FRAME_W 0
#define FRAME_H 1

#define NMAX_OBJS 16
#define UPL_SONG 0
#define UPR_SONG 1
#define BTL_SONG 2
#define BTR_SONG 3

#define BUF_SIZE 1024
#define NMAX_SONGS 16
 
#define NAREAS 4

struct Args
{
	Range hsv[NMAX_OBJS][3];
	int nsongs,nobjs,cam_index,frame_dim[2];
	char songs[NMAX_OBJS][NAREAS][NMAX_SONGS][BUF_SIZE];
	bool randomize;
 	Args(): randomize(false), nsongs(1), nobjs(1), cam_index(0) 
	{
		frame_dim[0] = frame_dim[1] = SIZE;
	}
};

int readConf(Args& args,const char* filename)
{
	char line[BUF_SIZE],*word,*line_args,garbage[100],obj_str[256]; 
	const char *delim = "	 \n";
	int count,i,nargs=0,obj_count=0;
	bool nsdef=false;
	FILE * file = fopen(filename,"r");

	if(file==NULL)
	return -1;

	while(fgets(line,BUF_SIZE,file)!=NULL)
	{	
		word = strtok(line,delim);

		if(word==NULL)
			continue;

		if(word[0]=='#')
			continue;

		else if(strcmp(word,"cam_index:")==0)
		{
			line_args = strtok(NULL,"\n");
			if(sscanf(line_args,"%d",&args.cam_index) != 1)
				return -1;
		}
		else if(strcmp(word,"frame_w:")==0)
		{
			line_args = strtok(NULL,"\n");
			if(sscanf(line_args,"%d",&args.frame_dim[0]) != 1)
				return -1;
		}
		else if(strcmp(word,"frame_h:")==0)
		{
			line_args = strtok(NULL,"\n");
			if(sscanf(line_args,"%d",&args.frame_dim[1]) != 1)
				return -1;
		}
		else if(strcmp(word,"randomize_songs:")==0)
		{
			line_args = strtok(NULL,"\n");
			if(sscanf(line_args,"%d",&args.randomize) != 1)
				return -1;
		}
		else if(strcmp(word,"nsongs:")==0)
		{
			if(nsdef)
				return -1;

			line_args = strtok(NULL,"\n");
			if(sscanf(line_args,"%d",&args.nsongs) != 1)
				return -1;
			if(args.nsongs > NMAX_SONGS)
				return -1;
			nsdef = true;
			nargs++;
		}
		else if(strcmp(word,"OBJECT")==0)
			obj_count++;
		else if(obj_count < 1 || obj_count > NMAX_OBJS)
			return -1;
		else if(strcmp(word,"hsv_low:")==0)
		{
			line_args = strtok(NULL,"\n");
			if(sscanf(line_args,"%d %d %d",&args.hsv[obj_count-1][0].start,&args.hsv[obj_count-1][1].start,&args.hsv[obj_count-1][2].start) != 3)
				return -1;
			nargs++;	
		}		
		else if(strcmp(word,"hsv_upp:")==0)
		{
			line_args = strtok(NULL,"\n");
			if(sscanf(line_args,"%d %d %d",&args.hsv[obj_count-1][0].end,&args.hsv[obj_count-1][1].end,&args.hsv[obj_count-1][2].end) != 3)
				return -1;
			nargs++;	
		}
		else if(strcmp(word,"songs_upl:")==0)
		{
			nsdef = true;

			for(i=0,word=strtok(NULL,delim); i<args.nsongs && word!=NULL; i++,word=strtok(NULL,delim))
			{
				if(sscanf(word,"%s",args.songs[obj_count-1][0][i]) != 1)
					return -1;
			}
			if(i != args.nsongs)
				return -1;
			nargs++;
		}
		else if(strcmp(word,"songs_upr:")==0)
		{
			nsdef = true;

			for(i=0,word=strtok(NULL,delim); i<args.nsongs && word!=NULL; i++,word=strtok(NULL,delim))
			{
				if(sscanf(word,"%s",args.songs[obj_count-1][1][i]) != 1)
					return -1;
			}
			if(i != args.nsongs)
				return -1;
			nargs++;
		}
		else if(strcmp(word,"songs_btl:")==0)
		{
			nsdef = true;
		
			for(i=0,word=strtok(NULL,delim); i<args.nsongs && word!=NULL; i++,word=strtok(NULL,delim))
			{
				if(sscanf(word,"%s",args.songs[obj_count-1][2][i]) != 1)
					return -1;
			}
			if(i != args.nsongs)
				return -1;
			nargs++;
		}
		else if(strcmp(word,"songs_btr:")==0)
		{
			nsdef = true;

			for(i=0,word=strtok(NULL,delim); i<args.nsongs && word!=NULL; i++,word=strtok(NULL,delim))
			{
				if(sscanf(word,"%s",args.songs[obj_count-1][3][i]) != 1)
					return -1;
			}
			if(i != args.nsongs)
				return -1;
			nargs++;
		}
	}
	args.nobjs = obj_count;
	return ((nargs<args.nobjs*(2+args.nsongs*4))?-1:1);
}
/////////////////////////////////TEST

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

	//parameters
	Args params;
	//reading configuration file
	if(readConf(params,argv[1]) < 0)
		err("Error while configuring!");
	
	//debug
	cout << "PROGRAM PARAMETERS RESUME:" << endl;
	cout << "randomize_songs: " << params.randomize << endl;
	cout << "cam_index: " << params.cam_index << endl;
	cout << "nsongs: " << params.nsongs << endl;
	cout << "frame w,h: " << params.frame_dim[0] << "," << params.frame_dim[1] << endl;
	for(int i=0; i<params.nobjs; i++)
	{
		cout << "object " << i+1 << ":" << endl;
		cout << "\thsv_low = " << params.hsv[i][0].start << "," << params.hsv[i][1].start << "," << params.hsv[i][2].start << endl;
		cout << "\thsv_upp =  " << params.hsv[i][0].end << "," << params.hsv[i][1].end << "," << params.hsv[i][2].end << endl; 
		for(int j=0; j<NAREAS; j++)
			for(int k=0; k<params.nsongs; k++)
				cout << "\tsong[" << j << "][" << k << "]: " << params.songs[i][j][k] << endl;
	}
	cout << endl;
	/////////////test
/*
	for(int ct=0; ct<4; ct++) 
	for(int k=0;k<params.nsongs;k++)
	{
	}  */
	/////////////test

	enum {upl,upr,btl,btr};
	int i,j,k,ind=0,last_ind[params.nobjs],nblobs,count=0,randind;
	char wk, *filename = argv[1], obj_pos_str[256];
	float duration;
	ifstream file;
	//window name and songs array
	const char* winname = "image", *binname = "binary";
	//color definitions
	Scalar red(0,0,255), green(0,255,0), blue(255,0,0), gray(128,128,128), obj_color[params.nobjs];
	//color hsv values
	Range hue(0,4),sat(130,255),val(0,255), cands[NBLOBS][2];
	//point
	Point pt(0,0),pts[NPOINTS];
	//points for homography transform
	Point2f _dst[] = {Point2f(0,params.frame_dim[0]),Point2f(params.frame_dim[0],params.frame_dim[1]),Point2f(params.frame_dim[0],0),Point2f(0,0)};
	vector<Point2f> src,dst;
	//images
	Mat frame,bin,bin_res,obj_loc,H;
	//object for capturing video
	VideoCapture cap;
	//audio files
	Music music[params.nobjs][NAREAS][params.nsongs];
	//time for when music will start
	Time start;
	//blobs
	blob bl[NBLOBS];
	bl[0] = blob(Range(10,20),Range(30,40));
	//arguments for point collection
	ptColArgs pca(pts);

	//setting random
	srand(time(NULL));

	for(i=0; i<params.nobjs;i++)
		last_ind[i] = 0;

	//converting vector to array
	for(i=0;i<NPOINTS;i++)
		dst.push_back(_dst[i]);

     //initializating audio files 
	for(i=0; i<params.nobjs; i++)
     	for(j=0; j<NAREAS; j++)
          	for(k=0;k<params.nsongs;k++)
               	if(!music[i][j][k].openFromFile(params.songs[i][j][k]))
                    	err("ERROR: One of the songs could not be loaded!");

	//generating objects colors
	for(i=0; i<params.nobjs; i++)
		obj_color[i] = Scalar(rand() % 256, rand() % 256, rand() % 256);		

	//creating window
	namedWindow(winname,CV_WINDOW_AUTOSIZE);

	//opening camera
	cap.open(params.cam_index);
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
	grid dirg(Mat::zeros(params.frame_dim[1],params.frame_dim[0],CV_8UC3),64);

	//creating window for binary result and points
	namedWindow(binname,CV_WINDOW_AUTOSIZE);
	namedWindow("objects",CV_WINDOW_AUTOSIZE);

	//video loop
	while(true)
	{
		//capturing frame
		cap >> frame;
		if(!frame.data)
			err("ERROR: frame capture error");

		//creating result imgs
		bin_res = Mat::zeros(params.frame_dim[1],params.frame_dim[0],CV_8UC3);
		obj_loc = Mat::zeros(params.nobjs*15+10,250,CV_8U);

		//warping image acording to homography
		warpPerspective(frame,frame,H,Size(params.frame_dim[0],params.frame_dim[1]));
		
		for(i=0; i<params.nobjs; i++)
		{
			//locating object 
			cvtColor(frame,bin,CV_BGR2HSV);
			inRange(bin,Scalar(params.hsv[i][0].start,params.hsv[i][1].start,params.hsv[i][2].start),Scalar(params.hsv[i][0].end,params.hsv[i][1].end,params.hsv[i][2].end),bin);
			nblobs = dirg.locate(bin,cands);
			
			//adding binary image of object to binary result
			cvtColor(bin_res,bin_res,CV_BGR2GRAY); 
			bin_res += bin;
			cvtColor(bin_res,bin_res,CV_GRAY2BGR); 

			if(nblobs>0)
			{
				//converting ranges to blobs
				for(j=0;j<nblobs;j++)
					bl[j] = blob(cands[j][0],cands[j][1]);
				//ordering blobs
				orderBlob(bl,nblobs);
				//marking object in image
				bl[0].mark(frame,obj_color[i]);
				bl[0].mark(bin_res,obj_color[i]); 
				sprintf(obj_pos_str,"Object %2d: x = %5d , y = %5d",i+1,bl[0].point().x,bl[0].point().y);
			     //determining which music to play
				if(bl[0].point().x < frame.cols/2 && bl[0].point().y < frame.rows/2)
				{
					ind = upl;
				}
				else if(bl[0].point().x < frame.cols/2)
				{
					ind = btl;
				}
				else if(bl[0].point().y < frame.rows/2)
				{
					ind = upr;
				}
				else
				{
					ind = btr;
				}

				if(ind != last_ind[i])
					for(j=0; j<params.nsongs; j++)
						music[i][last_ind[i]][j].stop();
				else
					for(j=0; j<params.nsongs; j++)
						if(music[i][ind][j].getStatus() == PLAYING)
							break;
				if(j == params.nsongs)
				{
					randind = rand() % params.nsongs;
					duration = music[i][ind][randind].getDuration().asSeconds();
					start = seconds((duration > 30.0 && params.randomize)?(duration*0.01*(rand() % 90)):0.0);
					music[i][ind][randind].setPlayingOffset(start);
					music[i][ind][randind].play();
				}

				last_ind[i] = ind;
			}
			else
			{
				sprintf(obj_pos_str,"Object %2d: x = - , y = -",i+1); 
				for(j=0; j<params.nsongs; j++)
					music[i][last_ind[i]][j].stop();		
			}
			putText(obj_loc,obj_pos_str,Point(5,i*15+10),FONT_HERSHEY_SIMPLEX,0.4,Scalar(255,255,255));
		}


		//displaying images
		gride(frame);
		imshow(winname,frame); 
		imshow(binname,bin_res); 
		imshow("objects",obj_loc); 

		if(waitKey(10) == 27)
			break;
	}

	return 0;
}
