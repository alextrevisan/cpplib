#ifndef CPPLIB_H
#define CPPLIB_H

#include "json11.hpp"

namespace cpplib
{

json11::Json load(const std::string& file);

void update();

void listcompilers();

void setcompiler(const std::string& compiler);

void addpackage(const std::string& package, const std::string& version);

void install(const std::string& package="all", const std::string& version="lastest");

void listpackages();


}//namespace cpplib
#endif // CPPLIB_H
