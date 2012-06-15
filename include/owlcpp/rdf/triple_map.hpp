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

struct any{};

/** Define return type of triple search based on search argument types
@targ Store triple store type, e.g., Triple_map<>, Triple_store
@targ Subj type for matching/ignoring triple subject,   e.g., Node_id or any
@targ Pred type for matching/ignoring triple predicate, e.g., Node_id or any
@targ Obj  type for matching/ignoring triple object,    e.g., Node_id or any
@targ Doc  type for matching/ignoring triple document,  e.g., Doc_id  or any
*******************************************************************************/
template<class Subj, class Pred, class Obj, class Doc, class Store = Triple_map<> >
class Ts_result {
   typedef detail::Search_config<typename Store::config,Subj,Pred,Obj,Doc> config;
public:
   typedef typename config::range range;
};

/** Define return type of triple search based on which node IDs are specified
@targ Store triple store type, e.g., Triple_map<>, Triple_store
@targ Subj true if Node_id was specified to match triple subject,   or false for any
@targ Pred true if Node_id was specified to match triple predicate, or false for any
@targ Obj  true if Node_id was specified to match triple object,    or false for any
@targ Doc  true if Doc_id  was specified to match triple document,  or false for any
*******************************************************************************/
template<bool Subj, bool Pred, bool Obj, bool Doc, class Store = Triple_map<> >
class Ts_result_i {
   typedef typename detail::Deduce_args<Subj,Pred,Obj,Doc>::type q_args;
public:
   typedef typename Ts_result<
            typename boost::mpl::at_c<q_args,0>::type,
            typename boost::mpl::at_c<q_args,1>::type,
            typename boost::mpl::at_c<q_args,2>::type,
            typename boost::mpl::at_c<q_args,3>::type,
            Store
   >::range range;
};

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
template<bool Index_subj, bool Index_pred, bool Index_obj, bool Index_doc>
class Triple_map {

   template<class, class, class, class, class> friend class Ts_result;

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
    @code template<bool Subj, bool Pred, bool Obj, bool Doc> class Query; @endcode
    For example,
    @code Query<1,0,0,1>::range_t range = triple_map.find(subj, blank(), blank(), doc);
    @endcode

   - find search index
   - form predicate
   - get index iterator range
   - make filter iterator range

   */
   template<class Subj, class Pred, class Obj, class Doc>
   typename Ts_result<Subj,Pred,Obj,Doc,self_t>::range
   find(const Subj subj, const Pred pred, const Obj obj, const Doc doc) const {
      typedef detail::Search_config<config,Subj,Pred,Obj,Doc> search_config;
      typedef typename search_config::index_num index_num;
      typedef typename boost::mpl::at<store, index_num>::type index_t;
//      typedef typename config::index index_t;
      index_t const& index = boost::fusion::at<index_num>(store_);
      typedef typename index_t::const_range range1_t;
      typedef detail::Search_range1<index_t> get_range1_t;
      const range1_t r1 = get_range1_t::get(index,subj,pred,obj,doc);

//      typedef typename detail::Query_ft<Subj,Pred,Obj,Doc>::type q1_t;
//      typedef detail::Search_config<store_tags, q1_t> search_t;
//      typedef typename search_t::index_num index_num;
//      typedef typename boost::fusion::at<index_num>::
//      typedef typename search_t::query2_tags q2_t;

//      typedef query_detail::Query_impl<Subj,Pred,Obj,Doc,Triple_map> q_type;
//      return q_type::make_range(*this, subj, pred, obj, doc);
   }

private:
   store store_;

};

}//namespace owlcpp
#endif /* TRIPLE_MAP_HPP_ */
