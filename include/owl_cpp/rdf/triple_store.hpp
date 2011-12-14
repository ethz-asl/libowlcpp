/** @file "/owl_cpp/include/owl_cpp/rdf/triple_store.hpp"
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef TRIPLE_STORE_HPP_
#define TRIPLE_STORE_HPP_
#include <string>
#include <vector>
#include "boost/tuple/tuple.hpp"
#include "boost/range.hpp"
#include "boost/iterator/indirect_iterator.hpp"

#include "owl_cpp/config.hpp"
#include "owl_cpp/exception.hpp"
#include "owl_cpp/rdf/object_store.hpp"
#include "owl_cpp/ns_id.hpp"
#include "owl_cpp/node_id.hpp"
#include "owl_cpp/node_base.hpp"

namespace owl_cpp{
class Triple;
namespace detail{
template<class> struct Range_retriever;
}//namespace detail

/** Specialize traits for storing URI strings
*******************************************************************************/
template<> struct Stored_obj_traits<std::string> {
   typedef std::string obj_t;
   typedef Ns_id id_t;
};

/** Store ontology information as URIs, nodes, and RDF triples.
Access objects through opaque object IDs

@details Fetching objects based on IDs is fast and unsafe:
result of fetching with invalid ID is undetermined.
Fetching ID for a non-existing object produces exception.
*******************************************************************************/
class OWLCPP_DECL Triple_store {
   typedef Obj_store<std::string> uri_store_t;
   typedef Obj_store<Node_base> term_store_t;
   typedef std::vector<Triple*> triple_store_t;
   typedef std::vector<triple_store_t> tid_store_t;
   typedef boost::indirect_iterator<triple_store_t::const_iterator, Triple>
      triple_iter_t;

public:
   struct Err : public base_exception {};
   typedef boost::iterator_range<triple_iter_t> triple_range_t;
   typedef uri_store_t::find_range_t ns_search_range_t;
   typedef term_store_t::find_range_t node_search_range_t;

   /**@brief Split IRI into namespace IRI and term.
   Use only this splitting procedure for consistency.
   */
   static boost::tuple<std::string,std::string> split(const std::string&);

   Triple_store();
   std::size_t n_uris() const {return uris_.size();}
   std::size_t n_nodes() const {return nodes_.size();}
   std::size_t n_triples() const {return triples_.size();}

   const std::string& operator[](const Ns_id ns_id) const {return uris_.get(ns_id);}
   const Node_base& operator[](const Node_id id) const {return nodes_.get(id);}
   ns_search_range_t find(const std::string& ns) const {return uris_.find(ns);}
   node_search_range_t find(const Node_base& node) const {return nodes_.find(node);}

   /**
    * @param ns namespace string
    * @return namespace ID
    */
   Ns_id operator[](const std::string& ns) const {
      uri_store_t::find_range_t r = uris_.find(ns);
      if( ! r ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unknown IRI")
            << Err::str1_t(ns)
      );
      return r.front().second;
   }

   Node_id operator[](const Node_base& node) const {
      term_store_t::find_range_t r = nodes_.find(node);
      if( ! r ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unknown term")
            << Err::str1_t( operator[](node.ns_id()) )
            << Err::str2_t( node.value_str() )
      );
      return r.front().second;
   }

   /**@return true if ontology with IRI iri is in store  */
//   bool is_loaded(const std::string& iri) const;

   typedef uri_store_t::obj_range_t ns_range_t;
   ns_range_t namespaces() const {return uris_.objects();}

   typedef uri_store_t::id_range_t ns_id_range_t;
   ns_id_range_t namespace_ids() const {return uris_.object_ids();}

   typedef term_store_t::obj_range_t node_range_t;
   node_range_t nodes() const {return nodes_.objects();}

   typedef term_store_t::id_range_t node_id_range_t;
   node_id_range_t node_ids() const {return nodes_.object_ids();}

   triple_range_t triples() const;


   /**
    * @param iri namespace IRI
    * @param prefix unique prefix to use with IRI
    */
   Ns_id insert_namespace(const std::string& iri, const std::string& prefix = "");
   Node_id insert_node(const Ns_id ns_id, const std::string& name);

   /** Generate triple of appropriate type and insert into store */
   Triple* insert(
         const Node_id subj_id,
         const Node_id pred_id,
         const Node_id obj_id
   );

   /**@return unique namespace prefix */
   std::string prefix(const Ns_id id) const {return prefixes_.get(id);}

   /**@return ID of namespace prefix */
   Ns_id prefix_id(const std::string& pref) const {
      uri_store_t::find_range_t r = prefixes_.find(pref);
      if( ! r ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unknown IRI prefix")
            << Err::str1_t(pref)
      );
      return r.front().second;
   }

   ~Triple_store();

   template<class T> Node_id insert_node(const T& node) {
      //all standard terms must be already inserted
      if( is_standard(node.ns_id()) && ! nodes_.find(node) )
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unknown term from a standard namespace")
               << Err::str1_t( node.value_str() )
               << Err::str2_t( operator[](node.ns_id()) )
         );

      const Node_id nid = nodes_.insert(node);
      if( subjects_.size() <= nid()) subjects_.resize(nid() + 1, triple_store_t(0));
      return nid;
   }

private:
   uri_store_t uris_;
   uri_store_t prefixes_;
   term_store_t nodes_;
   triple_store_t triples_;

   /**for each node store triple IDs in which it is the subject */
   tid_store_t subjects_;

   /** Insert standard OWL namespace URIs into the store;
    * used only during construction
    */
   void enter_standard_uris();

   /** Insert standard OWL term nodes into the store;
    * used only during construction
    */
   void enter_standard_terms();

   /**@brief Retrieve a range of triples from triple store */
   template<class> friend struct ::owl_cpp::detail::Range_retriever;

   /**
    * @param nid
    * @return range of RDF triples that have nid as subject
    */
   triple_range_t subject_in(const Node_id nid) const;

   bool is_standard(const Ns_id ns_id) const;

   /** throw exception if nid already exists as ontology IRI or version */
   void check_id(const Node_id nid) const;
};

}//namespace owl_cpp

#endif /* TRIPLE_STORE_HPP_ */
