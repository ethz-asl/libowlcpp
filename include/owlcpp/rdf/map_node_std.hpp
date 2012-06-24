/** @file "/owlcpp/include/owlcpp/rdf/map_node_std.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_NODE_STD_HPP_
#define MAP_NODE_STD_HPP_
#include "boost/assert.hpp"
#include "boost/range.hpp"
#include "boost/range/algorithm/max_element.hpp"
#include "boost/utility.hpp"

#include "owlcpp/rdf/detail/map_ns_base.hpp"
#include "owlcpp/rdf/detail/map_node_base.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{

/**@brief immutable static set of IRIs and nodes
@details Contains at least blank and empty (literal) namespaces and empty node
*******************************************************************************/
class Map_node_std : boost::noncopyable {
   typedef detail::Map_ns_base map_ns;
   typedef detail::Map_node_base map_node;

   template<class Inserter> explicit Map_node_std(Inserter const& ins)
   : ns_(Ns_id(0)), ns_id_next_(0), node_(Node_id(0)), node_id_next_(0)
   {
      insert_ns_tag(terms::N_empty());
      insert_ns_tag(terms::N_blank());
      insert_node_tag(terms::T_empty_());

      ins(*this);

      BOOST_ASSERT( ! ns_.empty());
      map_ns::const_iterator i = boost::max_element(ns_);
      ns_id_next_ = Ns_id((*i)() + 1);

      BOOST_ASSERT( ! node_.empty() );
      map_node::const_iterator j = boost::max_element(node_);
      node_id_next_ = Node_id((*j)() + 1);
   }

public:

   /**@brief Get instance of standard nodes map
    @tparam T SHOULD be callable with IRI map and node map inserting
    standard namespace IRIs and standard nodes.

    @code
   struct Inserter {
   void operator()(Map_node_std& map) const;
   };
   Map_node_std const& nms = Map_node_std::get(Inserter());
   @endcode

   */
   template<class T> static Map_node_std const& get(T const& t) {
      static const Map_node_std map(( t ));
      return map;
   }

   /**Insert standard namespace;
   non-constant method, can be used only during construction */
   template<class NsTag> void insert_ns_tag(NsTag const&) {
      ns_.insert(NsTag::id(), NsTag::iri());
      ns_.set_prefix(NsTag::id(), NsTag::prefix());
   }

   /**Insert standard IRI node;
   non-constant method, can be used only during construction */
   template<class NTag> void insert_node_tag(NTag const&) {
      typedef typename NTag::ns_type ns_type;
      insert_ns_tag(ns_type());
      node_.insert_iri(NTag::id(), ns_type::id(), NTag::name());
   }

   Ns_id ns_id_next() const {return ns_id_next_;}
   Ns_id const* find_iri(std::string const& iri) const {return ns_.find_iri(iri);}
   Ns_id const* find_prefix(std::string const& pref) const {return ns_.find_prefix(pref);}
   std::string operator[](const Ns_id iid) const {return ns_[iid];}
   std::string at(const Ns_id iid) const {return ns_.at(iid);}
   std::string prefix(const Ns_id iid) const {return ns_.prefix(iid);}

   Node_id node_id_next() const {return node_id_next_;}
   Node const& operator[](const Node_id nid) const {return node_[nid];}
   Node const& at(const Node_id nid) const {return node_.at(nid);}
   Node_id const* find(Node const& node) const {return node_.find(node);}

private:
   map_ns ns_;
   Ns_id ns_id_next_;
   map_node node_;
   Node_id node_id_next_;
};

}//namespace owlcpp
#endif /* MAP_NODE_STD_HPP_ */
