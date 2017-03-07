#include <iostream>
#include <fstream>
#include <streambuf>
#include <regex>
#include "cpplib.h"

int main(int argc, char ** argv)
{
    try
    {
        std::vector<std::string> args;
        args.reserve(6);
        for(int i = 0; i < argc; ++i)
        {
            args.push_back(argv[i]);
        }

        if(args.size()<2)
        {
            std::cout<<"Welcome to cpplib"<<std::endl;
            std::cout<<"1. Please run 'cpplib update'"<<std::endl;
            std::cout<<"2. Run 'cpplib compiler list' to show supported compilers"<<std::endl;
            std::cout<<"3. Run 'cpplib compiler COMPILER_NAME' to set default compiler"<<std::endl;
            std::cout<<"4. Run 'cpplib install LIBRARY' to install"<<std::endl;
        }


        /** UPDATE **/
        if(std::find(args.begin(), args.end(), "update") != args.end())
        {
            cpplib::update();
        }

        /** COMPILER **/
        auto it = std::find(args.begin(), args.end(), "compiler");
        if(it != args.end() && it+1 !=args.end())
        {
            /** LIST **/
            if(*(it+1)=="list")
            {
                cpplib::listcompilers();
            }
            else
            {
                cpplib::setcompiler(*(it+1));
            }
        }

        /** INSTALL **/
        it = std::find(args.begin(), args.end(), "install");
        if(it != args.end())
        {
            if(it+1 != args.end())
            {
                if(it+2 != args.end())//Install with defined version
                {
                    cpplib::install(*(it+1),*(it+2));
                }
                else                 //Install lastast version
                {
                    cpplib::install(*(it+1));
                }
            }
            else
            {
                cpplib::install();  //Install all libs in config.json
            }

        }

        /** LIST **/
        if(std::find(args.begin(), args.end(), "packages") != args.end())
        {
            cpplib::listpackages();
        }
    }
    catch(const std::exception& ex)
    {
        std::cout<<ex.what()<<std::endl;
        return -1;
    }
    return 0;
}
