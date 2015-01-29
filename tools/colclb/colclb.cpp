#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <dirent.h>
#include <ctime>
#include "opencv2/opencv.hpp"
#include "../../paths.hpp"

using namespace std;
using namespace cv;

void err(const char * errmsg)
{
	cout << errmsg << endl;
	exit(0);
}

int main(int argc, char * argv[])
{
	bool cama = false, camb = false;
	const char * file = argv[1],*main,*ssat,*shue,*sval,*info;
	char c, *p, name[8], ans[32], path[256] = CONF_MAIN_DIR;
	int slot;
	Mat clone, img;
	Range hue, sat, val;
	ofstream txtfile;
	string color;
	istringstream ss;

	//inicial values for color
	hue.start = 0;   hue.end = 179;
	sat.start = 100; sat.end = 255;
	val.start = 50;  val.end = 255;

	//strings for menu
	main = "\nPress 'h', 's' or 'v' for setting hue, saturation or value parameters. Press 'p' for parameters stats or 'q' to exit.\n";
	shue = "\nHue. Press 's' for changing start value and 'e' for end value. Press 'r' to return to main menu.\n";
	ssat = "\nSaturation. Press 's' for changing start value and 'e' for end value. Press 'r' to return to main menu.\n";
	sval = "\nValue. Press 's' for changing start value and 'e' for end value. Press 'r' to return to main menu.\n";
	info = "\nSoftware for setting HSV values in order to track a color.\nYou can pass as argument:\n- The slot number containing image followed by either 'a' or 'b' or 'ab'\n(alfa, beta or both cameras, respectively) argument;\n- The letter '-p' followed by the path of an image";

	if(argc == 2)
	{
		if(strcmp(argv[1],"-info")==0)
			err(info);
		else
			err("Invalid parameters. Use '-info' for more information.");
	}
	
	if(argc != 3)
		err("Invalid parameters. Use '-info' for more information.");

	if(strcmp(argv[1],"-p")==0)
	{
		img = imread(argv[2],CV_LOAD_IMAGE_COLOR);
		if(!img.data)
		{
			cout << "\nfile '" << argv[2] << "' not found.\n"; 
			return 0;
		} 
	}
	else
	{
		if( !(cama=(strcmp(argv[2],"a")==0)) && !(camb=(strcmp(argv[2],"b")==0)) && (strcmp(argv[2],"ab")!=0) )
			err("Invalid parameters. Use '-info' for more information.");

		ss.str(argv[1]);
		ss >> slot;

		if(slot<1 || slot>NSLOTS)
			err("Invalid chosen slot.");
		
		strcat(path,argv[1]);
		strcat(path,cama?CONF_DIR_CAM_ALFA:CONF_DIR_CAM_BETA);
		strcat(path,CONF_COL_DIR);
		strcat(path,COLCLB_IMG_FILENAME);

		img = imread(path,CV_LOAD_IMAGE_COLOR);
		if(!img.data)
			err("File not found.");
	}

	namedWindow("img",CV_WINDOW_NORMAL);         //window for binary image
	namedWindow("originalimg",CV_WINDOW_NORMAL); //window for original image
	imshow("originalimg",img);

	cvtColor(img,img,CV_BGR2HSV);
	inRange(img,Scalar(hue.start,sat.start,val.start),Scalar(hue.end,sat.end,val.end),clone);

	imshow("img",clone);

	cout << main;

	//main menu
	while(true){

	switch(waitKey(0))
	{
		case 'h': //setting hue values
			cout << shue << flush;
			while(true)
			{
				c = waitKey(0);
				if(c=='s')//upper values
				{
					while(true)
					{
						cout << "\rhue start value: " << flush;
						c = waitKey(0);
						switch(c)
						{
							case 'w':
								if(hue.start < 179) 
								  hue.start++;
								cout << setw(3) << hue.start << flush;
								break;
							case 's':
								if(hue.start > 0) 
								  hue.start--;
								cout << setw(3) << hue.start << flush;
								break;
						}
						if(c=='r')
						{
							cout << shue << flush;
							break;
						}
						inRange(img,Scalar(hue.start,sat.start,val.start),Scalar(hue.end,sat.end,val.end),clone);
						imshow("img",clone);
					}
				}
				else if(c=='e')//bottom values
				{
					while(true)
					{
						cout << "\rhue end value: " << flush;
						c = waitKey(0);
						switch(c)
						{
							case 'w':
								if(hue.end < 179) 
								  hue.end++;
								cout << setw(3) << hue.end << flush;
								break;
							case 's':
								if(hue.end > 0) 
								  hue.end--;
								cout << setw(3) << hue.end << flush;
								break;
						}
						if(c=='r')
						{
							cout << shue << flush;
							break;
						}
						inRange(img,Scalar(hue.start,sat.start,val.start),Scalar(hue.end,sat.end,val.end),clone);
						imshow("img",clone);
					}
				}
				else if(c=='r')
				{
					cout << main;
					break;
				}
			}
			break;

		case 's': //setting saturation values
			cout << ssat << flush;
			while(true)
			{
				c = waitKey(0);
				if(c=='s')
				{
					while(true)
					{
						cout << "\rsaturation start value: " << flush;
						c = waitKey(0);
						switch(c)
						{
							case 'w':
								if(sat.start < 255) 
								  sat.start++;
								cout << setw(3) << sat.start << flush;
								break;
							case 's':
								if(sat.start > 0) 
								  sat.start--;
								cout << setw(3) << sat.start << flush;
								break;
						}
						if(c=='r')
						{
							cout << ssat << flush;
							break;
						}
						inRange(img,Scalar(hue.start,sat.start,val.start),Scalar(hue.end,sat.end,val.end),clone);
						imshow("img",clone);
					}
				}
				else if(c=='e')
				{
					while(true)
					{
						cout << "\rsaturation end value: " << flush;
						c = waitKey(0);
						switch(c)
						{
							case 'w':
								if(sat.end < 255) 
								  sat.end++;
								cout << setw(3) << sat.end << flush;
								break;
							case 's':
								if(sat.end > 0) 
								  sat.end--;
								cout << setw(3) << sat.end << flush;
								break;
						}
						if(c=='r')
						{
							cout << ssat << flush;
							break;
						}
						inRange(img,Scalar(hue.start,sat.start,val.start),Scalar(hue.end,sat.end,val.end),clone);
						imshow("img",clone);
					}
				}
				else if(c=='r')
				{
					cout << main;
					break;
				}
			}
			break;

		case 'v': //setting color value values
			cout << sval << flush;
			while(true)
			{
				c = waitKey(0);
				if(c=='s')
				{
					while(true)
					{
						cout << "\rvalue start value: " << flush;
						c = waitKey(0);
						switch(c)
						{
							case 'w':
								if(val.start < 255) 
								  val.start++;
								cout << setw(3) << val.start << flush;
								break;
							case 's':
								if(val.start > 0) 
								  val.start--;
								cout << setw(3) << val.start << flush;
								break;
						}
						if(c=='r')
						{
							cout << sval << flush;
							break;
						}
						inRange(img,Scalar(hue.start,sat.start,val.start),Scalar(hue.end,sat.end,val.end),clone);
						imshow("img",clone);
					}
				}
				else if(c=='e')
				{
					while(true)
					{
						cout << "\rvalue end value: " << flush;
						c = waitKey(0);
						switch(c)
						{
							case 'w':
								if(val.end < 255) 
								  val.end++;
								cout << setw(3) << val.end << flush;
								break;
							case 's':
								if(val.end > 0) 
								  val.end--;
								cout << setw(3) << val.end << flush;
								break;
						}
						if(c=='r')
						{
							cout << sval << flush;
							break;
						}
						inRange(img,Scalar(hue.start,sat.start,val.start),Scalar(hue.end,sat.end,val.end),clone);
						imshow("img",clone);
					}
				}
				else if(c=='r')
				{
					cout << main;
					break;
				}
			}
			break;
	
			case 'p': //prints stats about actual parameters
				cout << "\nstats:" << endl;
				cout << "number of whitepixels on actual image: " << countNonZero(clone) << endl;
				cout << "hue values (start,end): " << hue.start << "," << hue.end << endl;
				cout << "saturation values (start,end): " << sat.start << "," << sat.end << endl;
				cout << "value values (start,end): " << val.start << "," << val.end << endl; 
				cout << main;
				break;
			
			case 'q'://quits loop
				cout << "\nAre you sure you want to quit? If so, press 'y'." << endl;
				if(waitKey(0) != 'y')
				{
					cout << main;
					break;
				}
				cout << "Do you want to save the parameters? If so, press 'y'." << endl;
				if(waitKey(0)!='y') 
					return 0;

				if(strcmp(argv[1],"-p")==0)
				{
					return 0;
					cout << "Choose the slot you want to save (from 1 to " << NSLOTS << ") :";
					while(true)
					{
						cin >> slot;
						if(slot<1||slot>NSLOTS)
							cout << "Invalid slot. Choose again: ";
						else
						{
							sprintf(name,"%d",slot);
							break;
						}
					}
					while(true)
					{
						cout << "Choose camera you want for which you want to save these parameters (a/b/ab): ";
						cin >> ans;
						if(strcmp(ans,"a")==0)
						{
							cama = true;
							break;
						}
						else if(strcmp(ans,"b")==0)
						{
							camb = true;
							break;
						}
						else if(strcmp(ans,"ab")==0)
							break;
						else
							cout << "Invalid answer. " << endl;
					}
				
				}
				else
					strcpy(name,argv[1]);

				//creating configuration file
				strcpy(path,CONF_MAIN_DIR);
				strcat(path,name);
				strcat(path,cama?CONF_DIR_CAM_ALFA:CONF_DIR_CAM_BETA);
				strcat(path,CONF_IN_FILES_DIR);
				strcat(path,COL_IN_FILENAME);

				txtfile.open(path);

				txtfile << hue.start << endl; //hue values
				txtfile << hue.end << endl;
				txtfile << sat.start << endl; //saturation values
				txtfile << sat.end << endl;
				txtfile << val.start << endl; //value values
				txtfile << val.end << endl;

				txtfile.close();

				//saving binary image
				strcpy(path,CONF_MAIN_DIR);
				strcat(path,name);
				strcat(path,cama?CONF_DIR_CAM_ALFA:CONF_DIR_CAM_BETA);
				strcat(path,CONF_COL_DIR);
				strcat(path,BIN_IMG_FILENAME);
				imwrite(path,clone);
				
				if(!cama && !camb)
				{
					strcpy(path,CONF_MAIN_DIR);
					strcat(path,name);
					strcat(path,CONF_DIR_CAM_ALFA);
					strcat(path,CONF_IN_FILES_DIR);
					strcat(path,COL_IN_FILENAME);

					txtfile.open(path);

					txtfile << hue.start << endl; //hue values
					txtfile << hue.end << endl;
					txtfile << sat.start << endl; //saturation values
					txtfile << sat.end << endl;
					txtfile << val.start << endl; //value values
					txtfile << val.end << endl;

					txtfile.close();

					//saving binary image
					strcpy(path,CONF_MAIN_DIR);
					strcat(path,name);
					strcat(path,CONF_DIR_CAM_ALFA);
					strcat(path,CONF_COL_DIR);
					strcat(path,BIN_IMG_FILENAME);
					imwrite(path,clone);
				}

				cout << "\nThe configurations have been saved." << endl;
				return 0;
		}
	}

	return 0;
}
