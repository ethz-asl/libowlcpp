/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_store_types.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_STORE_TYPES_HPP_
#define TRIPLE_STORE_TYPES_HPP_

#include "boost/multi_index_container_fwd.hpp"
#include "boost/multi_index/hashed_index_fwd.hpp"
#include "boost/multi_index/sequenced_index_fwd.hpp"
//#include "boost/multi_index/random_access_index_fwd.hpp"
#include "boost/multi_index/mem_fun.hpp"

#include "owlcpp/rdf/triple.hpp"

namespace owlcpp{ namespace query_detail{

struct seq_tag{};
struct subj_tag{};
struct pred_tag{};
struct obj_tag{};
struct doc_tag{};

typedef boost::multi_index_container<
   Triple,
   boost::multi_index::indexed_by<
//         boost::multi_index::random_access<
      boost::multi_index::sequenced<
            boost::multi_index::tag<seq_tag>
      >,
      boost::multi_index::hashed_non_unique<
         boost::multi_index::tag<subj_tag>,
         boost::multi_index::const_mem_fun<
            Triple, Node_id, &Triple::subject
         >
      >,
      boost::multi_index::hashed_non_unique<
         boost::multi_index::tag<pred_tag>,
         boost::multi_index::const_mem_fun<
            Triple, Node_id, &Triple::predicate
         >
      >,
      boost::multi_index::hashed_non_unique<
         boost::multi_index::tag<obj_tag>,
         boost::multi_index::const_mem_fun<
            Triple, Node_id, &Triple::object
         >
      >,
      boost::multi_index::hashed_non_unique<
         boost::multi_index::tag<doc_tag>,
         boost::multi_index::const_mem_fun<
            Triple, Doc_id, &Triple::document
         >
      >
   >
> triple_map_store_t;

}//namespace query_detail
}//namespace owlcpp
#endif /* TRIPLE_STORE_TYPES_HPP_ */
