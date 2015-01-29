#include "obj.hpp"

Object::Object(cv::Range hsv_limits[3], const std::string& name): name(name)
{
	setHSVLimits(hsv_limits);
}

Object::Object(const cv::Range& hue, const cv::Range& sat, const cv::Range& val, const std::string& name): name(name)
{
	setHue(hue);
	setSat(sat);
	setVal(val);
}

Object::Object(): name("object")
{;}

Object::~Object()
{;}

Object::Object(const Object& src): name(src.name)
{
	for(int i=0; i<3; i++)
		this->hsv_limits[i] = src.hsv_limits[i];
} 

Object& Object::operator=(const Object& src)
{
	for(int i=0; i<3; i++)
		this->hsv_limits[i] = src.hsv_limits[i];
	return *this;
}

cv::Range Object::getHue()
{
	return this->hsv_limits[HUE];
}

cv::Range Object::getSat()
{
	return this->hsv_limits[SAT];
}

cv::Range Object::getVal()
{
	return this->hsv_limits[VAL];
}

const std::string Object::getName()
{
	return this->name;
}

void Object::setHue(const cv::Range& hue)
{
	this->hsv_limits[HUE] = hue;
}

void Object::setSat(const cv::Range& sat)
{
	this->hsv_limits[SAT] = sat;
}

void Object::setVal(const cv::Range& val)
{
	this->hsv_limits[VAL] = val;
}

void Object::setHSVLimits(const cv::Range hsv_limits[3])
{
	for(int i=0; i<3; i++)
		this->hsv_limits[i] = hsv_limits[i];
}

cv::Point2d Object::location(cv::Mat& image, grid& dirg, bool mark, cv::Scalar color)
{
	int ncands;
	blob bl[32];
	cv::Mat bin;
	cv::Range cands[32][2];
	cv::Point2d defret = cv::Point2d(-1,-1);

	//checking if image is RGB
	if(image.type() != CV_8U)
		return defret;

	//converting bgr color to hsv		
	cvtColor(image,bin,CV_BGR2HSV);
	//thresholding
	inRange(bin,cv::Scalar(hsv_limits[HUE].start,hsv_limits[SAT].start,hsv_limits[VAL].start),cv::Scalar(hsv_limits[HUE].end,hsv_limits[SAT].end,hsv_limits[VAL].end),bin);

	ncands = dirg.locate(bin,cands);

	if(ncands > 0)
	{
		for(int i=0; i<ncands; i++)
			bl[i] = blob(cands[i][0], cands[i][1]);
		orderBlob(bl,ncands);
		if(mark)
			bl[0].mark(image,color);

		return bl[0].point();
	}

	return defret;
}
/*
using namespace cv;
using namespace std;

int main()
{
	cout << "zero, " << flush;
	Object obj1(Range(11,22),Range(33,44),Range(55,66));
	cout << "um, " << flush;
	Object obj2 = obj1;
	cout << "dois, " << flush;
	Object obj3(obj1);
	cout << "tres, " << flush;
	Object obj4;
	cout << "quatro, " << flush;
	obj4 = obj1;
	cout << "cinco, " << endl << flush;

	return 21;
}*/
