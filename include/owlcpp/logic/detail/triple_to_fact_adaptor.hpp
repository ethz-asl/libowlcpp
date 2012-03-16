/** @file "/owlcpp/include/owlcpp/logic/detail/triple_to_fact_adaptor.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_TO_FACT_ADAPTOR_HPP_
#define TRIPLE_TO_FACT_ADAPTOR_HPP_
#include "boost/foreach.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/logic/exception.hpp"
#include "owlcpp/logic/detail/node_type.hpp"
#include "owlcpp/logic/detail/node_property.hpp"
#include "owlcpp/logic/config.hpp"
#include "factpp/config.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

class /*FACTPP_KERNEL_DECL*/ ReasoningKernel;
class TDLConceptExpression;
class TDLIndividualExpression;
class TDLObjectRoleExpression;
class TDLDataRoleExpression;
class TDLDataTypeExpression;
class TExpressionManager;
class TDLDataValue;
class TDLAxiom;

namespace owlcpp{ namespace detail{

/**@brief 
*******************************************************************************/
class OWLCPP_REASONER_DECL Triple_to_fact_adaptor {
public:
   struct Err : public Logic_err {};

   Triple_to_fact_adaptor(Triple_store const& ts, ReasoningKernel& k, bool lax)
   : lax_(lax), ts_(ts), k_(k)
   {}

   void submit(Triple const& t) {axiom(t);}

   template<class Range> void submit(Range r) {
      BOOST_FOREACH(Triple const& t, r) submit(t);
   }

   TDLAxiom* axiom(Triple const& t);

private:
   bool lax_;
   Triple_store const& ts_;
   ReasoningKernel& k_;

   TDLAxiom* axiom_rdf_type(Triple const& t);
   TDLAxiom* axiom_custom_predicate(Triple const& t);
   TDLAxiom* all_disjoint(Triple const& t);
   TDLAxiom* disjoint_union(Triple const& t);
   TDLAxiom* sub_property_of(Triple const& t);
   TDLAxiom* equivalent_property(Triple const& t);
   TDLAxiom* equivalent_class(Triple const& t);
   TDLAxiom* property_chain(Triple const& t);
   TDLAxiom* property_disjoint_with(Triple const& t);
   TDLAxiom* domain(Triple const& t);
   TDLAxiom* range(Triple const& t);
   TDLAxiom* all_different(Triple const& t);

   TExpressionManager& e_m();

   template<class Decl> Decl declaration(const Node_id nid) const {
      using namespace owlcpp::terms;
      Decl d;
      BOOST_FOREACH(
               Triple const& t,
               ts_.triples().find(nid, T_rdf_type::id(), any(), any())) {
         d.set(t.object());
      }

      BOOST_FOREACH(
               Triple const& t,
               ts_.triples().find(any(), T_owl_annotatedSource::id(), nid, any())) {

         const Node_id x = t.subject();
         if( ts_[x].ns_id() != N_blank::id() ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("non-blank owl:annotatedSource x")
                  << Err::str1_t(ts_.string(nid))
         );
         BOOST_FOREACH(
                  Triple const& t,
                  ts_.triples().find(x, T_owl_annotatedTarget::id(), any(), any())) {
            d.set(t.object());
         }
      }
      return d;
   }

   /**@param t triple x y z */
   void submit_custom_triple(Triple const& t);

   TDLConceptExpression* concept(const Node_id nid);

   TDLIndividualExpression* instance(const Node_id nid);

   /** make instance of a class */
   TDLIndividualExpression* instance_of(const Node_id inst, const Node_id cls);

   TDLObjectRoleExpression* obj_property(const Node_id nid);

   TDLDataRoleExpression* data_property(const Node_id nid);

   TDLDataTypeExpression* datatype(const Node_id nid);

   TDLDataValue const* data_value(const Node_id nid);

   TDLAxiom* negative_property_assertion(const Node_id nid);
};

}//namespace detail
}//namespace owlcpp
#endif /* TRIPLE_TO_FACT_ADAPTOR_HPP_ */
