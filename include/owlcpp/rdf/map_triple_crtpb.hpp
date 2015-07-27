/** @file "/owlcpp/include/owlcpp/rdf/map_triple_crtpb.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_TRIPLE_CRTPB_HPP_
#define MAP_TRIPLE_CRTPB_HPP_
#include "boost/assert.hpp"
#include "boost/concept/assert.hpp"

#include "owlcpp/detail/map_traits.hpp"
#include "owlcpp/rdf/triple.hpp"

namespace owlcpp{

/**Enable operations on RDF triples.
Base for CRTP (Curiously Recurring Template Pattern).
*******************************************************************************/
template<class Super> class Map_triple_crtpb {
   typedef detail::Map_traits<Super> traits;
   typedef typename traits::map_triple_type map_triple_type;

   map_triple_type const& _map_triple() const {
      return static_cast<Super const&>(*this).map_triple_;
   }

   map_triple_type& _map_triple() {
      return static_cast<Super&>(*this).map_triple_;
   }

public:

   template<class Subj, class Pred, class Obj, class Doc> struct query
   : public map_triple_type::template query<Subj,Pred,Obj,Doc>{};

   template<bool Subj, bool Pred, bool Obj, bool Doc>struct query_b
   : public map_triple_type::template query_b<Subj,Pred,Obj,Doc>{};

   /**@brief Search triples by subject, predicate, object, or document IDs.
    @details Polymorphically search stored triples to find ones that match
    specified node IDs for subject, predicate, or object nodes or document ID.
    An instance of \b any matches all values for the corresponding triple
    element.
    If none of the nodes are specified, i.e., <tt>find(any, any, any, any)</tt>,
    the search returns a range of all stored triples, [begin(), end()).
    @param subj predicate for first element of triple (subject node),
    e.g., \b Node_id, \b any
    @param pred predicate for second element of triple (predicate node),
    e.g., \b Node_id, \b any
    @param obj predicate for third element of triple (object node),
    e.g., \b Node_id, \b any
    @param doc predicate for fourth element of triple (document ID),
    e.g., \b Doc_id, \b any
    @return iterator range of triples matching the query.
    @details
    The type of the range can be obtained from
    @code template<class Subj, class Pred, class Obj, class Doc> class query;
    @endcode
    or from
    @code template<bool Subj, bool Pred, bool Obj, bool Doc> class query_b;
    @endcode
    For example,
    @code Triple_map<>::query_b<1,0,0,1>::range range =
       triple_map.find(subj, any, any, doc);
    @endcode
   */
   template<class Subj, class Pred, class Obj, class Doc>
   typename query<Subj,Pred,Obj,Doc>::range
   find_triple(const Subj subj, const Pred pred, const Obj obj, const Doc doc) const {
      return _map_triple().find(subj, pred, obj, doc);
   }

   /**@brief Insert a new triple
   void insert(
            const Node_id subj,
            const Node_id pred,
            const Node_id obj,
            const Doc_id doc
   ) {
      insert(Triple::make(subj, pred, obj, doc));
   }
   */

   /**@brief Insert a new triple
   */
   void insert(Triple const& t) {
      BOOST_CONCEPT_ASSERT((Node_store<Super>));
      BOOST_ASSERT(
            static_cast<Super const&>(*this).find(t.subj_) &&
            "invalid subject ID"
      );
      BOOST_ASSERT(
            static_cast<Super const&>(*this).find(t.pred_) &&
            "invalid predicate ID"
      );
      BOOST_ASSERT(
            static_cast<Super const&>(*this).find(t.obj_) &&
            "invalid object ID"
      );
      BOOST_ASSERT( static_cast<Super const&>(*this).find(t.doc_) &&
            "invalid document ID"
      );
      _map_triple().insert(t);
   }

   void erase(Triple const& t) {
      _map_triple().erase(t);
   }

};

}//namespace owlcpp
#endif /* MAP_TRIPLE_CRTPB_HPP_ */
