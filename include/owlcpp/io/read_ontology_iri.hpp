/** @file "/owlcpp/include/owlcpp/io/read_ontology_iri.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef READ_ONTOLOGY_IRI_HPP_
#define READ_ONTOLOGY_IRI_HPP_
#include <limits>
#include <string>
#include <utility>

#include "boost/filesystem/path.hpp"

#include "owlcpp/io/config.hpp"

namespace owlcpp{

/**@brief Find ontologyIRI and versionIRI declarations in a stream containing
ontology document
@param is input stream containing ontology document
@param search_depth once ontologyIRI declaration is found, stop searching for
versionIRI declaration after @b search_depth triples
@return ontologyIRI and versionIRI strings
*******************************************************************************/
OWLCPP_IO_DECL std::pair<std::string,std::string>
read_ontology_iri(
         std::istream& is,
         const std::size_t search_depth = std::numeric_limits<std::size_t>::max()
);

/**@brief find ontologyIRI and versionIRI declarations in ontology document
@param file filesystem path to ontology document
@param search_depth once ontologyIRI declaration is found, stop searching for
versionIRI declaration after @b search_depth triples
@return ontologyIRI and versionIRI strings
*******************************************************************************/
OWLCPP_IO_DECL std::pair<std::string,std::string>
read_ontology_iri(
         boost::filesystem::path const& file,
         const std::size_t search_depth = std::numeric_limits<std::size_t>::max()
);

}//namespace owlcpp
#endif /* READ_ONTOLOGY_IRI_HPP_ */
