/** @file "/owlcpp/include/owlcpp/logic/detail/triple_to_fact_adaptor.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_TO_FACT_ADAPTOR_HPP_
#define TRIPLE_TO_FACT_ADAPTOR_HPP_
#include "owlcpp/logic/exception.hpp"
#include "owlcpp/logic/config.hpp"

class ReasoningKernel;
class TExpressionManager;
class TDLConceptExpression;
class TDLIndividualExpression;
class TDLObjectRoleExpression;
class TDLDataRoleExpression;
class TDLDataTypeExpression;
class TDLDataExpression;
class TDLDataValue;
class TDLAxiom;

namespace owlcpp{
class Triple_store;
struct Triple;

namespace logic{ namespace factpp{

/**@brief 
*******************************************************************************/
class OWLCPP_LOGIC_DECL Adaptor_triple {
public:
   struct Err : public Logic_err {};

   Adaptor_triple(Triple_store const& ts, ReasoningKernel& k, bool strict)
   : strict_(strict), ts_(ts), k_(k)
   {}

   TDLAxiom* submit(Triple const&);

private:
   bool strict_;
   Triple_store const& ts_;
   ReasoningKernel& k_;

   TDLAxiom* axiom(Triple const& t);

   TDLConceptExpression* obj_type(const Node_id nid);

   TDLIndividualExpression* obj_value(const Node_id nid);

   /** make instance of a class */
   TDLIndividualExpression* instance_of(const Node_id inst, const Node_id cls);

   TDLObjectRoleExpression* obj_property(const Node_id nid);

   TDLDataRoleExpression* data_property(const Node_id nid);

   TDLDataTypeExpression* data_type(const Node_id nid);

   TDLDataExpression* data_range(const Node_id nid);

   TDLDataValue const* data_value(const Node_id nid);

   TDLAxiom* negative_property_assertion(const Node_id nid);

   TDLAxiom* axiom_type(Triple const& t);

   TDLAxiom* axiom_blank_node_type(Triple const& t);

   TDLAxiom* axiom_iri_node_type(Triple const& t);

   /** x *:y z, where *:y is object or data property
    @param t RDF triple
    @return axiom pointer
   */
   TDLAxiom* axiom_custom_predicate(Triple const& t);

   TDLAxiom* axiom_from_seq(
            const Node_id op,
            const Node_id seq_nid,
            const std::size_t min_len,
            const Node_id subj
   );

   TExpressionManager& e_m();
};

}//namespace factpp
}//namespace logic
}//namespace owlcpp
#endif /* TRIPLE_TO_FACT_ADAPTOR_HPP_ */
