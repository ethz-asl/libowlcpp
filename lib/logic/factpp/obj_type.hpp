/** @file "/owlcpp/lib/logic/factpp/obj_type.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OBJ_TYPE_HPP_
#define OBJ_TYPE_HPP_
#include <vector>
#include <string>
#include "boost/ptr_container/ptr_vector.hpp"
#include "factpp/Kernel.hpp"
#include "expression.hpp"
#include "owlcpp/rdf/query_node.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{

/**@brief
*******************************************************************************/
struct Ot_thing : public Expression<Obj_type> {
   generated_t get(ReasoningKernel& k ) const {
      return k.getExpressionManager()->Top();
   }
};

/**@brief
*******************************************************************************/
struct Ot_nothing : public Expression<Obj_type> {
   generated_t get(ReasoningKernel& k ) const {
      return k.getExpressionManager()->Bottom();
   }
};

/**@brief
*******************************************************************************/
class Ot_declared : public Expression<Obj_type> {
public:
   Ot_declared(Expression_args const& ea, Triple_store const& ts)
   : iri_(to_string_short(ea.handle, ts))
   {}

   generated_t get(ReasoningKernel& k ) const {
      return k.getExpressionManager()->Concept(iri_);
   }

private:
   const std::string iri_;
};

/**@brief
*******************************************************************************/
class Ot_complement : public Expression<Obj_type> {
public:
   Ot_complement(Expression_args const& ea, Triple_store const& ts);
   Ot_complement(const Node_id nid, Triple_store const& ts)
   : ot_(make_expression<Obj_type>(nid, ts))
   {}

   TDLConceptExpression* get(ReasoningKernel& k ) const {
      return k.getExpressionManager()->Not(ot_->get(k));
   }

private:
   ptr_t ot_;
};

/**@brief
*******************************************************************************/
class Ot_restriction : public Expression<Obj_type> {
public:
   Ot_restriction(Expression_args const& ea, Triple_store const& ts);

   Ot_restriction(
            const Node_id r_type,
            const Node_id prop,
            const Node_id val,
            Triple_store const& ts
   )
   : restr_type_(r_type)
   {
      init(prop, val, ts);
   }

   TDLConceptExpression* get(ReasoningKernel& k ) const ;

private:
   const Node_id restr_type_;
   Expression<Obj_prop>::ptr_t op_;
   Expression<Obj_type>::ptr_t ot_;
   std::string inst_;

   void init(
            const Node_id prop,
            const Node_id val,
            Triple_store const& ts
   );
};

/**@brief
*******************************************************************************/
class Ot_card_restriction : public Expression<Obj_type> {
public:
   Ot_card_restriction(Expression_args const& ea, Triple_store const& ts);

   TDLConceptExpression* get(ReasoningKernel& k ) const ;

private:
   Expression<Obj_prop>::ptr_t op_;
   Expression<Obj_type>::ptr_t ot_;
   const Node_id card_type_;
   const unsigned n_;
};

/**@brief Type from a list of types (owl:intersectionOf, owl:unionOf)
*******************************************************************************/
class Ot_type_list : public Expression<Obj_type> {
public:
   Ot_type_list(Expression_args const& ea, Triple_store const& ts);

   Ot_type_list(const Node_id type, const Node_id seq, Triple_store const& ts)
   : type_(type)
   {
      make_sequence(seq, ts);
   }

   TDLConceptExpression* get(ReasoningKernel& k ) const;

private:
   const Node_id type_;
   boost::ptr_vector<Expression<Obj_type> > otl_;  /**< object type expressions list */

   void make_sequence(const Node_id seq, Triple_store const& ts);
};

/**@brief generate owl:oneOf type expression
*******************************************************************************/
class Ot_instance_list : public Expression<Obj_type> {
public:
   Ot_instance_list(const Node_id seq, Triple_store const& ts) {
      make_sequence(seq, ts);
   }

   Ot_instance_list(Expression_args const& ea, Triple_store const& ts);

   TDLConceptExpression* get(ReasoningKernel& k ) const;

private:
   std::vector<std::string> il_; /**< instance IRI list */

   void make_sequence(const Node_id seq, Triple_store const& ts);
};

}//namespace factpp
}//namespace logic
}//namespace owlcpp
#endif /* OBJ_TYPE_HPP_ */
