/** @file "/owlcpp/lib/rdf/literal_datatypes.cpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@date 2014 @author Mikhail K Levin
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/literal_datatypes.hpp"

#include "boost/utility/string_ref.hpp"


namespace owlcpp{ namespace detail{

namespace{

inline bool is_xsd_space(const char c) {
   switch(c) {
   case ' ':
   case '\t':
   case '\n':
   case '\r':
      return true;
   default:
      return false;
   }
}

inline boost::string_ref trim(boost::string_ref str) {
   for(; str.size() && is_xsd_space(str.front()); str.remove_prefix(1));
   for(; str.size() && is_xsd_space(str.back()); str.remove_suffix(1));
   return str;
}


}//anonymous namespace

/*
*******************************************************************************/
const std::string Datatype_bool::true_str = std::string("true");
const std::string Datatype_bool::false_str = std::string("false");

/*
*******************************************************************************/
Datatype_bool::value_type
Datatype_bool::convert(std::string const& str, const Node_id) {
   const boost::string_ref s = trim(str);
   if( s == true_str ) return true;
   if( s == false_str ) return false;
   return boost::lexical_cast<value_type>(s);
}

/*
*******************************************************************************/
Datatype_int::value_type
Datatype_int::convert(std::string const& str, const Node_id) {
   const boost::string_ref s = trim(str);
   return boost::lexical_cast<value_type>(s);
}

/*
*******************************************************************************/
Datatype_unsigned::value_type
Datatype_unsigned::convert(std::string const& str, const Node_id dt) {
   const boost::string_ref s = trim(str);
   return boost::numeric_cast<value_type>(
            boost::lexical_cast<Datatype_int::value_type>(s)
   );
}

/*
*******************************************************************************/
Datatype_real::value_type
Datatype_real::convert(std::string const& str, const Node_id dt) {
   const boost::string_ref s = trim(str);
   return boost::lexical_cast<value_type>(s);
}

}//namespace detail
}//namespace owlcpp
