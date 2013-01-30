/** @file "/owlcpp/lib/logic/factpp/expression.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef EXPRESSION_HPP_
#define EXPRESSION_HPP_
#include <vector>
#include <memory>

#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/logic/exception.hpp"
#include "logic/expression_args.hpp"

class ReasoningKernel;
class TDLConceptExpression;
class TDLIndividualExpression;
class TDLObjectRoleExpression;
class TDLDataTypeExpression;
class TDLDataRoleExpression;

namespace owlcpp{ namespace logic{ namespace factpp{

struct Obj_type {
   typedef TDLConceptExpression* fact_type;
};

struct Obj_prop {
   typedef TDLObjectRoleExpression* fact_type;
};

struct Obj_inst {
   typedef TDLIndividualExpression* fact_type;
};

struct Data_type {
   typedef TDLDataTypeExpression* fact_type;
};

struct Data_prop {
   typedef TDLDataRoleExpression* fact_type;
};

/**@brief 
*******************************************************************************/
template<class T> struct Expression {
   struct Err : public Logic_err {};
   typedef Expression self_t;
   typedef T expression_type;
   typedef std::auto_ptr<self_t> ptr_t;
   typedef typename expression_type::fact_type generated_t;
   virtual generated_t get(ReasoningKernel& k) const  = 0;
   virtual std::string string() const {return "Expression";}
   virtual ~Expression() {}
};

template<class T> typename Expression<T>::ptr_t
make_expression(Expression_args const& ea, Triple_store const& ts);

template<> Expression<Obj_type>::ptr_t
make_expression<Obj_type>(Expression_args const& ea, Triple_store const& ts);

template<> Expression<Obj_prop>::ptr_t
make_expression<Obj_prop>(Expression_args const& ea, Triple_store const& ts);

template<> Expression<Data_type>::ptr_t
make_expression<Data_type>(Expression_args const& ea, Triple_store const& ts);

template<> Expression<Data_prop>::ptr_t
make_expression<Data_prop>(Expression_args const& ea, Triple_store const& ts);

template<class T> inline typename Expression<T>::ptr_t
make_expression(const Node_id h, Triple_store const& ts) {
   return make_expression<T>(Expression_args(h, ts), ts);
}

/**@brief generate owl:Restriction object type expression
*******************************************************************************/
Expression<Obj_type>::ptr_t
make_restriction_ote(Expression_args const& ea, Triple_store const& ts);

/**@brief generate owl:Class object type expression
*******************************************************************************/
Expression<Obj_type>::ptr_t
make_class_ote(Expression_args const& ea, Triple_store const& ts);

}//namespace factpp
}//namespace logic
}//namespace owlcpp
#endif /* EXPRESSION_HPP_ */
