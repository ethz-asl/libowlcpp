/** @file "/owlcpp/include/owlcpp/rdf/triple_map.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_MAP_HPP_
#define TRIPLE_MAP_HPP_
#include "boost/fusion/include/at.hpp"
#include "boost/fusion/include/for_each.hpp"
#include "boost/fusion/include/front.hpp"
#include "boost/fusion/include/mpl.hpp"
#include "boost/mpl/assert.hpp"
#include "boost/mpl/at.hpp"
#include "boost/mpl/fold.hpp"
#include "boost/mpl/if.hpp"
#include "boost/mpl/push_back.hpp"
#include "boost/mpl/vector.hpp"
#include "boost/mpl/vector_c.hpp"

#include "owlcpp/rdf/detail/triple_map_config.hpp"
#include "owlcpp/rdf/detail/triple_index.hpp"

namespace owlcpp{

template<
   bool Index_subj = 1,
   bool Index_pred = 0,
   bool Index_obj = 0,
   bool Index_doc = 0
> class Triple_map;

/** Equals to any node or document ID.
Use for searching triples to indicate the elements of a triple that should be
ignored.
*******************************************************************************/
struct any{
   bool operator==(Node_id const&) const {return true;}
   bool operator==(Doc_id const&) const {return true;}
};

/** Define return type of triple search based on search argument types
@tparam Store triple store type, e.g., Triple_map<>, Triple_store
@tparam Subj type for matching/ignoring triple subject,   e.g., Node_id or any
@tparam Pred type for matching/ignoring triple predicate, e.g., Node_id or any
@tparam Obj  type for matching/ignoring triple object,    e.g., Node_id or any
@tparam Doc  type for matching/ignoring triple document,  e.g., Doc_id  or any
*******************************************************************************/
template<class Subj, class Pred, class Obj, class Doc, class Store = Triple_map<> >
class Search {
   typedef detail::Search_config<typename Store::config,Subj,Pred,Obj,Doc> config;
public:
   typedef typename config::range range;
   typedef typename config::iterator iterator;
};

/** Deduce return type of triple search based on boolean template arguments
indicating which IDs were specified
@tparam Subj true if Node_id was specified to match triple subject,   or false for any
@tparam Pred true if Node_id was specified to match triple predicate, or false for any
@tparam Obj  true if Node_id was specified to match triple object,    or false for any
@tparam Doc  true if Doc_id  was specified to match triple document,  or false for any
@tparam Store triple store type, e.g., Triple_map<>, Triple_store
*******************************************************************************/
template<bool Subj, bool Pred, bool Obj, bool Doc, class Store = Triple_map<> >
class Search_b {
   typedef typename detail::Deduce_args<Subj,Pred,Obj,Doc>::type q_args;
   typedef Search<
            typename boost::mpl::at_c<q_args,0>::type,
            typename boost::mpl::at_c<q_args,1>::type,
            typename boost::mpl::at_c<q_args,2>::type,
            typename boost::mpl::at_c<q_args,3>::type,
            Store
   > search;
public:
   typedef typename search::iterator iterator;
   typedef typename search::range range;
};

/**@brief Store, index, and search RDF triples
*******************************************************************************/
template<bool Index_subj, bool Index_pred, bool Index_obj, bool Index_doc>
class Triple_map {

   template<class, class, class, class, class> friend class Search;

   typedef Triple_map self_t;

   typedef typename
            detail::Store_config<Index_subj,Index_pred,Index_obj,Index_doc>
   config;

   typedef typename config::store store;
   typedef typename boost::mpl::front<store>::type main_store;
   BOOST_MPL_ASSERT((boost::is_same<typename main_store::tag, detail::Main_store_tag>));


   class Insert {
   public:
      Insert(Triple const& t) : t_(t) {}
      template<class Ind> void operator()(Ind& i) const {i.add(t_);}
   private:
      Triple const& t_;
   };

public:
   typedef typename main_store::iterator iterator;
   typedef typename main_store::const_iterator const_iterator;

   /**
    @return number of stored triples
   */
   std::size_t size() const {return boost::fusion::front(store_).get_range().size();}
   const_iterator begin() const {return boost::fusion::front(store_).get_range().begin();}
   const_iterator end() const {return boost::fusion::front(store_).get_range().end();}

   void clear() {
//      triples_.clear();
      //todo
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
    @code template<bool Subj, bool Pred, bool Obj, bool Doc> class Search_b; @endcode
    For example,
    @code Search_b<1,0,0,1>::range_t range = triple_map.find(subj, any(), any(), doc);
    @endcode
   */
   template<class Subj, class Pred, class Obj, class Doc>
   typename Search<Subj,Pred,Obj,Doc,self_t>::range
   find(const Subj subj, const Pred pred, const Obj obj, const Doc doc) const {
   /*
   - find search index
   - form predicate
   - get index iterator range
   - make filter iterator range
    */
      typedef detail::Search_config<config,Subj,Pred,Obj,Doc> search_config;
      typedef typename search_config::index_num index_num;
      typedef typename boost::mpl::at<store, index_num>::type index_t;
      index_t const& index = boost::fusion::at<index_num>(store_);
      typedef typename index_t::const_range range1_t;
      const range1_t r1 = detail::Search_range1<index_t>::get(index,subj,pred,obj,doc);
      return search_config::search::get(r1, subj, pred, obj, doc);
   }

private:
   store store_;

};

}//namespace owlcpp
#endif /* TRIPLE_MAP_HPP_ */
