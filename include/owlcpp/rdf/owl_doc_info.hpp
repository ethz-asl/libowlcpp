/** @file "/owlcpp/include/owlcpp/rdf/owl_doc_info.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWL_DOC_INFO_HPP_
#define OWL_DOC_INFO_HPP_
#include <string>
#include "owlcpp/node_id.hpp"

namespace owlcpp{

/**@brief IDs and location of an OWL document
*******************************************************************************/
class Doc_info {
public:
   Doc_info(std::string const& path, const Node_id iri, const Node_id version)
   : path_(path), iri_(iri), version_(version)
   {}
   Node_id iri() const {return iri_;}
   Node_id version() const {return version_;}
   std::string path() const {return path_;}

   bool operator<(Doc_info const& odi) const {
      if( iri() < odi.iri() ) return true;
      if( iri() > odi.iri() ) return false;

      if( version() < odi.version() ) return true;
      if( version() > odi.version() ) return false;

      return path() < odi.path();
   }

   bool operator==(Doc_info const& odi) const {
      return iri() == odi.iri() && version() == odi.version() && path() == odi.path();
   }

private:
   std::string path_;
   Node_id iri_;
   Node_id version_;
};

}//namespace owlcpp
#endif /* OWL_DOC_INFO_HPP_ */
