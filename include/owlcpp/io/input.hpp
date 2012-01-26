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

/**@brief 
*******************************************************************************/
OWLCPP_IO_DECL
std::istream& operator>>(std::istream& is, Triple_store& ts);

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
