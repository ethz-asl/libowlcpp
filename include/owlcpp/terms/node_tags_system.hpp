/** @file "/owlcpp/include/owlcpp/terms/node_tags_system.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_TAGS_SYSTEM_HPP_
#define NODE_TAGS_SYSTEM_HPP_

/**@brief Standard system nodes
@details ((standard namespace IRI prefix)(node name))
*******************************************************************************/
#define OWLCPP_NODE_LIST_SYSTEM \
      ((empty)()) \
/* */

#include "owlcpp/terms/node_iri_tag_gen_macro.hpp"
#include "owlcpp/terms/ns_iri_tags.hpp"

namespace owlcpp{ namespace terms{

/**@brief
@code
struct empty_ {
   typedef empty ns_type;
   typedef ::owlcpp::Node_id id_type;
   static const unsigned index = 0;
   static std::string const & name() {
      static const std::string str = std::string("");
      return str;
   }
   static id_type id() {return id_type(index);}
}; @endcode

*******************************************************************************/
//OWLCPP_GENERATE_NODE_TAGS(OWLCPP_NODE_LIST_SYSTEM, 0)
OWLCPP_NODE_TAG_TYPE(empty_, empty, "", 0); //avoid msvc pp warnings

}//namespace terms

/**
*******************************************************************************/
inline bool is_empty(const Node_id id) {return id == terms::empty_::id();}

}//namespace owlcpp
#endif /* NODE_TAGS_SYSTEM_HPP_ */
