/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_index.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_INDEX_HPP_
#define TRIPLE_INDEX_HPP_
#include <map>
#include <vector>
#include "boost/assert.hpp"
#include "boost/fusion/container/vector.hpp"
#include "boost/fusion/include/for_each.hpp"
#include "boost/iterator/indirect_iterator.hpp"
#include "boost/mpl/assert.hpp"
#include "boost/mpl/at.hpp"
#include "boost/mpl/fold.hpp"
#include "boost/mpl/if.hpp"
#include "boost/mpl/vector.hpp"
#include "boost/mpl/vector_c.hpp"
#include "boost/mpl/size.hpp"
#include "boost/range.hpp"

#include "owlcpp/rdf/triple.hpp"
#include "owlcpp/rdf/detail/triple_query_tags.hpp"
#include "owlcpp/rdf/detail/adapt_triple_fusion.hpp"

namespace owlcpp{ namespace query_detail{
namespace mpl = boost::mpl;
namespace fusion = boost::fusion;

struct Element_index {
   typedef std::vector<Triple const*> type;
   typedef boost::indirect_iterator<type::const_iterator> iterator;
   typedef boost::iterator_range<iterator> range;
};

template<class Id> class Vector_index {
   typedef std::vector<Element_index::type> stor_t;
public:

   void add(const Id id, Triple const& t) {
      if( id() >= stor_.size() ) stor_.resize(id() + 1);
      stor_[id()].push_back(&t);
   }

private:
   stor_t stor_;
};

template<class Id> class Map_index {
   typedef std::map<Id, Element_index::type> stor_t;
public:
   void add(const Id id, Triple const& t) {
      stor_[id].push_back(&t);
   }

private:
   stor_t stor_;
};

template<class Tag> struct Index : private Vector_index {
   void add(Triple const& t) { Vector_index::add(fusion::at<Tag>(t), t); }
};

template<> struct Index<Pred_tag> : private Map_index {
//   void add(Triple const& t) { Vector_index::add(fusion::at<Tag>(t), t); }
};

/*
template<> struct Index<Subj_tag> : public Vector_index {
   void add(Triple const& t) {

   }
};

template<> struct Index<Pred_tag> : public Vector_index {

};

template<> struct Index<Obj_tag> : public Vector_index {

};

template<> struct Index<Doc_tag> : public Vector_index {

};
*/

typedef mpl::vector<Subj_tag, Pred_tag, Obj_tag, Doc_tag> all_tags;

/** For each true template argument, put the corresponding tag into MPL vector
*******************************************************************************/
template<bool Subj, bool Pred, bool Obj, bool Doc> struct Tag_vector {
   typedef mpl::vector_c<bool,Subj,Pred,Obj,Doc> bool_vector;
   typedef typename mpl::fold<
            all_tags,
            mpl::vector<>,
            mpl::if_<
               mpl::at<bool_vector, mpl::_2>,
               mpl::push_back<mpl::_1, mpl::_2>,
               mpl::_1
            >
   >::type type;
};

/**
*******************************************************************************/
template<bool Subj, bool Pred, bool Obj, bool Doc> class Triple_indices {
   typedef typename Tag_vector<Subj, Pred, Obj, Doc>::type tags_t;
   typedef typename mpl::fold<
            tags_t,
            fusion::vector<>,
            mpl::push_back<mpl::_1, Index<mpl::_2> >
   >::type stor_t;

   class Add_to_index {
   public:
      Add_to_index(Triple const& t) : t_(t) {}
      template<class Ind> void operator()(Ind& i) const {i.add(t_);}
   private:
      Triple const& t_;
   };

public:

   void add(Triple const& t) {
      Add_to_index ati(t);
      fusion::for_each(stor_, ati);
   }

   void clear() {
      //todo
   }

private:
   stor_t stor_;
};

}//namespace query_detail
}//namespace owlcpp
#endif /* TRIPLE_INDEX_HPP_ */
