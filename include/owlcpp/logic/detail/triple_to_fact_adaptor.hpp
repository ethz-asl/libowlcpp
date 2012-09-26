/** @file "/owlcpp/include/owlcpp/logic/detail/triple_to_fact_adaptor.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_TO_FACT_ADAPTOR_HPP_
#define TRIPLE_TO_FACT_ADAPTOR_HPP_
#include "boost/foreach.hpp"
#include "boost/range.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/query_node.hpp"
#include "owlcpp/logic/exception.hpp"
#include "owlcpp/logic/config.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

class ReasoningKernel;
class TExpressionManager;
class TDLConceptExpression;
class TDLIndividualExpression;
class TDLObjectRoleExpression;
class TDLDataRoleExpression;
class TDLDataTypeExpression;
class TDLDataValue;
class TDLAxiom;

namespace owlcpp{ namespace logic{ namespace factpp{

/**@brief 
*******************************************************************************/
class OWLCPP_LOGIC_DECL Adaptor_triple {
public:
   struct Err : public Logic_err {};

   Adaptor_triple(Triple_store const& ts, ReasoningKernel& k, bool lax)
   : lax_(lax), ts_(ts), k_(k)
   {}

   void submit(Triple const& t) {axiom(t);}

   template<class Range> void submit(Range r) {
      BOOST_FOREACH(Triple const& t, r) {
         try{
            submit(t);
         } catch(...) {
            BOOST_THROW_EXCEPTION(
                     Err()
                     << Err::msg_t("error submitting triple")
                     << Err::str1_t(
                              to_string_short(t.subject(), ts_) + ' ' +
                              to_string_short(t.predicate(), ts_) + ' ' +
                              to_string_short(t.object(), ts_)
                     )
                     << Err::nested_t(boost::current_exception())
            );
         }
      }
   }

   TDLAxiom* axiom(Triple const& t);

private:
   bool lax_;
   Triple_store const& ts_;
   ReasoningKernel& k_;

   /**@param t triple x y z */
   void submit_custom_triple(Triple const& t);

   TDLConceptExpression* obj_type(const Node_id nid);

   TDLIndividualExpression* obj_value(const Node_id nid);

   /** make instance of a class */
   TDLIndividualExpression* instance_of(const Node_id inst, const Node_id cls);

   TDLObjectRoleExpression* obj_property(const Node_id nid);

   TDLDataRoleExpression* data_property(const Node_id nid);

   TDLDataTypeExpression* data_type(const Node_id nid);

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
            const Node_id subj = owlcpp::terms::T_empty_::id()
   );

   TExpressionManager& e_m();
};

}//namespace factpp
}//namespace logic
}//namespace owlcpp
#endif /* TRIPLE_TO_FACT_ADAPTOR_HPP_ */
