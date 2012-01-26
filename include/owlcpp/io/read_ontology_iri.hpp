/** @file "/owlcpp/include/owlcpp/io/read_ontology_iri.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef READ_ONTOLOGY_IRI_HPP_
#define READ_ONTOLOGY_IRI_HPP_
#include <string>
#include <utility>

#include "boost/filesystem/path.hpp"

#include "owlcpp/io/config.hpp"

namespace owlcpp{

/**@param file local path to ontology document
@return ontologyIRI and versionIRI strings
*******************************************************************************/
OWLCPP_IO_DECL std::pair<std::string,std::string>
read_ontology_iri(boost::filesystem::path const& file);

}//namespace owlcpp
#endif /* READ_ONTOLOGY_IRI_HPP_ */
