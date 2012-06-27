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
class Doc_meta {
public:
   Doc_meta(
            const Node_id ontology_iri,
            const Node_id version_iri,
            std::string const& path
   )
   : oiri_(ontology_iri), viri_(version_iri), path_(path)
   {}

   Node_id ontology_iri() const {return oiri_;}
   Node_id version_iri() const {return viri_;}
   std::string const& path() const {return path_;}

private:
   Node_id oiri_;
   Node_id viri_;
   std::string path_;
};

}//namespace owlcpp
#endif /* DOC_META_HPP_ */
