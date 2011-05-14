/** @file "/owl_cpp/lib/raptor_triple.hpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#ifndef RAPTOR_TRIPLE_HPP_
#define RAPTOR_TRIPLE_HPP_
#include <string>
#include "boost/tuple/tuple.hpp"
namespace b = boost;

namespace owl_cpp{

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


}//namespace owl_cpp

#endif /* RAPTOR_TRIPLE_HPP_ */
