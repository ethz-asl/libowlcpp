/** @file "/owlcpp/include/owlcpp/rdf/map_triple.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_TRIPLE_HPP_
#define MAP_TRIPLE_HPP_
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

#include "owlcpp/rdf/detail/map_triple_config.hpp"
#include "owlcpp/rdf/detail/triple_index.hpp"

namespace owlcpp{

/**@brief Store, index, and search RDF triples
*******************************************************************************/
template<bool Index_subj, bool Index_pred, bool Index_obj, bool Index_doc>
class Map_triple {

   typedef typename
            map_triple_detail::Store_config<Index_subj,Index_pred,Index_obj,Index_doc>
   config;

   typedef typename config::store store;
   typedef typename boost::mpl::front<store>::type main_store;
   BOOST_MPL_ASSERT((boost::is_same<typename main_store::tag, map_triple_detail::Main_store_tag>));

   class Insert {
   public:
      Insert(Triple const& t) : t_(t) {}
      template<class Ind> void operator()(Ind& i) const {i.add(t_);}
   private:
      Triple const& t_;
   };

   struct Clear {
      template<class Ind> void operator()(Ind& i) const {i.clear();}
   };

   class Erase {
   public:
      Erase(Triple const& t) : t_(t) {}
      template<class Ind> void operator()(Ind& i) const {i.erase(t_);}
   private:
      const Triple t_;
   };

public:
   typedef typename main_store::iterator iterator;
   typedef typename main_store::const_iterator const_iterator;

   /** Define return type of triple search based on search argument types
   @tparam Subj type for matching/ignoring triple subject,   e.g., Node_id or any
   @tparam Pred type for matching/ignoring triple predicate, e.g., Node_id or any
   @tparam Obj  type for matching/ignoring triple object,    e.g., Node_id or any
   @tparam Doc  type for matching/ignoring triple document,  e.g., Doc_id  or any
   *******************************************************************************/
   template<class Subj, class Pred, class Obj, class Doc> struct result {
      typedef typename
               map_triple_detail::Search_config<config,Subj,Pred,Obj,Doc>::range
               type;
   };

   /** Deduce return type of triple search based on boolean template arguments
   indicating which IDs were specified
   @tparam Subj true if Node_id was specified to match triple subject,   or false for any
   @tparam Pred true if Node_id was specified to match triple predicate, or false for any
   @tparam Obj  true if Node_id was specified to match triple object,    or false for any
   @tparam Doc  true if Doc_id  was specified to match triple document,  or false for any
   *******************************************************************************/
   template<bool Subj, bool Pred, bool Obj, bool Doc>
   class result_b {
      typedef typename map_triple_detail::Deduce_args<Subj,Pred,Obj,Doc>::type
               q_args;

   public:
      typedef typename result<
               typename boost::mpl::at_c<q_args,0>::type,
               typename boost::mpl::at_c<q_args,1>::type,
               typename boost::mpl::at_c<q_args,2>::type,
               typename boost::mpl::at_c<q_args,3>::type
      >::type type;
   };


   /** @return number of stored triples */
   std::size_t size() const {return boost::fusion::front(store_).get_range().size();}
   const_iterator begin() const {return boost::fusion::front(store_).get_range().begin();}
   const_iterator end() const {return boost::fusion::front(store_).get_range().end();}

   void clear() {
      Clear c;
      boost::fusion::for_each(store_, c);
   }

   /** @brief erase one triple
    @param t triple stored in triple map
    @throw Rdf_err exception if triple is not found
   */
   void erase(Triple const& t) {
      Erase e(t);
      boost::fusion::for_each(store_, e);
   }

   /**@brief Insert a new triple
   */
   void insert(
            const Node_id subj,
            const Node_id pred,
            const Node_id obj,
            const Doc_id doc
   ) {
      const Triple t(subj, pred, obj, doc);
      Insert ins(t);
      boost::fusion::for_each(store_, ins);
   }


   /**@brief Search triples by subject, predicate, object, or document IDs.
    @details Polymorphically search stored triples to find ones that match
    specified node IDs for subject, predicate, or object nodes or document ID.
    An instance of \b any matches all values for the corresponding triple
    element.
    If none of the nodes are specified, i.e., <tt>find(any(), any(), any(), any())</tt>,
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
    @code template<class Subj, class Pred, class Obj, class Doc> class result; @endcode
    or from
    @code template<bool Subj, bool Pred, bool Obj, bool Doc> class result_b; @endcode
    For example,
    @code Triple_map<>::result_b<1,0,0,1>::type range = triple_map.find(subj, any(), any(), doc);
    @endcode
   */
   template<class Subj, class Pred, class Obj, class Doc>
   typename result<Subj,Pred,Obj,Doc>::type
   find(const Subj subj, const Pred pred, const Obj obj, const Doc doc) const {
   /*
   - find search index
   - form predicate
   - get index iterator range
   - make filter iterator range
    */
      typedef map_triple_detail::Search_config<config,Subj,Pred,Obj,Doc> search_config;
      typedef typename search_config::index_num index_num;
      typedef typename boost::mpl::at<store, index_num>::type index_t;
      index_t const& index = boost::fusion::at<index_num>(store_);
      typedef typename index_t::const_range range1_t;
      const range1_t r1 = map_triple_detail::Search_range1<index_t>::get(index,subj,pred,obj,doc);
      return search_config::search::get(r1, subj, pred, obj, doc);
   }

private:
   store store_;

};

}//namespace owlcpp
#endif /* MAP_TRIPLE_HPP_ */
