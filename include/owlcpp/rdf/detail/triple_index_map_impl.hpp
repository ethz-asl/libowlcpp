/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_index_map_impl.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef TRIPLE_INDEX_MAP_IMPL_HPP_
#define TRIPLE_INDEX_MAP_IMPL_HPP_
#include <map>
#include "boost/foreach.hpp"
#include "boost/fusion/sequence/intrinsic/at.hpp"
#include "boost/iterator/filter_iterator.hpp"
#include "boost/range.hpp"

#include "owlcpp/rdf/any_triple_element.hpp"
#include "owlcpp/rdf/detail/triple_set.hpp"
#include "owlcpp/rdf/exception.hpp"

namespace owlcpp{ namespace map_triple_detail{

/**@brief Fragment map vector configuration
*******************************************************************************/
template<class Tag0,class Tag1, class Tag2, class Tag3> struct Tim_config {
   typedef typename boost::mpl::at<Triple, Tag0>::type id_type;
   typedef Triple_set<Tag1,Tag2,Tag3> set_type;
   typedef std::pair<id_type, set_type> value_type;
   typedef std::map<id_type, set_type> storage;
   typedef typename storage::const_iterator iterator;
};

/**@brief
*******************************************************************************/
template<class Tag0,class Tag1, class Tag2, class Tag3, class Q0>
class Tim_query_dispatch {
   typedef Tim_config<Tag0,Tag1,Tag2,Tag3> config;
   typedef typename config::storage storage;
   typedef typename config::value_type value_type;

   class Equal : public std::unary_function<value_type, bool> {
   public:
      explicit Equal(const Q0 q) : q_(q) {}
      bool operator()(value_type const& p) const {return p.first == q_;}
   private:
      Q0 q_;
   };

public:
   typedef boost::filter_iterator<Equal, typename config::iterator> iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(storage const& s, const Q0 q) {
      const Equal eq(q);
      return range(
               iterator(eq, s.begin(), s.end()),
               iterator(eq, s.end(), s.end())
      );
   }
};

template<class Tag0,class Tag1, class Tag2, class Tag3>
class Tim_query_dispatch<Tag0,Tag1,Tag2,Tag3,Any> {
   typedef Tim_config<Tag0,Tag1,Tag2,Tag3> config;
   typedef typename config::storage storage;
public:
   typedef typename config::iterator iterator;
   typedef boost::iterator_range<iterator> range;

   static range find(storage const& s, Any const&) {
      return s;
   }
};

/**@brief 
*******************************************************************************/
template<class Tag0,class Tag1, class Tag2, class Tag3>
class Triple_index_map_impl {
   typedef Tim_config<Tag0,Tag1,Tag2,Tag3> config;
   typedef typename config::storage storage;
   typedef typename config::id_type id_type;
public:
   typedef typename config::set_type set_type;
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

   bool insert(Triple const& t) {
      const id_type id = boost::fusion::at<Tag0>(t);
      return s_[id].insert(t);
   }

   void erase(Triple const& t) {
      const id_type id = boost::fusion::at<Tag0>(t);
      const_iterator i = s_.find(id);
      if( i == s_.end() ) BOOST_THROW_EXCEPTION(
               Rdf_err()
               << Rdf_err::msg_t("element not found")
               << Rdf_err::int1_t(id())
      );
      i->second.erase(t);
   }

   void clear() {s_.clear();}

   set_type const& operator[](const id_type id) const {
      const_iterator i = s_.find(id);
      if( i == s_.end() ) return set_type::empty_set();
      return i->second;
   }

   template<class Q0> struct query {
      typedef typename Tim_query_dispatch<Tag0,Tag1,Tag2,Tag3,Q0>::iterator iterator;
      typedef typename Tim_query_dispatch<Tag0,Tag1,Tag2,Tag3,Q0>::range range;
   };

   template<class Q0> typename query<Q0>::range
   find(Q0 const& q) const {
      return Tim_query_dispatch<Tag0,Tag1,Tag2,Tag3,Q0>::find(s_, q);
   }

private:
   storage s_;
};

}//namespace map_triple_detail
}//namespace owlcpp
#endif /* TRIPLE_INDEX_MAP_IMPL_HPP_ */
