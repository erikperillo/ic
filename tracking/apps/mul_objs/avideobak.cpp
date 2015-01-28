#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include "../../blob/blob.hpp"
#include "../../grid/grid.hpp"
#include "../../ptcol/ptcol.hpp"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace sf;
using namespace cv;

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

#define UPL_SONG 0
#define UPR_SONG 1
#define BTL_SONG 2
#define BTR_SONG 3

#define BUF_SIZE 1024
#define NMAX_SONGS 16
#define NMAX_CHARS 128
 
struct Args
{
	Range hsv[3];
	int nsongs,cam_index,frame_dim[2];
	char songs[4][NMAX_SONGS][NMAX_CHARS];
	Args(): nsongs(0), cam_index(0) 
	{
	frame_dim[0] = frame_dim[1] = SIZE;
	hsv[0] = hsv[1] = hsv[2] = Range(0,127);
	}
};

int readConf(Args& args,const char* filename)
{
	char line[BUF_SIZE],*word,*line_args,garbage[100]; const char *delim = "	 \n";
	int count,i,nargs=0;
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
	
	else if(strcmp(word,"hsv_low:")==0)
	{
		line_args = strtok(NULL,"\n");
		if(sscanf(line_args,"%d %d %d",&args.hsv[0].start,&args.hsv[1].start,&args.hsv[2].start) != 3)
			return -1;
		nargs++;
	}
	else if(strcmp(word,"hsv_upp:")==0)
	{
		line_args = strtok(NULL,"\n");
		if(sscanf(line_args,"%d %d %d",&args.hsv[0].end,&args.hsv[1].end,&args.hsv[2].end) != 3)
			return -1;
		nargs++;
	}
	else if(strcmp(word,"nsongs:")==0)
	{
		line_args = strtok(NULL,"\n");
		if(sscanf(line_args,"%d",&args.nsongs) != 1)
			return -1;
		if(args.nsongs > NMAX_SONGS)
			return -1;
		nsdef = true;
		nargs++;
	}
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
	else if(strcmp(word,"songs_upl:")==0)
	{
		if(!nsdef)
			return -1;
		
		for(i=0,word=strtok(NULL,delim); i<args.nsongs && word!=NULL; i++,word=strtok(NULL,delim))
		{
			if(sscanf(word,"%s",args.songs[0][i]) != 1)
				return -1;
		}
		if(i != args.nsongs)
			return -1;
		nargs++;
	}
	else if(strcmp(word,"songs_upr:")==0)
	{
		if(!nsdef)
			return -1;

		for(i=0,word=strtok(NULL,delim); i<args.nsongs && word!=NULL; i++,word=strtok(NULL,delim))
		{
			if(sscanf(word,"%s",args.songs[1][i]) != 1)
				return -1;
		}
		if(i != args.nsongs)
			return -1;
		nargs++;
	}
	else if(strcmp(word,"songs_btl:")==0)
	{
		if(!nsdef)
			return -1;

		for(i=0,word=strtok(NULL,delim); i<args.nsongs && word!=NULL; i++,word=strtok(NULL,delim))
		{
			if(sscanf(word,"%s",args.songs[2][i]) != 1)
				return -1;
		}
		if(i != args.nsongs)
			return -1;
		nargs++;
	}
	else if(strcmp(word,"songs_btr:")==0)
	{
		if(!nsdef)
			return -1;

		for(i=0,word=strtok(NULL,delim); i<args.nsongs && word!=NULL; i++,word=strtok(NULL,delim))
		{
			if(sscanf(word,"%s",args.songs[3][i]) != 1)
				return -1;
		}
		if(i != args.nsongs)
			return -1;
		nargs++;
	}  
	}

	return ((nargs<7)?-1:1);
}
/////////////////////////////////TEST

