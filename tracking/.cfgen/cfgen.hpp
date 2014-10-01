#ifndef _CFGEN_HEADER_
#define _CFGEN_HEADER_

#include <iostream>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <vector>

#define BUF_SIZE   1024

#define CFGEN_ERR_FNO 1
#define CFGEN_ERR_SNS 2
#define CFGEN_ERR_NEA 3
#define CFGEN_ERR_PNF 4
#define CFGEN_ERR_WFF 5
#define CFGEN_ERR_NAG 6

class Conf
{
   public:

   FILE * file;

   Conf(const char*);
   Conf(): file(NULL) {;}

   void acquire(const char*,int,const char*,...);
	template <class Tp>
	void vacquire(const char*,int,std::vector<Tp>&);
   void open(const char*);
   void close();

   ~Conf();
};

void error(int,int=0,const char* = NULL);

#endif
