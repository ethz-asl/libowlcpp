/** @file "/owlcpp/include/owlcpp/rdf/doc_meta.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef DOC_META_HPP_
#define DOC_META_HPP_
#include <string>
#include "owlcpp/node_id.hpp"

namespace owlcpp{

/**@brief Document metadata
*******************************************************************************/
struct Doc_meta {
   Doc_meta(
            const Node_id ontology_iri_,
            const Node_id version_iri_,
            std::string const& path_
   )
   : ontology_iri(ontology_iri_), version_iri(version_iri_), path(path_)
   {}

   Node_id ontology_iri;
   Node_id version_iri;
   std::string path;
};

}//namespace owlcpp
#endif /* DOC_META_HPP_ */
