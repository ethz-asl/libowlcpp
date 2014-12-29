/** @file "/owlcpp/include/owlcpp/rdf/map_triple.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef MAP_TRIPLE_HPP_
#define MAP_TRIPLE_HPP_
#include "boost/fusion/algorithm/iteration/for_each.hpp"
#include "boost/fusion/container/vector.hpp"
#include "boost/fusion/include/mpl.hpp"
#include "boost/fusion/sequence/intrinsic/at.hpp"
#include "boost/fusion/sequence/intrinsic/front.hpp"
#include "boost/fusion/sequence/intrinsic/value_at.hpp"
#include "boost/mpl/fold.hpp"
#include "boost/mpl/front.hpp"
#include "boost/mpl/push_back.hpp"

#include "owlcpp/rdf/detail/triple_index_map_impl.hpp"
#include "owlcpp/rdf/detail/triple_index_selector.hpp"
#include "owlcpp/rdf/detail/triple_index_vector_impl.hpp"
#include "owlcpp/rdf/detail/triple_index.hpp"
#include "owlcpp/rdf/map_triple_fwd.hpp"

namespace owlcpp{

/**@brief Store, index, and search RDF triples
*******************************************************************************/
template<class Config, bool> class Map_triple {
   typedef typename boost::mpl::fold<
            Config,
            boost::fusion::vector<>,
            boost::mpl::push_back<
               boost::mpl::_1,
               map_triple_detail::Triple_index_selector<boost::mpl::_2>
            >
   >::type store;

   typedef typename boost::mpl::front<store>::type index1;

public:
   typedef typename index1::const_iterator const_iterator;
   typedef typename index1::iterator iterator;

   Map_triple() : store_(), size_(0) {}

   std::size_t size() const {return size_;}
   bool empty() const {return ! size_;}
   const_iterator begin() const {return front(store_).begin();}
   const_iterator end() const {return front(store_).end();}

   void insert(Triple const& t) {
      bool inserted;
      map_triple_detail::Insert insert(t, inserted);
      boost::fusion::for_each(store_, insert);
      if( inserted ) ++size_;
   }

   void erase(Triple const& t) {
      map_triple_detail::Erase erase(t);
      boost::fusion::for_each(store_, erase);
      --size_;
   }

   void clear() {
      map_triple_detail::Clear clear;
      boost::fusion::for_each(store_, clear);
      size_ = 0;
   }

   /**

   */
   template<class Subj, class Pred, class Obj, class Doc> struct query {
      typedef typename
               map_triple_detail::Index_selector<store,Subj,Pred,Obj,Doc>::index
               index_pos;
      typedef typename boost::fusion::result_of::value_at<store, index_pos>::type
               index;
      typedef typename index::template query<Subj,Pred,Obj,Doc>::iterator iterator;
      typedef typename index::template query<Subj,Pred,Obj,Doc>::range range;
   };

   template<bool Subj, bool Pred, bool Obj, bool Doc> struct query_b
   : public query<
        typename boost::mpl::if_c<Subj,Node_id,Any>::type,
        typename boost::mpl::if_c<Pred,Node_id,Any>::type,
        typename boost::mpl::if_c<Obj, Node_id,Any>::type,
        typename boost::mpl::if_c<Doc, Doc_id, Any>::type
     >
   {};

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
    @code template<class Subj, class Pred, class Obj, class Doc> class query; @endcode
    or from
    @code template<bool Subj, bool Pred, bool Obj, bool Doc> class query_b; @endcode
    For example,
    @code Map_triple<>::query_b<1,0,0,1>::range range =
          map_triple.find(subj, any, any, doc);
    @endcode
   */
   template<class Subj, class Pred, class Obj, class Doc>
   typename query<Subj,Pred,Obj,Doc>::range
   find(const Subj subj, const Pred pred, const Obj obj, const Doc doc) const {
      typedef typename query<Subj,Pred,Obj,Doc>::index_pos index_pos;
      return boost::fusion::at<index_pos>(store_).find(subj, pred, obj, doc);
   }

private:
   store store_;
   std::size_t size_;
};

/**@brief Store, index, and search RDF triples
*******************************************************************************/
template<class Config> class Map_triple<Config, true> {
   typedef std::vector<Triple> store_t;
public:
   typedef store_t::const_iterator const_iterator;

   std::size_t size() const {return v_.size();}
   bool empty() const {return v_.empty();}
   const_iterator begin() const {return v_.begin();}
   const_iterator end() const {return v_.end();}

   void insert(Triple const& t) {v_.push_back(t);}

   void erase(Triple const& t) {
      v_.erase(find(v_.begin(), v_.end(), t));
   }

   void clear() {v_.clear();}

private:
   store_t v_;
};

}//namespace owlcpp
#endif /* MAP_TRIPLE_HPP_ */
