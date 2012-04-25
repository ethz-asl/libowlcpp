/** @file "/owlcpp/include/owlcpp/rdf/triple_map.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_MAP_HPP_
#define TRIPLE_MAP_HPP_
#include <vector>
#include "boost/iterator/indirect_iterator.hpp"
#include "boost/ptr_container/ptr_vector.hpp"
#include "boost/tuple/tuple.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/doc_id.hpp"
#include "owlcpp/rdf/triple.hpp"
//#include "owlcpp/rdf/detail/triple_store_query.hpp"

namespace owlcpp{

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

*******************************************************************************/
class Triple_map {
   /**

   Create triples on heap, store pointers in boost::ptr_vector

   */
   typedef boost::ptr_vector<Triple> triples_t;
   typedef std::vector<Triple const*> index_t;
   typedef boost::tuple<index_t,index_t,index_t> value_t;
   typedef std::vector<value_t> indices_t;

public:
   typedef triples_t::iterator iterator;
   typedef triples_t::const_iterator const_iterator;

   /**
    @return number of stored triples
   */
   std::size_t size() const {return triples_.size();}
   const_iterator begin() const {return triples_.begin();}
   const_iterator end() const {return triples_.end();}
//   Triple const& operator[](const std::size_t i) const {
//      return store_.get<query_detail::seq_tag>()[i];
//   }
   void clear() {
      indices_.clear();
      triples_.clear();
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
      triples_.push_back(t);
      const Node_id max = subj > pred ? std::max(subj, obj) : std::max(pred, obj) ;
      if( max() >= indices_.size() ) indices_.resize(max() + 1);
      indices_[subj()].get<0>().push_back(t);
      indices_[pred()].get<1>().push_back(t);
      indices_[obj() ].get<2>().push_back(t);
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
   typename query_detail::Query_type<Subj,Pred,Obj,Doc>::range_t
   find(const Subj subj, const Pred pred, const Obj obj, const Doc doc) const {
      typedef typename query_detail::Query_type<Subj, Pred, Obj, Doc> q_type;
      return q_type::range(triples_, indices_, subj, pred, obj, doc);
   }
   */

private:
   triples_t triples_;
   indices_t indices_;

};

}//namespace owlcpp
#endif /* TRIPLE_MAP_HPP_ */
