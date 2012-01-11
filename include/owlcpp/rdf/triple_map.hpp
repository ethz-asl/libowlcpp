/** @file "/owlcpp/include/owlcpp/rdf/triple_map.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_MAP_HPP_
#define TRIPLE_MAP_HPP_
#include "boost/assert.hpp"
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/sequenced_index.hpp"
//#include "boost/multi_index/random_access_index.hpp"
#include "boost/multi_index/mem_fun.hpp"
#include "boost/range.hpp"

#include "owlcpp/rdf/triple.hpp"

namespace owlcpp{ namespace detail{
template<class,class,class,class> class Select_iter1;
template<class,bool> class Iterator_selector;
}//namespace detail
template<class,class,class,class>class Query;
struct blank{};

/**@brief Store, index, and search RDF triples
*******************************************************************************/
class Triple_map {

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
   > store_t;

   template<class,class,class,class> friend class Query;
   template<class,class,class,class> friend class detail::Select_iter1;
   template<class,bool> friend class detail::Iterator_selector;

public:
   typedef store_t::iterator iter_t;
   typedef boost::iterator_range<iter_t> range_t;

   void insert(Triple const& t) { store_.push_back(t); }

   template<class S, class P, class O, class D>
   typename Query<S,P,O,D>::range_t
   find(const S s, const P p, const O o, const D d) const {
      return Query<S,P,O,D>::find(store_, s, p, o, d);
   }

   range_t range() const {return boost::make_iterator_range(store_);}

private:
   store_t store_;
};

}//namespace owlcpp
#endif /* TRIPLE_MAP_HPP_ */
