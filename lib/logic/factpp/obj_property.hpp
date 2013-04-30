/** @file "/owlcpp/lib/logic/factpp/obj_property.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OBJ_PROPERTY_HPP_
#define OBJ_PROPERTY_HPP_
#include "expression.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{

/**@brief
*******************************************************************************/
struct Op_top : public Expression<Obj_prop> {
   generated_t get(ReasoningKernel& k ) const;
};

/**@brief
*******************************************************************************/
struct Op_bottom : public Expression<Obj_prop> {
   generated_t get(ReasoningKernel& k ) const;
};

/**@brief
*******************************************************************************/
class Op_declared : public Expression<Obj_prop> {
public:
   Op_declared(Expression_args const& ea, Triple_store const& ts);

   generated_t get(ReasoningKernel& k ) const;

private:
   std::string iri_;
};

/**@brief
*******************************************************************************/
class Op_inverse : public Expression<Obj_prop> {
public:
   Op_inverse(Expression_args const& ea, Triple_store const& ts);

   generated_t get(ReasoningKernel& k ) const;

private:
   ptr_t op_;
};

}//namespace factpp
}//namespace logic
}//namespace owlcpp
#endif /* OBJ_PROPERTY_HPP_ */
