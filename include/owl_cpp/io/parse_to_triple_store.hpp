/** @file "/owl_cpp/include/owl_cpp/parse_to_triple_store.hpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef PARSE_TO_TRIPLE_STORE_HPP_
#define PARSE_TO_TRIPLE_STORE_HPP_
#include <iosfwd>
#include <string>
#include "boost/tuple/tuple.hpp"
#include "owl_cpp/config.hpp"
#include "owl_cpp/exception.hpp"
#include "owl_cpp/io/catalog.hpp"

namespace owl_cpp{
class OWLCPP_DECL Triple_store;

struct OWLCPP_DECL Parse_err : public base_exception {};

/** @brief Populate triple store from a stream
@details parsing is done with
Raptor RDF Syntax Library <http://librdf.org/raptor/>
*******************************************************************************/
OWLCPP_DECL void load(
      std::istream& stream,
      Triple_store& store
);

/** @brief Populate triple store from a file
@details parsing is done with
Raptor RDF Syntax Library <http://librdf.org/raptor/>
*******************************************************************************/
OWLCPP_DECL void load(
      const std::string& file, /**< path to OWL file */
      Triple_store& store /**< store parsed ontologies */
);

/**
@param file path to ontology file
@return ontology IRI, ontology version
*******************************************************************************/
OWLCPP_DECL boost::tuple<std::string,std::string> ontology_id(const std::string& file);


/** @brief map OWL ontologies present at the path to their IRIs
@param cat map from ontology IRI to OWL file path
@param path if file, read as ontology and determine IRI;
if directory, read all *.owl files
*******************************************************************************/
OWLCPP_DECL void find_ontologies(
      Catalog& cat,
      const std::string& path
);

namespace detail{
/** If not already loaded, increment n, load ontology with IRI iri,
recurse for each imported ontology
*******************************************************************************/
OWLCPP_DECL void load_iri(
      const std::string& iri, /**< ontology IRI */
      Triple_store& store, /**< store parsed ontologies */
      const Catalog& cat /**< path for each ontology IRI */
);
}//namespace detail

/**
*******************************************************************************/
OWLCPP_DECL inline void load(
      const std::string& file, /**< path to OWL file */
      Triple_store& store, /**< store parsed ontologies */
      const Catalog& cat /**< path for each ontology IRI */
) {
   const boost::tuple<std::string,std::string> ts = ontology_id(file);
   if( ts.get<1>().empty() ) detail::load_iri(ts.get<0>(), store, cat);
   else detail::load_iri(ts.get<1>(), store, cat);
}

}//namespace owl_cpp

#endif /* PARSE_TO_TRIPLE_STORE_HPP_ */
