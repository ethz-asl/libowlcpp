/** @file "/owlcpp/include/owlcpp/reasoner/detail/triple_to_fact_adaptor.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_TO_FACT_ADAPTOR_HPP_
#define TRIPLE_TO_FACT_ADAPTOR_HPP_
#include "boost/foreach.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/reasoner/exception.hpp"
#include "owlcpp/reasoner/detail/node_type.hpp"

class /*FACTPP_KERNEL_DECL*/ ReasoningKernel;
class TDLConceptExpression;
class TDLIndividualExpression;
class TDLObjectRoleExpression;
class TDLDataRoleExpression;
class TDLDataTypeExpression;
class TExpressionManager;
class TDLDataValue;

namespace owlcpp{ namespace detail{

/**@brief 
*******************************************************************************/
class Triple_to_fact_adaptor {
public:
   struct Err : public Reasoner_err {};

   Triple_to_fact_adaptor(Triple_store const& ts, ReasoningKernel& k, bool lax)
   : lax_(lax), ts_(ts), k_(k)
   {}

   void submit(Triple const& t);

   template<class Range> void submit(Range r) {
      BOOST_FOREACH(Triple const& t, r) submit(t);
   }



private:
   bool lax_;
   Triple_store const& ts_;
   ReasoningKernel& k_;

   TExpressionManager& e_manager();

   Node_type node_type(const Node_id nid) const;

   /**@param t triple x rdf:type y */
   void submit_type_triple(Triple const& t);

   /**@param t triple x y z */
   void submit_custom_triple(Triple const& t);

   TDLConceptExpression* concept(const Node_id nid);
   TDLConceptExpression* concept_expression(const Node_id nid);

   TDLIndividualExpression* instance(const Node_id nid);

   /** make instance of a class */
   TDLIndividualExpression* instance_of(const Node_id inst, const Node_id cls);

   TDLObjectRoleExpression* obj_role(const Node_id nid);
   TDLObjectRoleExpression* obj_role_expression(const Node_id nid);

   TDLDataRoleExpression* data_role(const Node_id nid);

   TDLDataTypeExpression* datatype(const Node_id nid);
   TDLDataTypeExpression* datatype_expression(const Node_id nid);

   TDLDataValue const* data_value(const Node_id nid);
};

}//namespace detail
}//namespace owlcpp
#endif /* TRIPLE_TO_FACT_ADAPTOR_HPP_ */
