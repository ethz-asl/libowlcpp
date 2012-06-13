/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_index.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_INDEX_HPP_
#define TRIPLE_INDEX_HPP_
#include <map>
#include <vector>

#include "boost/foreach.hpp"
#include "boost/iterator/indirect_iterator.hpp"
#include "boost/ptr_container/ptr_vector.hpp"
#include "boost/range.hpp"

#include "owlcpp/rdf/detail/triple_map_tags.hpp"

namespace owlcpp{ namespace detail{
namespace fusion = boost::fusion;
namespace fusion_rof = boost::fusion::result_of;

/**
*******************************************************************************/
class Element_index {
protected:
   typedef std::vector<Triple const*> e_index;
public:
   typedef boost::indirect_iterator<e_index::const_iterator> iterator;
   typedef boost::iterator_range<iterator> range;
};

/**
*******************************************************************************/
template<class Id> class Vector_index : public Element_index {
   typedef std::vector<e_index> stor_t;

public:
   range find(const Id id) const {return stor_[id()];}

protected:
   void add(const Id id, Triple const& t) {
      if( id() >= stor_.size() ) stor_.resize(id() + 1);
      stor_[id()].push_back(&t);
   }

private:
   stor_t stor_;
};

/**
*******************************************************************************/
template<class Id> class Map_index : public Element_index {
   typedef std::map<Id, e_index> stor_t;
public:
   range find(const Id id) const {
      const typename stor_t::const_iterator i = stor_.find(id);
      if( i == stor_.end() ) return range();
      return i->second;
   }

protected:
   void add(const Id id, Triple const& t) {
      stor_[id].push_back(&t);
   }

private:
   stor_t stor_;
};

/**
*******************************************************************************/
template<class Tag> struct Index
: public Vector_index<typename fusion_rof::value_at<Triple,Tag>::type> {
   void add(Triple const& t) {
      Vector_index<typename fusion_rof::value_at<Triple,Tag>::type>::add(fusion::at<Tag>(t), t);
   }
};

/**
*******************************************************************************/
template<> struct Index<Pred_tag>
: public Map_index<fusion_rof::value_at<Triple,Pred_tag>::type> {
   void add(Triple const& t) {
      Map_index<fusion_rof::value_at<Triple,Pred_tag>::type>::add(fusion::at<Pred_tag>(t), t);
   }
};


/** Container that owns heap-allocated triples

@decision Store triple pointers in std::vector.
Alternatives:
- ptr_vector tracks ownership; ptr_vector<const Triple> does not work
- std::vector<Triple const*> works; same storage as in search indices
*******************************************************************************/
template<> struct Index<Main_store_tag> : public Element_index {
public:
   void add(Triple const& t) {
      stor_.push_back(&t);
   }

   void clear() {
      BOOST_FOREACH(Triple const* t, stor_) delete t;
      stor_.clear();
   }

   ~Index() {
      BOOST_FOREACH(Triple const* t, stor_) delete t;
   }

private:
   e_index stor_;
};


}//namespace detail
}//namespace owlcpp
#endif /* TRIPLE_INDEX_HPP_ */
