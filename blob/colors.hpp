#ifndef _TRACKING_COLORS_
#define _TRACKING_COLORS_

#define COLOR(n) (((i)>14)?Scalar(64,128,255):Scalar(((((n+1)-(n+1)%4)%8!=0)?(255-(((n+1)>7)?127:0)):0),((((n+1)-(n+1)%2)%4!=0)?(255-(((n+1)>7)?127:0)):0),(((n+1)%2!=0)?(255-(((n+1)>7)?127:0)):0)))

#endif
