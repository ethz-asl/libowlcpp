/** @file "/owlcpp/include/owlcpp/rdf/map_std.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012-3
*******************************************************************************/
#ifndef MAP_STD_HPP_
#define MAP_STD_HPP_
#include "boost/assert.hpp"
#include "boost/range.hpp"
#include "boost/range/algorithm/max_element.hpp"
#include "boost/utility.hpp"
#include "boost/concept/assert.hpp"

#include "owlcpp/rdf/map_ns_prefix.hpp"
#include "owlcpp/rdf/ns_iri.hpp"
#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/rdf/detail/map_id_object.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/terms/detail/max_standard_id.hpp"

namespace owlcpp{

/**@brief Immutable static set of namespace IRIs and nodes
@details Contains at least blank and empty (literal) namespaces and empty node.
*******************************************************************************/
class Map_std : boost::noncopyable {
   typedef detail::Map_id_object<Ns_iri, Ns_id> map_ns_t;
   typedef detail::Map_id_object<Node_iri, Node_id> map_node_t;

   template<class Inserter> explicit Map_std(Inserter const& ins);

public:

   /**@brief Get instance of standard nodes map
    @tparam T SHOULD be callable with IRI map and node map inserting
    standard namespace IRIs and standard nodes.

    @code
   struct Inserter {
   void operator()(Map_std& map) const;
   };
   Map_std const& nms = Map_std::get(Inserter());
   @endcode

   */
   template<class T> static Map_std const& get(T const& t) {
      BOOST_CONCEPT_ASSERT((boost::UnaryFunction<T, void, Map_std&>));
      static const Map_std map(t);
      return map;
   }

   void insert(const Ns_id nsid, Ns_iri const& nsiri, std::string const& pref) {
      BOOST_ASSERT(nsid < detail::min_ns_id());
      map_ns_.insert(nsid, nsiri);
      map_pref_.set(nsid, pref);
   }

   void insert(const Node_id nid, Node_iri const& node) {
      BOOST_ASSERT(nid < detail::min_node_id());
      BOOST_ASSERT(find(node.ns_id()));
      map_node_.insert(nid, node);
   }

   /**
    @param nsid namespace IRI ID
    @return true if nodes should not be added to this namespace by user;
    e.g., in case of RDF or OWL namespaces.
    That is, all nodes from this namespace should reside in this map.
   */
   bool is_standard(const Ns_id nsid) const {
      return nsid < detail::min_ns_id();
   }

   bool is_standard(const Node_id nid) const {
      return nid < detail::min_node_id();
   }

   Ns_id const* find(Ns_iri const& iri) const {return map_ns_.find(iri);}
   Ns_id const* find_prefix(std::string const& pref) const {return map_pref_.find(pref);}
   Ns_iri const& operator[](const Ns_id nsid) const {return map_ns_[nsid];}
   Ns_iri const& at(const Ns_id nsid) const {return map_ns_.at(nsid);}
   Ns_iri const* find(const Ns_id nsid) const {return map_ns_.find(nsid);}
   std::string prefix(const Ns_id nsid) const {return map_pref_.prefix(nsid);}

   Node_iri const& operator[](const Node_id nid) const {return map_node_[nid];}
   Node_iri const& at(const Node_id nid) const {return map_node_.at(nid);}
   Node_iri const* find(const Node_id nid) const {return map_node_.find(nid);}
   Node_id const* find(Node_iri const& node) const {return map_node_.find(node);}

   Node_id const* find(const Ns_id ns, std::string const& val) const {
      if( is_blank(ns) || ns >= detail::min_ns_id() ) return 0;
      return map_node_.find(Node_iri(ns, val));
   }

private:
   map_ns_t map_ns_;
   Map_ns_prefix map_pref_;
   map_node_t map_node_;
};

namespace detail{

/**Insert namespace IRI tags into standard node map
*******************************************************************************/
class Ns_inserter {
public:
   explicit Ns_inserter(Map_std& map) : map_(&map) {}
   template<class NsTag> void operator()(NsTag const&) const {
      map_->insert(NsTag::id(), Ns_iri(NsTag::iri()), NsTag::prefix());
   }
private:
   mutable Map_std* map_;
};

/**Insert node IRI tags into standard node map
*******************************************************************************/
class Node_inserter {
public:
   explicit Node_inserter(Map_std& map) : map_(&map) {}
   template<class NodeTag> void operator()(NodeTag const&) const {
      typedef typename NodeTag::ns_type ns_type;
      map_->insert(ns_type::id(), Ns_iri(ns_type::iri()), ns_type::prefix());
      map_->insert(NodeTag::id(), Node_iri(ns_type::id(), NodeTag::fragment()));
   }
private:
   mutable Map_std* map_;
};
}//namespace detail

template<class Inserter> Map_std::Map_std(Inserter const& ins)
: map_ns_(Ns_id(0)), map_pref_(), map_node_(Node_id(0))
{
   detail::Ns_inserter ns_ins(*this);
   ns_ins(terms::empty());
   ns_ins(terms::blank());
   detail::Node_inserter node_ins(*this);
   node_ins(terms::empty_());

   ins(*this);
}

}//namespace owlcpp
#endif /* MAP_STD_HPP_ */
