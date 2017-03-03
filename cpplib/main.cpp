#include <iostream>
#include <fstream>
#include <streambuf>
#include <regex>
#include "download.h"
#include "json11.hpp"
#include "unzip.h"

json11::Json load(const std::string& file)
{
    std::ifstream infile(file);
    std::string jsondata((std::istreambuf_iterator<char>(infile) ),
                         (std::istreambuf_iterator<char>()));

    std::string err;
    json11::Json json = json11::Json::parse(jsondata, err);
    if(err.size()>0)
    {
        return json11::Json{};
    }
    return json;
}

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
        /*args.push_back("update");
        args.push_back("compiler");
        args.push_back("MinGW-W64-4.9.2");
        args.push_back("install");
        args.push_back("SFML");*/
        if(args.size()<2)
        {
            std::cout<<"Welcome to cpplib"<<std::endl;
            std::cout<<"1. Please run 'cpplib update'"<<std::endl;
            std::cout<<"2. Run 'cpplib compiler list' to show supported compilers"<<std::endl;
            std::cout<<"3. Run 'cpplib compiler COMPILER_NAME' to set default compiler"<<std::endl;
            std::cout<<"4. Run 'cpplib install LIBRARY' to install"<<std::endl;
            system("pause");
        }


        /** UPDATE **/
        if(std::find(args.begin(), args.end(), "update") != args.end())
        {
            std::cout<<"Updating packages..."<<std::endl;
            Download("https://raw.githubusercontent.com/alextrevisan/cpplib/master/packages/packages.json", "packages.json");
        }

        /** COMPILER **/
        {
        auto it = std::find(args.begin(), args.end(), "compiler");
        if(it != args.end() && it+1 !=args.end())
        {
            /** LIST **/
            if(*(it+1)=="list")
            {
                auto json = load("packages.json");
                if(json.object_items().size()<=0)
                {
                    std::cout<<"Run cpplib update first"<<std::endl;
                    return 0;
                }
                std::cout<<"Listing compilers: "<<std::endl;
                auto jbegin = json.object_items().begin();
                auto jend = json.object_items().end();
                for (auto mapit =  jbegin; mapit!=jend; ++mapit)
                {
                    std::cout<<"    * "<< mapit->first<<std::endl;
                }
            }
            else
            {
                json11::Json config = load("config.json");

                if(config.object_items().size()<=0)
                {
                    config = json11::Json::object({{"compiler",*(it+1)}});
                }
                else
                {
                    json11::Json::object ob = config.object_items();
                    ob["compiler"] = json11::Json(*(it+1));
                    config = ob;
                }
                std::ofstream configfile("config.json");
                configfile<<config.dump();
                configfile.close();
                std::cout<<"compiler set to: "<<*(it+1)<<std::endl;
            }
        }
        }

        /** INSTALL **/
        {
        auto it = std::find(args.begin(), args.end(), "install");
        if(it != args.end() && it+1 !=args.end())
        {

            json11::Json config = load("config.json");
            if(config.object_items().find("compiler") == config.object_items().end())
            {
                std::cout<<"Select the compiler first with command: cpplib compiler NAME-VERSION"<<std::endl;
                std::cout<<"To list compilers and versions: cpplib compiler list"<<std::endl;
                return 0;
            }
            const std::string compiler = config["compiler"].string_value();
            json11::Json packages = load("packages.json");

            if(packages.object_items().find(compiler) == packages.object_items().end())
            {
                std::cout<<"Can't find selected compiler: "<<compiler<<std::endl;
                return 0;
            }
            packages = packages[compiler];
            if(packages.object_items().find(*(it+1)) == packages.object_items().end())
            {
                std::cout<<"Can't find selected package: "<<*(it+1)<<std::endl;
            }
            packages = packages[*(it+1)];


            std::string packageDescription = packages["description"].string_value();
            //std::cout<<packageDescription<<std::endl;
            packages = packages["versions"];

            if(it+2 != args.end())
            {
                std::stringstream ss(*(it+2));
                std::string version;
                std::getline(ss, version, '=');
                if(version=="version")
                {
                    std::getline(ss, version, '=');
                    if(packages.object_items().find(version)==packages.object_items().end())
                    {
                        std::cout<<packageDescription<<" version "<<version<<" not found."<<std::endl;
                        return 0;
                    }
                    std::cout<<"Installing "<<packageDescription<<" version "<<version<<std::endl;
                    Download(packages[version]["mirrors"].array_items()[0]["url"].string_value().c_str(), "lib.zip");
                    unzipAll("lib.zip");

                }

            }
            else for (auto mapit = packages.object_items().begin(); mapit!=packages.object_items().end(); ++mapit)
            {
                std::cout<<"Installing "<<packageDescription;
                std::cout<<" version "<<mapit->first<<std::endl;
                Download(packages[mapit->first]["mirrors"].array_items()[0]["url"].string_value().c_str(), "lib.zip");
                unzipAll("lib.zip");
            }

        }
        }

        /** LIST **/
        {
        if(std::find(args.begin(), args.end(), "packages") != args.end())
        {
            json11::Json config = load("config.json");

            if(config.object_items().find("compiler") == config.object_items().end())
            {
                std::cout<<"Select the compiler first with command: cpplib compiler NAME-VERSION"<<std::endl;
                std::cout<<"To list compilers and versions: cpplib compiler list"<<std::endl;
                return 0;
            }

            auto json = load("packages.json");
            const std::string& compiler = config["compiler"].string_value();
            auto jbegin = json[compiler].object_items().begin();
            auto jend = json[compiler].object_items().end();
            std::cout<<"Available packages for "<<compiler<<":"<<std::endl;
            for (auto mapit =  jbegin; mapit!=jend; ++mapit)
            {
                std::cout<<"    * "<< mapit->first<<std::endl;
            }
            //Download("https://raw.githubusercontent.com/alextrevisan/cpplib/master/packages/packages.json", "packages.json");
        }
        }
    }
    catch(const std::exception& ex)
    {
        std::cout<<ex.what()<<std::endl;
        return -1;
    }
    return 0;
}
