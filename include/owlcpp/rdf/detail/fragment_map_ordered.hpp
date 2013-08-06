/** @file "/owlcpp/include/owlcpp/rdf/detail/fragment_map_ordered.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef FRAGMENT_MAP_ORDERED_HPP_
#define FRAGMENT_MAP_ORDERED_HPP_
#include <map>
#include "boost/foreach.hpp"
#include "boost/iterator/filter_iterator.hpp"
#include "boost/range.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/rdf/any_triple_element.hpp"

namespace owlcpp{ namespace map_triple_detail{

/**@brief Fragment map vector configuration
*******************************************************************************/
template<class Id, class Set> struct Fmo_config {
   typedef typename Set::value_type fragment;
   typedef std::pair<Id, Set const&> value_type;
   typedef std::map<Id, Set> storage;
   typedef typename storage::const_iterator iterator;
};

/**@brief
*******************************************************************************/
template<class Id, class Set, class Q> class Fmo_search_dispatch {
   typedef Fmo_config<Id,Set> config;
   typedef typename config::storage storage;
   typedef typename config::value_type value_type;

   class Equal : public std::unary_function<value_type, bool> {
   public:
      explicit Equal(const Q q) : q_(q) {}
      bool operator()(value_type const& p) const {return p.first == q_;}
   private:
      Q q_;
   };

public:
   typedef boost::filter_iterator<Equal, typename config::iterator> iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(storage const& s, const Q q) {
      const Equal eq(q);
      return range(
               iterator(eq, s.begin(), s.end()),
               iterator(eq, s.end(), s.end())
      );
   }
};

template<class Id, class Set> class Fmo_search_dispatch<Id,Set,Id> {
   typedef Fmo_config<Id,Set> config;
   typedef typename config::storage storage;
public:
   typedef typename config::iterator iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(storage const& s, const Id id) {
      const iterator i = s.find(id);
      if( i == s.end() ) return range(s.end(), s.end());
      return range(i, i + 1);
   }
};

template<class Id, class Set> class Fmo_search_dispatch<Id,Set,any> {
   typedef Fmo_config<Id,Set> config;
   typedef typename config::storage storage;
public:
   typedef typename config::iterator iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(storage const& s, any const&) {
      return s;
   }
};

/**@brief 
*******************************************************************************/
template<class Id, class Set> class Fragment_map_ordered {
   typedef Fmo_config<Id,Set> config;
   typedef typename config::storage storage;
public:
   typedef typename config::fragment fragment;
   typedef typename config::value_type value_type;
   typedef typename config::iterator iterator;
   typedef iterator const_iterator;
   typedef boost::iterator_range<iterator> range;

   const_iterator begin() const {return s_.begin();}
   const_iterator end() const {return s_.end();}

   std::size_t n_fragments() const {
      std::size_t n = 0;
      BOOST_FOREACH(value_type const& v, s_) {
         n += v.second.size();
      }
      return n;
   }

   bool insert(const Id id, fragment const& f) {return s_[id].insert(f);}

   void erase(const Id id, fragment const& f) {
      const_iterator i = s_.find(id);
      if( i == s_.end() ) BOOST_THROW_EXCEPTION(
               Rdf_err()
               << Rdf_err::msg_t("element not found")
               << Rdf_err::int1_t(id())
      );
      i->second.erase(f);
   }

   void clear() {s_.clear();}

   Set const& operator[](const Id id) const {
      const_iterator i = s_.find(id);
      if( i == s_.end() ) return Set::empty_set();
      return i->second;
   }

   template<class Q> struct query {
      typedef typename Fmo_search_dispatch<Id,Set,Q>::iterator iterator;
      typedef typename Fmo_search_dispatch<Id,Set,Q>::range range;
   };

   template<class Q> typename query<Q>::range
   find(Q const& q) const {
      return Fmo_search_dispatch<Id,Set,Q>::find(s_, q);
   }

private:
   storage s_;
};

}//namespace map_triple_detail
}//namespace owlcpp
#endif /* FRAGMENT_MAP_ORDERED_HPP_ */
