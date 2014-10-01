#ifndef _BLOB_COLORS_
#define _BLOB_COLORS_

#define BLUE         Scalar(255,0,0)      //0
#define LIME         Scalar(0,255,0)      //1
#define RED          Scalar(0,0,255)      //2
#define WHITE        Scalar(255,255,255)  //3
#define AQUA         Scalar(255,255,0)    //4
#define YELLOW       Scalar(0,255,255)    //5
#define FUCHSIA      Scalar(255,0,255)    //6
#define NAVY         Scalar(128,0,0)      //7
#define GREEN        Scalar(0,128,0)      //8
#define MAROON       Scalar(0,0,128)      //9
#define GRAY         Scalar(128,128,128)  //10
#define TEAL         Scalar(128,128,0)    //11
#define OLIVE        Scalar(0,128,128)    //12
#define PURPLE       Scalar(128,0,128)    //13
#define BLACK        Scalar(0,0,0)        //14
#define SILVER       Scalar(192,192,192)  //15

#define COLOR(x) ( ((x)==0)?BLUE:(((x)==1)?LIME:(((x)==2)?RED:(((x)==3)?WHITE:(((x)==4)?AQUA:\
					    (((x)==5)?YELLOW:(((x)==6)?FUCHSIA:(((x)==7)?NAVY:(((x)==8)?GREEN:\
                   (((x)==9)?MAROON:(((x)==10)?GRAY:(((x)==11)?TEAL:(((x)==12)?OLIVE:\
                   (((x)==13)?PURPLE:(((x)==14)?BLACK:SILVER)))))))))))))) ) 


#endif
