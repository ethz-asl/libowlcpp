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

/**@brief Load ontology from input stream into triple store
@param stream STL input stream
@param store triple store
@param path optional path to ontology document, used for identification only
@param check
*******************************************************************************/
OWLCPP_IO_DECL
void load(
         std::istream& stream,
         Triple_store& store,
         std::string const& path = "",
         Check_id const& check = Check_id()
);

/**@brief Load ontology from input stream into triple store
@param stream STL input stream
@param store triple store
@param cat
@param path optional path to ontology document, used for identification only
@param check
*******************************************************************************/
OWLCPP_IO_DECL
void load(
         std::istream& stream,
         Triple_store& store,
         Catalog const& cat,
         std::string const& path = "",
         Check_id const& check = Check_id()
);

/**@brief
*******************************************************************************/
OWLCPP_IO_DECL
void load_file(
         boost::filesystem::path const& file,
         Triple_store& store,
         Check_id const& check = Check_id()
);

/**@brief
*******************************************************************************/
OWLCPP_IO_DECL
void load_file(
         boost::filesystem::path const& file,
         Triple_store& store,
         Catalog const& cat,
         Check_id const& check = Check_id()
);

/**@brief
*******************************************************************************/
OWLCPP_IO_DECL
void load_iri(std::string const& iri, Triple_store& store, Catalog const& cat);


}//namespace owlcpp
#endif /* INPUT_HPP_ */
