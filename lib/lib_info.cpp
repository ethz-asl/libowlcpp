/** @file "/owl_cpp/lib/lib_info.cpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#include "owl_cpp/lib_info.hpp"
#include "boost/preprocessor/stringize.hpp"

#ifndef OWLCPP_RELEASE_VERSION_1
#error define OWLCPP_RELEASE_VERSION_1
#endif

#ifndef OWLCPP_RELEASE_VERSION_2
#error define OWLCPP_RELEASE_VERSION_2
#endif

#ifndef OWLCPP_RELEASE_VERSION_3
#error define OWLCPP_RELEASE_VERSION_3
#endif

#ifndef OWLCPP_BUILD_VERSION
#error define OWLCPP_BUILD_VERSION
#endif

#ifndef OWLCPP_CODE_REVISION
#error define OWLCPP_CODE_REVISION
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
         BOOST_PP_STRINGIZE( OWLCPP_RELEASE_VERSION_1.OWLCPP_RELEASE_VERSION_2.OWLCPP_RELEASE_VERSION_3 )
   );
   return v;
}

/*
*******************************************************************************/
const std::string& build_str() {
   static const std::string str(BOOST_PP_STRINGIZE(OWLCPP_BUILD_VERSION));
   return str;
}

/*
*******************************************************************************/
const std::string& revision_str() {
   static const std::string str(BOOST_PP_STRINGIZE(OWLCPP_CODE_REVISION));
   return str;
}

/*
*******************************************************************************/
const std::string& lib_info_str() {
   static const std::string str(
         lib_name() + " v" + version_str() + " rev:" + revision_str() +
         " build:" + build_str()
   );
   return str;
}

}//namespace owl_cpp
