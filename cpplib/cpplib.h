#ifndef CPPLIB_H
#define CPPLIB_H

#include "download.h"
#include "json11.hpp"

namespace cpplib {

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

void update()
{
    std::cout<<"Updating packages..."<<std::endl;
    Download("https://raw.githubusercontent.com/alextrevisan/cpplib/master/packages/packages.json", "packages.json");
}

void listcompilers()
{
    auto json = load("packages.json");
    if(json.object_items().size()<=0)
    {
        std::cout<<"Run cpplib update first"<<std::endl;
        return;
    }
    std::cout<<"Listing compilers: "<<std::endl;
    auto jbegin = json.object_items().begin();
    auto jend = json.object_items().end();
    for (auto mapit =  jbegin; mapit!=jend; ++mapit)
    {
        std::cout<<"    * "<< mapit->first<<std::endl;
    }
}

void setcompiler(const std::string& compiler)
{
    json11::Json config = load("config.json");

    if(config.object_items().size()<=0)
    {
        config = json11::Json::object({{"compiler",compiler}});
    }
    else
    {
        json11::Json::object ob = config.object_items();
        ob["compiler"] = json11::Json(compiler);
        config = ob;
    }
    std::ofstream configfile("config.json");
    configfile<<config.dump();
    configfile.close();
    std::cout<<"compiler set to: "<<compiler<<std::endl;
}

void install(const std::string& package, const std::string& version="lastest")
{
    json11::Json config = load("config.json");
    if(config.object_items().find("compiler") == config.object_items().end())
    {
        std::cout<<"Select the compiler first with command: cpplib compiler NAME-VERSION"<<std::endl;
        std::cout<<"To list compilers and versions: cpplib compiler list"<<std::endl;
        return;
    }
    const std::string compiler = config["compiler"].string_value();
    json11::Json packages = load("packages.json");

    if(packages.object_items().find(compiler) == packages.object_items().end())
    {
        std::cout<<"Can't find selected compiler: "<<compiler<<std::endl;
        return;
    }
    packages = packages[compiler];
    if(packages.object_items().find(package) == packages.object_items().end())
    {
        std::cout<<"Can't find selected package: "<<package<<std::endl;
    }
    packages = packages[package];


    std::string packageDescription = packages["description"].string_value();
    //std::cout<<packageDescription<<std::endl;
    packages = packages["versions"];

    if(version!="lastest")
    {
        std::stringstream ss(version);
        std::string v;
        std::getline(ss, v, '=');
        if(v=="version")
        {
            std::getline(ss, v, '=');
            if(packages.object_items().find(v)==packages.object_items().end())
            {
                std::cout<<packageDescription<<" version "<<v<<" not found."<<std::endl;
                return;
            }
            std::cout<<"Installing "<<packageDescription<<" version "<<v<<std::endl;
            Download(packages[v]["mirrors"].array_items()[0]["url"].string_value().c_str(), "lib.zip");
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

void listpackages()
{
    json11::Json config = load("config.json");

    if(config.object_items().find("compiler") == config.object_items().end())
    {
        std::cout<<"Select the compiler first with command: cpplib compiler NAME-VERSION"<<std::endl;
        std::cout<<"To list compilers and versions: cpplib compiler list"<<std::endl;
        return;
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
}


}//namespace cpplib
#endif // CPPLIB_H
