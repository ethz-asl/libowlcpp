/** @file "/owlcpp/include/owlcpp/rdf/detail/node_map_owl.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_MAP_OWL_HPP_
#define NODE_MAP_OWL_HPP_
#include <string>
#include "boost/assert.hpp"
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/member.hpp"

#include "owlcpp/rdf/config.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/ns_id.hpp"
#include "owlcpp/node.hpp"
#include "owlcpp/node_id.hpp"

namespace owlcpp{ namespace detail{

/**@brief Store RDF nodes
*******************************************************************************/
class OWLCPP_RDF_DECL Node_map_owl {
   typedef std::pair<Ns_id, std::string> iri_entry_t;
   typedef boost::multi_index_container<
         iri_entry_t,
         boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
               boost::multi_index::tag<struct id_tag>,
               boost::multi_index::member<
                  iri_entry_t, Ns_id, &iri_entry_t::first
               >
            >,
            boost::multi_index::hashed_unique<
               boost::multi_index::tag<struct string_tag>,
               boost::multi_index::member<
                  iri_entry_t, std::string, &iri_entry_t::second
               >
            >
         >
      > store_t;
   typedef store_t::index<id_tag>::type iid_index_t;
   typedef iid_index_t::iterator iid_iter_t;
   typedef store_t::index<string_tag>::type string_index_t;
   typedef string_index_t::iterator string_iter_t;

   typedef std::pair<Node_id, Node> node_entry_t;
   typedef boost::multi_index_container<
         node_entry_t,
         boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
               boost::multi_index::tag<struct id_tag>,
               boost::multi_index::member<
                  node_entry_t, Node_id, &node_entry_t::first
               >
            >,
            boost::multi_index::hashed_unique<
               boost::multi_index::tag<struct node_tag>,
               boost::multi_index::member<
                  node_entry_t, Node, &node_entry_t::second
               >
            >
         >
      > node_store_t;
   typedef node_store_t::index<id_tag>::type nid_index_t;
   typedef nid_index_t::iterator nid_iter_t;
   typedef node_store_t::index<node_tag>::type node_index_t;
   typedef node_index_t::iterator node_iter_t;

public:
   Node_map_owl();

private:
   node_store_t node_store_;
};

}//namespace detail
}//namespace owlcpp
#endif /* NODE_MAP_OWL_HPP_ */
