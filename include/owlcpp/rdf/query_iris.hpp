/** @file "/owlcpp/include/owlcpp/rdf/query_iris.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef QUERY_IRIS_HPP_
#define QUERY_IRIS_HPP_

#include "owlcpp/terms/iri_tags_owl.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
inline bool is_std_owl(const Ns_id ns) {
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

}//namespace owlcpp
#endif /* QUERY_IRIS_HPP_ */
