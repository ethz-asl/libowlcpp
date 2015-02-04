/** @file "/owlcpp/include/owlcpp/io/input.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef INPUT_HPP_
#define INPUT_HPP_
#include <iosfwd>
#include <string>

#include "boost/filesystem/path.hpp"

#include "owlcpp/io/config.hpp"
#include "owlcpp/io/exception.hpp"
#include "owlcpp/io/check_ontology_id.hpp"

namespace owlcpp{
class OWLCPP_IO_DECL Triple_store;
class OWLCPP_IO_DECL Catalog;

/**@brief Load ontology from input stream ignoring imports
@param stream STL input stream
@param store triple store
@param path optional path to ontology document, used for identification only
@param check reference to a polymorphic class that checks that the input ontology
has the expected ontologyIRI and versionIRI.
@throw Input_err if input ontology contains an error or an ontology with the same
ID is already in the triple store.
If an exception is thrown, the destination triple store remains unchanged.
*******************************************************************************/
OWLCPP_IO_DECL
void load(
         std::istream& stream,
         Triple_store& store,
         std::string const& path = "",
         Check_id const& check = Check_id()
);

/**@brief Load ontology from input stream and its imports from the file system
@param stream STL input stream
@param store triple store
@param cat catalog of ontology documents used for locating imports
@param path optional path to ontology document, used for identification only
@param check reference to a polymorphic class that checks that the input ontology
has the expected ontologyIRI and versionIRI.
@throw Input_err if input ontology contains an error or an ontology with the same
ID is already in the triple store.
If an exception is thrown, the destination triple store remains unchanged.
*******************************************************************************/
OWLCPP_IO_DECL
void load(
         std::istream& stream,
         Triple_store& store,
         Catalog const& cat,
         std::string const& path = "",
         Check_id const& check = Check_id()
);

/**@brief Load ontology from file ignoring imports
@param file filesystem path to ontology document
@param store triple store
@param check reference to a polymorphic class that checks that the input ontology
has the expected ontologyIRI and versionIRI.
@throw Input_err if input ontology contains an error or an ontology with the same
ID has already been loaded into the triple store.
If an exception is thrown, the destination triple store remains unchanged.
*******************************************************************************/
OWLCPP_IO_DECL
void load_file(
         boost::filesystem::path const& file,
         Triple_store& store,
         Check_id const& check = Check_id()
);

/**@brief Load ontology from file ignoring imports
@param file filesystem path to ontology document
@param store triple store
@param cat catalog of ontology documents used for locating imports
@param check reference to a polymorphic class that checks that the input ontology
has the expected ontologyIRI and versionIRI.
@throw Input_err if input ontology contains an error or an ontology with the same
ID has already been loaded into the triple store.
If an exception is thrown, the destination triple store remains unchanged.
*******************************************************************************/
OWLCPP_IO_DECL
void load_file(
         boost::filesystem::path const& file,
         Triple_store& store,
         Catalog const& cat,
         Check_id const& check = Check_id()
);

/**Example of loading ontologies into triple store
@example print_triples.cpp
*/

/**@brief Load ontology identified by versionIRI or ontologyIRI and its imports
@param iri ontology versionIRI or ontologyIRI
@param store triple store
@param cat catalog of ontology documents used for locating imports
@throw Input_err if input ontology contains an error or an ontology with the same
ID has already been loaded into the triple store.
If an exception is thrown, the destination triple store remains unchanged.
*******************************************************************************/
OWLCPP_IO_DECL
void load_iri(std::string const& iri, Triple_store& store, Catalog const& cat);


}//namespace owlcpp
#endif /* INPUT_HPP_ */
