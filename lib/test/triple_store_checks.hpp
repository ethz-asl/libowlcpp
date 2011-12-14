/** @file "/owl_cpp/lib/test/triple_store_checks.hpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef TRIPLE_STORE_CHECKS_HPP_
#define TRIPLE_STORE_CHECKS_HPP_
#include <set>
#include "type_vector.hpp"
#include "boost/mpl/for_each.hpp"
namespace bmp = boost::mpl;
#include "boost/foreach.hpp"

#include "owl_cpp/exception.hpp"
namespace ot = owl_cpp::terms;
#include "owl_cpp/rdf/triple_store.hpp"
#include "owl_cpp/rdf/query_nodes.hpp"

namespace owl_cpp { namespace test{ namespace detail{

/**
*******************************************************************************/
class Check_std_iri {
   const Triple_store& store_;
public:
   struct Err : public base_exception {};

   Check_std_iri(const Triple_store& store) : store_(store) {}

   template<class T> void operator()(const T& t) const {
      if( store_[T::name] != T::id() ) {
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("wrong ID retrieved")
               << Err::str1_t(T::name)
         );
      }

      if( store_[T::id()] != T::name ) {
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("wrong name retrieved")
               << Err::str1_t(T::name)
         );
      }

      if( store_.prefix(T::id()) != T::prefix ) {
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("wrong prefix retrieved")
               << Err::str1_t(T::prefix)
         );
      }

      if( store_.prefix_id(T::prefix) != (T::id()) ) {
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("wrong ID retrieved")
               << Err::str1_t(T::prefix)
         );
      }
   }
};

}//namespace detail

/**
*******************************************************************************/
void check_std_namespaces(const Triple_store& store) {
   detail::Check_std_iri csi(store);
   bmp::for_each<ot::mpl_vector_namespaces_all_t>(csi);
}

namespace detail{

/**
*******************************************************************************/
class Check_std_term {
   const Triple_store& store_;
public:
   struct Err : public base_exception {};

   Check_std_term(const Triple_store& store) : store_(store) {}

   template<class T> void operator()(const T& t) const {
      typedef typename T::ns_type ns_type;
      if( find_node(ns_type::name, T::name, store_) != T::id() ) {
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("wrong ID retrieved")
               << Err::str1_t(T::name)
         );
      }

      if( store_[T::id()].value_str() != T::name ) {
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("wrong name retrieved")
               << Err::str1_t(T::name)
         );
      }

      if( store_[T::id()].ns_id() != T::ns_type::id() ) {
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("wrong namespace ID retrieved")
               << Err::str1_t(T::name)
         );
      }

      if( short_name(T::id(), store_) != ns_type::prefix + ":" + T::name ) {
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("wrong short name retrieved")
               << Err::str1_t(short_name(T::id(), store_))
         );
      }
   }
};

}//namespace detail

/**
*******************************************************************************/
void check_std_terms(const Triple_store& store) {
   detail::Check_std_term csi(store);
   bmp::for_each<ot::mpl_vector_terms_rdfs_t>(csi);
   bmp::for_each<ot::mpl_vector_terms_rdf_t >(csi);
   bmp::for_each<ot::mpl_vector_terms_owl1_t >(csi);
   bmp::for_each<ot::mpl_vector_terms_owl2_t >(csi);
}

/**
*******************************************************************************/
void check_node_ranges(const Triple_store& store) {
   std::set<Node_id> ids;
   BOOST_FOREACH(const Node_id id, store.node_ids()) {
      if( ! ids.insert(id).second )
         BOOST_THROW_EXCEPTION(
            base_exception()
            << base_exception::msg_t("duplicate node ID")
            << base_exception::str1_t(short_name(id, store))
      );
      store[id];
   }

   BOOST_FOREACH(const Node_base& node, store.nodes()) {
      const Node_id id = store[node];
      if( ids.erase(id) == 0 )
         BOOST_THROW_EXCEPTION(
            base_exception()
            << base_exception::msg_t("node ID not found")
            << base_exception::str1_t(short_name(id, store))
      );
   }

   if( ! ids.empty() )
      BOOST_THROW_EXCEPTION(
            base_exception()
            << base_exception::msg_t("no object for ID")
            << base_exception::str1_t( short_name(*ids.begin(), store) )
      );
}

/**
*******************************************************************************/
inline void check(const Triple_store& store) {
   try{
      check_std_namespaces(store);
      check_std_terms(store);
      check_node_ranges(store);
   } catch(detail::Check_std_iri::Err& e) {
      BOOST_THROW_EXCEPTION(
            base_exception()
            << base_exception::msg_t("error in namespaces")
            << base_exception::nested_t(b::copy_exception(e))
      );
   } catch(detail::Check_std_term::Err& e) {
      BOOST_THROW_EXCEPTION(
            base_exception()
            << base_exception::msg_t("error in terms")
            << base_exception::nested_t(b::copy_exception(e))
      );
   }
}

/**
*******************************************************************************/
inline bool check_and_print(const Triple_store& store, std::ostream& stream) {
   try{
      check(store);
   } catch(...) {
      stream
      << std::endl << b::current_exception_diagnostic_information() << std::endl;
      return false;
   }
   return true;
}

}//namespace test
}//namespace owl_cpp

#endif /* TRIPLE_STORE_CHECKS_HPP_ */
