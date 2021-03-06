#include "grid.hpp"

//finds best division for grid
grid::grid(const cv::Mat& mat,int sdiv)
{
	int i;

	if(mat.rows%sdiv != 0.0)//finding the divisions that best fill the grid
	{
		int bestdiv = sdiv;
		for(i=sdiv;sdiv-i<5 && i>0;i--)
		{
			if(mat.rows%i < mat.rows%bestdiv)
			  bestdiv = i;
		}
		for(i=sdiv;i-sdiv<5;i++)
		{
			if(mat.rows%i < mat.rows%bestdiv)
			  bestdiv = i;
		}
		divy = bestdiv;
	}
	else divy = sdiv;	

	secs = mat.rows/divy;	
	i=0;
	while(mat.cols - (i+1)*secs >= 0) i++;
	divx = i;

	maxwp = (0.25)*(secs*secs); //setting minimum number of whitepixels required per sector
}

//draws a grid on an image
int grid::mark(cv::Mat& img, const cv::Scalar color)
{
   if(img.type() != CV_8UC3)
      return -1;

   int i;

   for(i=0;i<divx;i++)
   {
      if(i*secs > img.cols)
         return -1;
      img.col(i*secs) = color;
   }
   for(i=0;i<divy;i++)
   {
      if(i*secs > img.rows)
         return -1;
      img.row(i*secs) = color;
   }

   return 1;
}

//finds blobs in image. 
//returns an negative number if error occurs or number of blobs found 
int grid::locate(const cv::Mat& bin,cv::Range blobs[][2],int nmax)
{
   if(bin.type() != CV_8UC1)
      return -1;

	//locating algorithm's variables
	int i,j,k,l,m=0,y=0,x=0,ini,nlines=0,npastlines=0,ncands=0,nblobs=0;
	bool found = false;	
	Line enil[512], pastenil[512];
	cand idate[256], *p;

	//algorithm to find:
	for(i=0;i<divy;i++){
	y = i*secs;
	for(j=0;j<divx;j++){
	x = j*secs;
	if(cv::countNonZero(bin(cv::Range(y,y+secs),cv::Range(x,x+secs))) >= maxwp) //condition for the particle to be considered
	{
		if(!found)
		{
			ini = x;
			found = true;
		}
	}
	else if(found) //end of whitepixels "line"
	{
		found = false;
		enil[nlines].line = cv::Range(ini,x); 
		nlines++; 
	}
	//end of second for. 
	}
	if(found) //the object is at the right end of the canvas
	{
		found = false;
		enil[nlines].line = cv::Range(ini,x+secs); 
		nlines++; 
	}
	for(k=0;k<nlines;k++)
	{
		found = false;
		for(l=0;l<npastlines;l++)
		  if(!(pastenil[l].line.start >= enil[k].line.end || pastenil[l].line.end <= enil[k].line.start)) //line above is part of the same candidate
		  {
		     if(!found) //first past line found that intersects present one
			  {
				  //checking past line positions to update candidate size:
			  	  if(enil[k].line.start < pastenil[l].idate->x.start)
				    pastenil[l].idate->x.start = enil[k].line.start;
				  if(enil[k].line.end > pastenil[l].idate->x.end)
				    pastenil[l].idate->x.end = enil[k].line.end;
				  enil[k].idate = pastenil[l].idate;
				  found = true;
			  }
			  else if((p=pastenil[l].idate) != enil[k].idate) //candidates may merge into one
				 {
				    if(p->x.start < enil[k].idate->x.start)
			         enil[k].idate->x.start = p->x.start;
			       if(p->x.end > enil[k].idate->x.end)
			         enil[k].idate->x.end = p->x.end;
			  	    if(p->y.start < enil[k].idate->y.start)
				      enil[k].idate->y.start = p->y.start;
					 for(m=l;m<npastlines;m++)
					   if(pastenil[m].idate==p)
						  pastenil[m].idate = enil[k].idate;
					 p->x.end = GRID_NOTOBJ; // condition for the candidate to be no longer considered an object
				 }
		  }

		if(!found) //new candidate is created
		{
			idate[ncands].x = enil[k].line;
			idate[ncands].y.start = y;
			enil[k].idate = &idate[ncands];
			ncands++;
		}

		enil[k].idate->y.end = y+secs; //updating end y position of candidate
	}

	for(k=0;k<nlines;k++) 
		pastenil[k] = enil[k];

	npastlines = nlines; 
	nlines = 0;
	found = false;
	}//algorithm

	//determining object and it's position
	for(i=0;i<ncands;i++) //checking if object has minumum size not to be considered a particle
	   if((p=&idate[i])->x.end != GRID_NOTOBJ && (p->x.end-p->x.start)*(p->y.end-p->y.start) > 2*secs*secs && nblobs < nmax)
	   {
         blobs[nblobs][0] = p->x;
         blobs[nblobs][1] = p->y;
         nblobs++;
	   }
		
   return nblobs;
}
