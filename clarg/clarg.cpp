/* clarg - a simple command line parser 
** version 1.0 
** by Erik Perillo 
*/ 

//implementation of routines

#include "clarg.hpp"

clarg::Clarg::Clarg(const char* names, const char* description): description(description)
{
     std::stringstream ss(names);
     std::string       name;

     while(ss >> name)
          this->names.push_back(name);     
}  

clarg::Container::Container(int argc, char ** argv): read_args(0)
{
     for(int i=0; i<argc; i++)
          this->argv.push_back(std::string(argv[i]));
}
  
namespace clarg
{
     template<> 
     bool Container::parse<bool>(const Clarg& clarg, const bool& defval)
     {
          clargs.push_back(clarg);

          for(int i=0; i<argv.size(); i++)
               for(int j=0; j<clarg.names.size(); j++)
                    if(argv[i] == clarg.names[j])
                    {
                         read_args++;
                         return !defval;
                    }

          return defval;
     }

     template<>
     std::string Container::parse(const Clarg& clarg, const std::string& defval)
     {
          std::string retval = defval;
          size_t      pos;

          clargs.push_back(clarg);

          for(int i=0; i<argv.size()-1; i++)
               for(int j=0; j<clarg.names.size(); j++)
                    if(argv[i] == clarg.names[j])
                    {
                         read_args++;

                         retval = argv[i+1];
                         while((pos=retval.find("\\")) != std::string::npos)
                              retval.replace(pos,1," ");

                         argv.erase(argv.begin()+i+1);
                         return retval;
                    }

          return retval;
     }
}

void clarg::Container::describe(const char* helpmsg)
{
     int j;
     std::string line;

     if(helpmsg!=NULL)
          std::cout << helpmsg << std::endl;

     for(int i=0; i<clargs.size(); i++)
     {  
          line = "\t";
          for(j=0; j<clargs[i].names.size()-1; j++)
               line +=  clargs[i].names[j] + ", ";
          line += clargs[i].names[j];
          std::cout << std::left << std::setw(32) << line;
         
          line = clargs[i].description;
          std::cout << std::left << std::setw(32) << line << std::endl;
     }
     
     return;              
}

int clarg::Container::getReadArgs() 
{
     return read_args;
}

