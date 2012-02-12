/** @file "/owlcpp/include/owlcpp/rdf/owl_terms.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWL_TERMS_HPP_
#define OWL_TERMS_HPP_

#include "owlcpp/rdf/config.hpp"
#include "owlcpp/terms/iri_tags.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
inline bool is_owl(const Ns_id ns) {
      switch ( ns() ) {
         case terms::N_owl::index:
         case terms::N_rdf::index:
         case terms::N_rdfs::index:
         case terms::N_xsd::index:
            return true;
         default:
            return false;
      }
}

class OWLCPP_RDF_DECL Node_map;

/**@brief
*******************************************************************************/
OWLCPP_RDF_DECL void insert_owl_terms(Node_map& node_map);

}//namespace owlcpp
#endif /* OWL_TERMS_HPP_ */
