/* clarg - a simple command line parser
** version 1.0
** by Erik Perillo
*/

#ifndef __CLARG_HEADER__
#define __ClARG_HEADER__

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>

namespace clarg
{
     template <class Tp>
     Tp defVal()
     {return Tp();}

     //struct for describing a command line argument 
     struct Clarg
     {
          std::vector<std::string> names;
          std::string              description;

          Clarg(const char*, const char*);       
     };

     //stores information about command line arguments created and parses argv
     class Container
     {
          int                      read_args;
          std::vector<std::string> argv;
          std::vector<Clarg>       clargs;     

          public:

          Container(int argc, char** argv);

          #include "parse.tpp" 
          //template <class Tp> Tp parse(const Clarg&, const Tp&);
          //template <> std::string parse<std::string>(const Clarg&, const std::string&);
          //template <> bool parse<bool>(const Clarg&, const bool&);

          void describe(const char*);

          int getReadArgs();
     };
}

#endif
