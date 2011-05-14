/** @file "/owl_cpp/include/owl_cpp/custom_terms.hpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#ifndef CUSTOM_TERMS_HPP_
#define CUSTOM_TERMS_HPP_

#include "boost/mpl/for_each.hpp"
namespace bmp = boost::mpl;

#include "terms/custom_type_macro.hpp"
#include "terms/term_methods.hpp"
#include "triple_store.hpp"
#include "query_nodes.hpp"

namespace owl_cpp{ namespace detail{
/**
*******************************************************************************/
class Store_namespaces {
   mutable Triple_store& store_;
public:
   Store_namespaces(Triple_store& store) : store_(store) {}

   template<class T> void operator()(const T&) const {
      store_.insert_namespace(T::name, T::prefix);
   }
};

/**
*******************************************************************************/
class Store_terms {
   mutable Triple_store& store_;
public:
   Store_terms(Triple_store& store) : store_(store) {}

   template<class T> void operator()(const T& t) const {
      typedef typename T::ns_type ns_type;
      const Ns_id ns_id = store_.insert_namespace(ns_type::name, ns_type::prefix);
      store_.insert_node( ns_id, T::name );
   }
};

}//namespace detail

/**
*******************************************************************************/
template<class Vec> inline void insert_namespaces(Triple_store& store) {
   detail::Store_namespaces sns(store);
   bmp::for_each<Vec>(sns);
}

/**
*******************************************************************************/
template<class Vec> inline void insert_terms(Triple_store& store) {
   detail::Store_terms st(store);
   bmp::for_each<Vec>(st);
}

/**
*******************************************************************************/
template<class TermTag>inline Node_id find_node(const TermTag&, const Triple_store& store) {
   typedef typename TermTag::ns_type ns_type;
   return find_node(ns_type::name, TermTag::name, store);
}

}//namespace owl_cpp

#endif /* CUSTOM_TERMS_HPP_ */
