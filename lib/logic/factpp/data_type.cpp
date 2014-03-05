/** @file "/owlcpp/lib/logic/factpp/data_type.cpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#include "data_type.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/print_node.hpp"
#include "owlcpp/detail/datatype_impl.hpp"
#include "factpp/Kernel.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{
using namespace owlcpp::terms;

/*
*******************************************************************************/
Dt_standard::Dt_standard(Expression_args const& ea, Triple_store const& ts)
   : nid_(ea.handle) {
   switch( internal_type_id(nid_) ) {
   case detail::Bool_tid:
   case detail::Int_tid:
   case detail::Unsigned_tid:
   case detail::Double_tid:
   case detail::Time_tid:
   case detail::String_tid:
      break;
   default: BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("unknown standard datatype")
            << Err::str1_t(ea.string(ts))
   );
   /* no break */
   }

}

/*
*******************************************************************************/
TDLDataTypeExpression* Dt_standard::get(ReasoningKernel& k ) const {
   TExpressionManager& em = *k.getExpressionManager();
   switch( internal_type_id(nid_) ) {
   case detail::Bool_tid: return em.getBoolDataType();
   case detail::Int_tid:
   case detail::Unsigned_tid: return em.getIntDataType();
   case detail::Double_tid: return em.getRealDataType();
   case detail::Time_tid: return em.getTimeDataType();
   case detail::String_tid:
   default: return em.getStrDataType();
   }
}

/*
*******************************************************************************/
Dt_declared::Dt_declared(Expression_args const& ea, Triple_store const& ts)
: iri_(to_string(ea.handle, ts)) {
   BOOST_ASSERT(is_iri(ts[ea.handle].ns_id()));
}

/*
*******************************************************************************/
TDLDataTypeName* Dt_declared::get(ReasoningKernel& k ) const {
   std::cout << '\n' << iri_ << std::endl;
   return k.getExpressionManager()->DataType(iri_);
}

}//namespace factpp
}//namespace logic
}//namespace owlcpp
