/** @file "/owlcpp/lib/raptor_triple.hpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef RAPTOR_TRIPLE_HPP_
#define RAPTOR_TRIPLE_HPP_
#include <string>
#include "boost/tuple/tuple.hpp"
namespace b = boost;

namespace owlcpp{

enum Node_type {
   Resource,
   Literal,
   Anonymous,
   Unknown
};

/**
*******************************************************************************/
struct Term {
   Term(const Node_type t, const std::string& v) : type(t), value(v) {}
   Term(const Node_type t, unsigned char* v) : type(t), value((const char*)v) {}
   const Node_type type;
   const std::string value;

   template<class Stream> Stream& print(Stream& stream) const {
      switch (type) {
      case Resource:
         stream << "Resource: {";
         break;
      case Literal:
         stream << "Literal: {";
         break;
      case Anonymous:
         stream << "Anonymous: {";
         break;
      case Unknown:
         stream << "Unknown";
         break;
      default:
         break;
      }
      stream << value << "}";
      return stream;
   }
};

typedef b::tuple<Term,Term,Term> triple_t;


}//namespace owlcpp

#endif /* RAPTOR_TRIPLE_HPP_ */
