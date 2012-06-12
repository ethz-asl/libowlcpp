/** @file "/owlcpp/include/owlcpp/rdf/triple_map.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_MAP_HPP_
#define TRIPLE_MAP_HPP_
#include "boost/fusion/include/as_vector.hpp"
#include "boost/fusion/include/for_each.hpp"

#include "owlcpp/rdf/detail/triple_map_detail.hpp"

namespace owlcpp{

struct any{};

/**@brief Store, index, and search RDF triples

   @decision Storing triples.
   Options and RAM/time performance:
   - boost::multi_index 2GB/150s
   - std::vector<Triple> 0.921GB/91s
   - boost::ptr_vector<Triple> 1.1GB/98s

   @decision Indexing triples.
   Options and RAM/time performance:
   - boost::multi_index 2GB/150s
   - std::vector<Triple>, no index 0.921GB/91s
   - boost::ptr_vector<Triple>, no index 1.1GB/98s
   - boost::ptr_vector<Triple>, vector<vector> empty 1.4GB/98s
   - boost::ptr_vector<Triple>, vector<vector> 1.7GB/110s

   @decision Storing triples
   Create triples on heap, store pointers in boost::ptr_vector<Triple>

*******************************************************************************/
template<
   bool Index_subj = 1,
   bool Index_pred = 0,
   bool Index_obj = 0,
   bool Index_doc = 0
> class Triple_map {

   typedef detail::Index_types<
            Index_subj,
            Index_pred,
            Index_obj,
            Index_doc
            > index_types;

   typedef typename index_types::store_type store_t;

   class Insert {
   public:
      Insert(Triple const& t) : t_(t) {}
      template<class Ind> void operator()(Ind& i) const {i.add(t_);}
   private:
      Triple const& t_;
   };

public:
//   typedef triples_t::iterator iterator;
//   typedef triples_t::const_iterator const_iterator;

   /**
    @return number of stored triples
   */
//   std::size_t size() const {return triples_.size();}
//   const_iterator begin() const {return triples_.begin();}
//   const_iterator end() const {return triples_.end();}

   void clear() {
//      triples_.clear();
   }

   /**
    @param t triple stored in triple map
   */
   void erase(Triple const& t) {

   }

   /**@brief Insert a new triple
   */
   void insert(
            const Node_id subj,
            const Node_id pred,
            const Node_id obj,
            const Doc_id doc
   ) {
      Triple* const t = new Triple(subj, pred, obj, doc);
      Insert ins(*t);
      boost::fusion::for_each(store_, ins);
   }


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
    @code Query<1,0,0,1>::range_t range = triple_map.find(subj, blank(), blank(), doc);
    @endcode
   template<class Subj, class Pred, class Obj, class Doc>
   typename query_detail::Query_impl<Subj,Pred,Obj,Doc,Triple_map>::range
   find(const Subj subj, const Pred pred, const Obj obj, const Doc doc) const {
      typedef query_detail::Query_impl<Subj,Pred,Obj,Doc,Triple_map> q_type;
      return q_type::make_range(*this, subj, pred, obj, doc);
   }
   */

private:
   store_t store_;

};

}//namespace owlcpp
#endif /* TRIPLE_MAP_HPP_ */
