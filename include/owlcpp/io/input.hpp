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

namespace owlcpp{
class OWLCPP_IO_DECL Triple_store;
class OWLCPP_IO_DECL Catalog;

/**@brief Load ontology from input stream into triple store
@details

@param stream STL input stream
@param store triple store
@param path optional path to ontology document, used for identification only
*******************************************************************************/
OWLCPP_IO_DECL
void load(std::istream& stream, Triple_store& store, std::string const& path = "");

/**@brief
*******************************************************************************/
OWLCPP_IO_DECL
void load_file(boost::filesystem::path const& file, Triple_store& ts);

/**@brief
*******************************************************************************/
OWLCPP_IO_DECL
void load_file(boost::filesystem::path& file, Triple_store& ts, Catalog const& cat);

/**@brief
*******************************************************************************/
OWLCPP_IO_DECL
void load_iri(std::string const& iri, Triple_store& ts, Catalog const& cat);


}//namespace owlcpp
#endif /* INPUT_HPP_ */
