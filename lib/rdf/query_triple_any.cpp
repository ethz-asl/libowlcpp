/** @file "/owlcpp/lib/rdf/query_triple_any.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/query_triple_any.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/exception.hpp"

namespace owlcpp {

/*
*******************************************************************************/
triple_any_range_t find_triple(
         Triple_store const& ts,
         Node_id const* subj,
         Node_id const* pred,
         Node_id const* obj,
         Doc_id const* doc
) {
   const unsigned S = 1 << 0;
   const unsigned P = 1 << 1;
   const unsigned O = 1 << 2;
   const unsigned D = 1 << 3;
   unsigned n =
            (subj ? S : 0 ) |
            (pred ? P : 0 ) |
            ( obj ? O : 0 ) |
            ( doc ? D : 0 )
            ;
   switch (n) {
      case 0: return ts.map_triple();
      case S: return ts.find_triple(*subj, any(), any(), any());
      case P: return ts.find_triple(any(), *pred, any(), any());
      case O: return ts.find_triple(any(), any(),  *obj, any());
      case D: return ts.find_triple(any(), any(), any(),  *doc);
      default:
         BOOST_THROW_EXCEPTION(
                  Rdf_err() << Rdf_err::msg_t("unsupported query")
         );
   }
}


}//namespace owlcpp
