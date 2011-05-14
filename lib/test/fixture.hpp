/** @file "/owl_cpp/lib/test/fixture.hpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2011 @author Mikhail K Levin
*******************************************************************************/
#ifndef FIXTURE_HPP_
#define FIXTURE_HPP_
#include <string>
#include "boost/preprocessor/stringize.hpp"
#include "owl_cpp/catalog.hpp"
#include "Kernel.h"
#include "owl_cpp/triple_to_fact.hpp"
#include "owl_cpp/parse_to_triple_store.hpp"
#include "owl_cpp/triple_store.hpp"

#ifndef SAMPLE_DATA_DIR
#error SAMPLE_DATA_DIR needs to be defined
#endif
#ifndef TEMPORARY_DIR
#error TEMPORARY_DIR needs to be defined
#endif

namespace owl_cpp{ namespace test{

/** File names of sample data, catalog
*******************************************************************************/
class Sample_data {
   struct Sample_catalog {
      Sample_catalog(const std::string& path):cat_(){find_ontologies(cat_, path);}
      Catalog cat_;
   private:
      Sample_catalog();
   };

public:
   static const std::string& sample_data_dir() {
      static const std::string str(BOOST_PP_STRINGIZE(SAMPLE_DATA_DIR));
      return str;
   }

   static const std::string& temp_dir() {
      static const std::string str(BOOST_PP_STRINGIZE(TEMPORARY_DIR));
      return str;
   }

   static const Catalog& catalog() {return cat_.cat_;}

private:
   static const Sample_catalog cat_;
   Triple_store store_;
   ReasoningKernel kernel_;

};

const Sample_data::Sample_catalog Sample_data::cat_(Sample_data::sample_data_dir());

}//namespace test
}//namespace owl_cpp
#endif /* FIXTURE_HPP_ */
