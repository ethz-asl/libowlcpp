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

template<class,class,class,class> class Selector_1;
template<class, class, class, class> struct Query_type;

}//namespace detail

template<bool,bool,bool,bool>class Query;

struct blank{};


/**@brief Store, index, and search RDF triples
*******************************************************************************/
class Triple_map {

//   struct seq_tag{};
   struct subj_tag{};
   struct pred_tag{};
   struct obj_tag{};
   struct doc_tag{};

   typedef boost::multi_index_container<
      Triple,
      boost::multi_index::indexed_by<
//         boost::multi_index::random_access<
         boost::multi_index::sequenced<
//            boost::multi_index::tag<seq_tag>
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

   template<bool,bool,bool,bool> friend class Query;
   template<class,class,class,class> friend class detail::Selector_1;


public:
   typedef store_t::iterator iter_t;
   typedef boost::iterator_range<iter_t> range_t;

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
   typename detail::Query_type<Subj,Pred,Obj,Doc>::query_t::range_t
   find(const Subj subj, const Pred pred, const Obj obj, const Doc doc) const {
      return detail::Query_type<Subj,Pred,Obj,Doc>::query_t::find(*this, subj, pred, obj, doc);
   }

   /**
    @return iterator range of all stored triples
   */
   range_t all() const {return boost::make_iterator_range(store_);}

private:
   store_t store_;
};

}//namespace owlcpp
#endif /* TRIPLE_MAP_HPP_ */
