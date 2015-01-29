/* clarg - a simple command line parser 
** version 1.0 
** by Erik Perillo 
*/ 

//implementation of template function of class Container
template <class Tp>
Tp parse(const Clarg& clarg, const Tp& defval = defVal<Tp>())
{
     std::stringstream ss;
     Tp retval = defval;

     clargs.push_back(clarg);

     for(int i=0; i<argv.size()-1; i++)
          for(int j=0; j<clarg.names.size(); j++)
               if(argv[i] == clarg.names[j])
               {
                    read_args++;
                    
                    ss.str(argv[i+1]);
                    ss >> retval;

                    argv.erase(argv.begin()+i+1);
                    return retval;                    
               } 

     return retval;
}


