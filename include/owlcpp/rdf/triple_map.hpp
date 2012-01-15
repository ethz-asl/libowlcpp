/** @file "/owlcpp/include/owlcpp/rdf/triple_map.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_MAP_HPP_
#define TRIPLE_MAP_HPP_
#include "owlcpp/rdf/detail/triple_store_types.hpp"
#include "boost/assert.hpp"
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/sequenced_index.hpp"
//#include "boost/multi_index/random_access_index.hpp"
#include "boost/multi_index/mem_fun.hpp"

#include "owlcpp/rdf/detail/triple_store_types.hpp"
#include "owlcpp/rdf/detail/triple_store_query.hpp"

namespace owlcpp{

/**@brief Store, index, and search RDF triples
*******************************************************************************/
class Triple_map {
//   template<bool,bool,bool,bool> friend class Query;
   typedef query_detail::triple_map_store_t store_t;

public:
   typedef store_t::iterator iterator;

   /**@brief Insert a new triple
    @param triple
   */
   void insert(Triple const& triple) { store_.push_back(triple); }

   /**
    @return number of stored triples
   */
   std::size_t size() const {return store_.size();}

   /**@brief Search triples by subject, predicate, object, or document IDs.
    @details Polymorphically search stored triples to find ones with matching
    any combination of node IDs for subject, predicate, or object nodes or
    document ID.
    If none of the nodes are specified, the search returns a range containing all
    stored triples, same as from all() method.
    @param subj ID for subject node of blank()
    @param pred ID for predicate node of blank()
    @param obj ID for object node of blank()
    @param doc ID for document node of blank()
    @return iterator range of triples matching the query.
    @details
    The type of the range can be obtained from
    @code template<bool Subj, bool Pred, bool Obj, bool Doc> class Query; @endcode
    For example,
    @code Query<1,0,0,1>::range_t range = triple_map.find(subj, blank(), blank(), doc); @endcode
   */
   template<class Subj, class Pred, class Obj, class Doc>
   typename query_detail::Query_type<Subj,Pred,Obj,Doc>::range_t
   find(const Subj subj, const Pred pred, const Obj obj, const Doc doc) const {
      typedef typename query_detail::Query_type<Subj, Pred, Obj, Doc> q_type;
      return q_type::range(store_, subj, pred, obj, doc);
   }

   iterator begin() const {return store_.begin();}
   iterator end() const {return store_.end();}

private:
   store_t store_;
};

}//namespace owlcpp
#endif /* TRIPLE_MAP_HPP_ */
