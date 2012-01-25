/** @file "/owlcpp/include/owlcpp/io/parse_to_triple_store.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef PARSE_TO_TRIPLE_STORE_HPP_
#define PARSE_TO_TRIPLE_STORE_HPP_
#include <iosfwd>
#include <string>
#include "boost/tuple/tuple.hpp"
#include "owlcpp/io/config.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/io/catalog.hpp"

namespace owlcpp{
class OWLCPP_IO_DECL Triple_store;

struct OWLCPP_IO_DECL Parse_err : public base_exception {};

/** @brief Populate triple store from a stream
@details parsing is done with
Raptor RDF Syntax Library <http://librdf.org/raptor/>
*******************************************************************************/
OWLCPP_IO_DECL void load(
      std::istream& stream,
      Triple_store& store
);

/** @brief Populate triple store from a file
@details parsing is done with
Raptor RDF Syntax Library <http://librdf.org/raptor/>
*******************************************************************************/
OWLCPP_IO_DECL void load(
      const std::string& file, /**< path to OWL file */
      Triple_store& store /**< store parsed ontologies */
);

/**
@param file path to ontology file
@return ontology IRI, ontology version
*******************************************************************************/
OWLCPP_IO_DECL boost::tuple<std::string,std::string> ontology_id(const std::string& file);


/** @brief map OWL ontologies present at the path to their IRIs
@param cat map from ontology IRI to OWL file path
@param path if file, read as ontology and determine IRI;
if directory, read all *.owl files
*******************************************************************************/
OWLCPP_IO_DECL void find_ontologies(
      Catalog& cat,
      const std::string& path
);

namespace detail{
/** If not already loaded, increment n, load ontology with IRI iri,
recurse for each imported ontology
*******************************************************************************/
OWLCPP_IO_DECL void load_iri(
      const std::string& iri, /**< ontology IRI */
      Triple_store& store, /**< store parsed ontologies */
      const Catalog& cat /**< path for each ontology IRI */
);
}//namespace detail

/**
*******************************************************************************/
OWLCPP_IO_DECL inline void load(
      const std::string& file, /**< path to OWL file */
      Triple_store& store, /**< store parsed ontologies */
      const Catalog& cat /**< path for each ontology IRI */
) {
   const boost::tuple<std::string,std::string> ts = ontology_id(file);
   if( ts.get<1>().empty() ) detail::load_iri(ts.get<0>(), store, cat);
   else detail::load_iri(ts.get<1>(), store, cat);
}

}//namespace owlcpp

#endif /* PARSE_TO_TRIPLE_STORE_HPP_ */