void err(const char* errmsg,int errcode=-1)
{
	cout << errmsg << endl;
	exit(errcode);
}
/*
void musicInfo(const Music& music)
{
	cout << "SampleRate: "	<< music.GetSampleRate();
	cout << "\nChannelsCount: " << music.GetChannelsCount();
	cout << "\nStatus: "	  << music.GetStatus();
	cout << "\nLoop: "		 << music.GetLoop();
	cout << "\nPitch: "		<< music.GetPitch();
	cout << "\nVolume: "	  << music.GetVolume();
	cout << "\nDuration: "	<< music.GetDuration();
	cout << "\nOffset: "	  << music.GetPlayingOffset() << endl;
	return;
}*/

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

	/////////////test
/*
	cout << "hsv start, end:" << params.hsv[0].start << "," << params.hsv[1].start << "," << params.hsv[2].start << " - " << params.hsv[0].end << "," << params.hsv[1].end << "," << params.hsv[2].end << endl; 
	cout << "nsongs: " << params.nsongs << endl;
	cout << "frame w,h: " << params.frame_dim[0] << "," << params.frame_dim[1] << endl;
	for(int ct=0; ct<4; ct++) 
	for(int k=0;k<params.nsongs;k++)
	{
		cout << "song[" << ct << "][" << k << "] = " << params.songs[ct][k] << endl;
	}  */
	/////////////test

	enum {upl,upr,btl,btr};
	int i,j,ind=0,last_ind=0,nblobs,count=0,randind;
	float duration;
	char wk, *filename = argv[1];
	ifstream file;
	//window name and songs array
	const char* winname = "image", *binname = "binary", *songs[] = {"song1.wav","song2.wav","song3.wav","song4.wav"};
	//color definitions
	Scalar red(0,0,255), green(0,255,0), blue(255,0,0), gray(128,128,128);
	//color hsv values
	Range hue(0,4),sat(130,255),val(0,255), cands[NBLOBS][2];
	//point
	Point pt(0,0),pts[NPOINTS];
	//points for homography transform
	Point2f _dst[] = {Point2f(0,params.frame_dim[0]),Point2f(params.frame_dim[0],params.frame_dim[1]),Point2f(params.frame_dim[0],0),Point2f(0,0)};
	vector<Point2f> src,dst;
	//images
	Mat frame,bin,H;
	//object for capturing video
	VideoCapture cap;
	//audio files
	Music music[NSONGS][NMAX_SONGS];
	//time for when the music will start
	Time start;
	//blobs
	blob bl[NBLOBS];
	bl[0] = blob(Range(10,20),Range(30,40));
	//arguments for point collection
	ptColArgs pca(pts);

	//setting random
	srand(time(NULL));

	//converting vector to array
	for(i=0;i<NPOINTS;i++)
	dst.push_back(_dst[i]);

	//initializating audio files 
	for(i=0;i<NSONGS;i++)
		for(j=0;j<params.nsongs;j++)	
			if(!music[i][j].openFromFile(params.songs[i][j]))
				err("ERROR: One of the songs could not be loaded!");

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

	//video loop
	while(true)
	{
		//capturing frame
		cap >> frame;
		if(!frame.data)
			err("ERROR: frame capture error");

		//warping image acording to homography
		warpPerspective(frame,frame,H,Size(params.frame_dim[0],params.frame_dim[1]));

		//locating object 
		cvtColor(frame,bin,CV_BGR2HSV);
		inRange(bin,Scalar(params.hsv[0].start,params.hsv[1].start,params.hsv[2].start),Scalar(params.hsv[0].end,params.hsv[1].end,params.hsv[2].end),bin);
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
				for(i=0; i<params.nsongs; i++)
					music[last_ind][i].stop();
			else
				for(i=0; i<params.nsongs; i++)
					if(music[ind][i].getStatus() == PLAYING)
						break;
			if(i==params.nsongs)
			{
				randind = rand() % params.nsongs;
				duration = music[ind][randind].getDuration().asSeconds();
				start = seconds((duration > 30.0)?(duration*0.01*(rand() % 90)):0.0);
				music[ind][randind].setPlayingOffset(start);
				music[ind][randind].play();
			}

			last_ind = ind;
		}
		else	  
			for(i=0; i<params.nsongs; i++)
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
