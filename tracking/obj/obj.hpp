#ifndef _TRACKING_OBJ_HEADER_
#define _TRACKING_OBJ_HEADER_

#include "../blob/blob.hpp"
#include "../grid/grid.hpp"
#include <string>

enum {HUE, SAT, VAL};

class Object
{
	public:
	Object(cv::Range hsv_limits[3], const std::string& name = "object");
	Object(const cv::Range& hue, const cv::Range& sat, const cv::Range& val, const std::string& name = "object");
	Object(const Object& src);
	Object();
	~Object();

	//getters
	cv::Range getHue();
	cv::Range getSat();
	cv::Range getVal();
	cv::Range getHSVLimits();
	const std::string getName();
	//setters
	void setHue(const cv::Range& hue);
	void setSat(const cv::Range& sat);
	void setVal(const cv::Range& val);
	void setHSVLimits(const cv::Range hsv_limits[3]);

	Object& operator=(const Object&);

	//routines
	cv::Point2d location(cv::Mat& image, grid& dirg, bool mark = true, cv::Scalar color = cv::Scalar(255,255,255));

	private:
	cv::Range hsv_limits[3];
	const std::string name;
};

#endif
