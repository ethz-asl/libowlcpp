/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_index.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_INDEX_HPP_
#define TRIPLE_INDEX_HPP_
#include <map>
#include <vector>

#include "boost/fusion/include/at.hpp"
#include "boost/fusion/include/value_at.hpp"
#include "boost/range.hpp"
#include "boost/range/algorithm/find.hpp"

#include "owlcpp/rdf/detail/map_triple_tags.hpp"
#include "owlcpp/rdf/exception.hpp"

namespace owlcpp{ namespace map_triple_detail{
namespace fusion = boost::fusion;
namespace fusion_rof = boost::fusion::result_of;

/**
@decision Store and index triples by value in std::vector<Triple>.\n
Alternatives and RAM/time performance for adding many triples:
- boost::multi_index 2GB/150s
- ptr_vector tracks ownership; ptr_vector<const Triple> does not work
- boost::ptr_vector<Triple> 1.1GB/98s
- std::vector<Triple> 0.921GB/91s
- std::vector<Triple> + subj. index 1.2GB/97s

@decision Indexing triples.
Options and RAM/time performance:
- boost::multi_index 2GB/150s
- std::vector<Triple>, no index 0.921GB/91s
- boost::ptr_vector<Triple>, no index 1.1GB/98s
- boost::ptr_vector<Triple>, vector<vector> empty 1.4GB/98s
- boost::ptr_vector<Triple>, vector<vector> 1.7GB/110s
*******************************************************************************/
class Element_index {
protected:
   typedef std::vector<Triple> e_index;
public:
   typedef e_index::iterator iterator;
   typedef e_index::const_iterator const_iterator;
   typedef boost::iterator_range<iterator> range;
   typedef boost::iterator_range<const_iterator> const_range;
   struct Err : public Rdf_err {};
protected:
   static const_iterator null_iter() {
      static e_index v;
	  return v.end();
   }
};

/**
*******************************************************************************/
template<class Id> class Vector_index : public Element_index {
   typedef std::vector<e_index> stor_t;

public:
   const_range find(const Id id) const {
      if( stor_.size() > id() ) return stor_[id()];
      return const_range(null_iter(),null_iter());
   }

   void clear() {stor_.clear();}

protected:
   void add(const Id id, Triple const& t) {
      if( id() >= stor_.size() ) stor_.resize(id() + 1);
      stor_[id()].push_back(t);
   }

   void erase(const Id id, Triple const& t) {
      BOOST_ASSERT(stor_.size() > id());
      e_index& ind = stor_[id()];
      ind.erase(boost::find(ind, t));
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
      if( i != stor_.end() ) return i->second;
	  return const_range(null_iter(),null_iter());
   }

   void clear() {stor_.clear();}

protected:
   void add(const Id id, Triple const& t) {
      stor_[id].push_back(t);
   }

   void erase(const Id id, Triple const& t) {
      const typename stor_t::iterator i = stor_.find(id);
      BOOST_ASSERT(i != stor_.end());
      e_index& ind = i->second;
      ind.erase(boost::find(ind, t));
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
   void erase(Triple const& t) { base::erase(fusion::at<tag>(t), t); }
};

/**
*******************************************************************************/
template<> struct Index<Pred_tag>
: public Map_index<fusion_rof::value_at<Triple,Pred_tag>::type> {
   typedef Pred_tag tag;
   typedef Map_index<fusion_rof::value_at<Triple,tag>::type> base;

   void add(Triple const& t) { base::add(fusion::at<tag>(t), t); }
   void erase(Triple const& t) { base::erase(fusion::at<tag>(t), t); }
};


/** Main contiguous storage of all triples
@decision Store all triples by value in std::vector.\n
Alternatives:
- store triples only in one or many indices:
   order is not preserved,
   iterating over all triples is slower
- track order of triples in vector<pair<uint,uint>>,
where pair<uint,uint> gives position of the triple in the subject index
*******************************************************************************/
template<> struct Index<Main_store_tag> : public Element_index {
public:
   typedef Main_store_tag tag;

   void add(Triple const& t) {
      stor_.push_back(t);
   }

   void clear() {stor_.clear();}

   void erase(Triple const& t) {
      const iterator i = boost::find(stor_, t);
      if( i == stor_.end()) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("triple not found")
      );
      stor_.erase(i);
   }

   const_range get_range() const { return stor_; }

private:
   e_index stor_;
};


}//namespace map_triple_detail
}//namespace owlcpp
#endif /* TRIPLE_INDEX_HPP_ */
