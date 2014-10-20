/** @file "/owlcpp/include/owlcpp/rdf/literal_datatypes.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012

Implementation details for literal node values:
internal representations, default datatypes, conversions
*******************************************************************************/
#ifndef LITERAL_DATATYPES_HPP_
#define LITERAL_DATATYPES_HPP_
#include <string>
#include "boost/cstdint.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/numeric/conversion/cast.hpp"

#include "owlcpp/rdf/config.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/terms/term_methods.hpp"

namespace owlcpp{ namespace detail{

/*
*******************************************************************************/
struct OWLCPP_RDF_DECL Datatype_bool {
   typedef bool value_type;
   typedef owlcpp::terms::xsd_boolean default_datatype;
   static const std::string true_str;
   static const std::string false_str;

   static value_type convert(std::string const& str, const Node_id);

   template<class T> static value_type convert(const T x, const Node_id) {
      return boost::numeric_cast<value_type>(x);
   }

   static std::string const& name_str(const Node_id) {
      static const std::string name(to_string_pref(default_datatype()));
      return name;
   }

   static std::string to_string(value_type const v, const Node_id) {
      return v ? true_str : false_str;
   }
};

/*
*******************************************************************************/
struct OWLCPP_RDF_DECL Datatype_int{
   typedef boost::intmax_t value_type;
   typedef owlcpp::terms::xsd_int default_datatype;

   static value_type convert(std::string const& str, const Node_id);

   template<class T> static value_type convert(const T x, const Node_id) {
      return boost::numeric_cast<value_type>(x);
   }

   static std::string const& name_str(const Node_id) {
      static const std::string name(to_string_pref(default_datatype()));
      return name;
   }

   static std::string to_string(value_type const v, const Node_id) {
      return boost::lexical_cast<std::string>(v);
   }
};

/*
*******************************************************************************/
struct OWLCPP_RDF_DECL Datatype_unsigned{
   typedef boost::uintmax_t value_type;
   typedef owlcpp::terms::xsd_unsignedInt default_datatype;

   static value_type convert(std::string const& str, const Node_id dt);

   template<class T> static value_type convert(const T x, const Node_id) {
      return boost::numeric_cast<value_type>(x);
   }

   static std::string const& name_str(const Node_id) {
      static const std::string name(to_string_pref(default_datatype()));
      return name;
   }

   static std::string to_string(value_type const v, const Node_id) {
      return boost::lexical_cast<std::string>(v);
   }
};

/*
*******************************************************************************/
struct OWLCPP_RDF_DECL Datatype_real{
   typedef double value_type;
   typedef owlcpp::terms::xsd_double default_datatype;

   static value_type convert(std::string const& str, const Node_id dt);

   template<class T> static value_type convert(const T x, const Node_id) {
      return boost::numeric_cast<value_type>(x);
   }

   static std::string const& name_str(const Node_id) {
      static const std::string name(to_string_pref(default_datatype()));
      return name;
   }

   static std::string to_string(value_type const v, const Node_id) {
      return boost::lexical_cast<std::string>(v);
   }
};

/*
*******************************************************************************/
struct Datatype_string{
   typedef std::string value_type;
   typedef owlcpp::terms::xsd_string default_datatype;

   static value_type const& convert(std::string const& str, const Node_id dt) {
      return str;
   }

   template<class T> static value_type convert(const T x) {return x;}

   static std::string const& name_str(const Node_id) {
      static const std::string name(to_string_pref(default_datatype()));
      return name;
   }
};

}//namespace detail
}//namespace owlcpp
#endif /* LITERAL_DATATYPES_HPP_ */
