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
#include "boost/fusion/include/at.hpp"
#include "boost/fusion/include/value_at.hpp"
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
   typedef boost::indirect_iterator<e_index::iterator> iterator;
   typedef boost::indirect_iterator<e_index::const_iterator> const_iterator;
   typedef boost::iterator_range<iterator> range;
   typedef boost::iterator_range<const_iterator> const_range;
};

/**
*******************************************************************************/
template<class Id> class Vector_index : public Element_index {
   typedef std::vector<e_index> stor_t;

public:
   const_range find(const Id id) const {return stor_[id()];}

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
   const_range find(const Id id) const {
      const typename stor_t::const_iterator i = stor_.find(id);
      if( i == stor_.end() ) return const_range();
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
template<class Tag> class Index
: public Vector_index<typename fusion_rof::value_at<Triple,Tag>::type> {

   typedef Vector_index<typename fusion_rof::value_at<Triple,Tag>::type> base;

public:
   typedef Tag tag;

   void add(Triple const& t) { base::add(fusion::at<tag>(t), t); }
};

/**
*******************************************************************************/
template<> struct Index<Pred_tag>
: public Map_index<fusion_rof::value_at<Triple,Pred_tag>::type> {
   typedef Pred_tag tag;
   typedef Map_index<fusion_rof::value_at<Triple,tag>::type> base;

   void add(Triple const& t) { base::add(fusion::at<tag>(t), t); }
};


/** Container that owns heap-allocated triples

@decision Store triple pointers in std::vector.
Alternatives:
- ptr_vector tracks ownership; ptr_vector<const Triple> does not work
- std::vector<Triple const*> works; same storage as in search indices
*******************************************************************************/
template<> struct Index<Main_store_tag> : public Element_index {
public:
   typedef Main_store_tag tag;

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

   const_range get_range() const { return stor_; }

private:
   e_index stor_;
};


}//namespace detail
}//namespace owlcpp
#endif /* TRIPLE_INDEX_HPP_ */
