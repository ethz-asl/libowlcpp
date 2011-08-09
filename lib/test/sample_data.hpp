/** @file "/owl_cpp/lib/test/sample_data.hpp"
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef SAMPLE_DATA_HPP_
#define SAMPLE_DATA_HPP_
#include <string>
#include "boost/preprocessor/stringize.hpp"

#ifndef SAMPLE_DATA_DIR
#error SAMPLE_DATA_DIR needs to be defined
#endif
#ifndef TEMPORARY_DIR
#error TEMPORARY_DIR needs to be defined
#endif

namespace owl_cpp{ namespace test{

inline std::string sample_file_path(const std::string& name) {
   std::string sf( BOOST_PP_STRINGIZE(SAMPLE_DATA_DIR) );
   sf += '/';
   sf += name;
   return sf;
}

inline std::string temp_file_path(const std::string& name) {
   std::string sf( BOOST_PP_STRINGIZE(TEMPORARY_DIR) );
   sf += '/';
   sf += name;
   return sf;
}

}//namespace test
}//namespace owl_cpp

#endif /* SAMPLE_DATA_HPP_ */
