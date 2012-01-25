/** @file "/owlcpp/lib/io/input.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_IO_SOURCE
#define OWLCPP_IO_SOURCE
#endif
#include "owlcpp/io/input.hpp"

namespace owlcpp {

/*
*******************************************************************************/
std::istream& operator>>(std::istream& is, Triple_store& ts) {
   //TODO

   return is;
}

/*
*******************************************************************************/
void load_file(std::string const& file, Triple_store& ts) {
   //TODO
}

/*
*******************************************************************************/
void load_file(std::string const& file, Triple_store& ts, Catalog const& cat) {
   //TODO
}

/*
*******************************************************************************/
void load_iri(std::string const& iri, Triple_store& ts, Catalog const& cat) {
   //TODO
}



}//namespace owlcpp
