/** @file "/owl_cpp/lib/lib_info.cpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#include "owl_cpp/lib_info.hpp"
#include "boost/preprocessor/stringize.hpp"

#include "owl_cpp/version.hpp"

#ifndef OWLCPP_VERSION_1
#error define OWLCPP_VERSION_1
#endif

#ifndef OWLCPP_VERSION_2
#error define OWLCPP_VERSION_2
#endif

#ifndef OWLCPP_VERSION_3
#error define OWLCPP_VERSION_3
#endif

#ifndef OWLCPP_VERSION_EXTRA
#error define OWLCPP_VERSION_EXTRA
#endif

#ifndef OWLCPP_BUILD
#error define OWLCPP_BUILD
#endif

namespace owl_cpp {

/*
*******************************************************************************/
const std::string& lib_name() {
   static const std::string name("owl_cpp");
   return name;
}

/*
*******************************************************************************/
const std::string& version_str() {
   static const std::string v(
         BOOST_PP_STRINGIZE(OWLCPP_VERSION_1) "."
         BOOST_PP_STRINGIZE(OWLCPP_VERSION_2) "."
         BOOST_PP_STRINGIZE(OWLCPP_VERSION_3) "-"
         BOOST_PP_STRINGIZE(OWLCPP_VERSION_EXTRA)
   );
   return v;
}

/*
*******************************************************************************/
const std::string& build_str() {
   static const std::string str(BOOST_PP_STRINGIZE(OWLCPP_BUILD));
   return str;
}

/*
*******************************************************************************/
const std::string& lib_info_str() {
   static const std::string str(
         lib_name() + " v" + version_str() + " build:" + build_str()
   );
   return str;
}

}//namespace owl_cpp
