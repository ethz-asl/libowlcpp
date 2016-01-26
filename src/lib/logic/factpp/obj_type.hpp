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
#include "expression.hpp"

namespace owlcpp{ namespace logic{ namespace factpp{

/**@brief
*******************************************************************************/
struct Ot_thing : public Expression<Obj_type> {
   generated_t get(ReasoningKernel& k ) const;
};

/**@brief
*******************************************************************************/
struct Ot_nothing : public Expression<Obj_type> {
   generated_t get(ReasoningKernel& k ) const;
};

/**@brief
*******************************************************************************/
class Ot_declared : public Expression<Obj_type> {
public:
   Ot_declared(Expression_args const& ea, Triple_store const& ts);
   generated_t get(ReasoningKernel& k ) const;

private:
   const std::string iri_;
};

/**@brief
*******************************************************************************/
class Ot_complement : public Expression<Obj_type> {
public:
   Ot_complement(Expression_args const& ea, Triple_store const& ts);
   Ot_complement(const Node_id nid, Triple_store const& ts);
   generated_t get(ReasoningKernel& k ) const;

private:
   ptr_t ot_;
};

/**@brief Object type expression generator for object property restrictions
*******************************************************************************/
class Ot_op_restriction : public Expression<Obj_type> {
public:
   Ot_op_restriction(Expression_args const& ea, Triple_store const& ts)
   :  restr_type_(ea.pred2)
   {
      init(ea.obj1, ea.obj2, ts);
   }

   generated_t get(ReasoningKernel& k ) const ;

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

/**@brief Object type expression generator for data property restrictions
Data(All/Some)ValuesFrom
*******************************************************************************/
class Ot_data_has_value : public Expression<Obj_type> {
public:
   Ot_data_has_value(Expression_args const& ea, Triple_store const& ts);

   generated_t get(ReasoningKernel& k ) const ;

private:
   Expression<Data_prop>::ptr_t dp_;
   Expression<Data_type>::ptr_t dt_;
   std::string val_str_;
};

/**@brief Object type expression generator for data property restrictions
Data(All/Some)ValuesFrom
*******************************************************************************/
class Ot_dp_restriction : public Expression<Obj_type> {
public:
   Ot_dp_restriction(Expression_args const& ea, Triple_store const& ts)
   :  restr_type_(ea.pred2)
   {
      init(ea.obj1, ea.obj2, ts);
   }

   generated_t get(ReasoningKernel& k ) const ;

private:
   const Node_id restr_type_;
   Expression<Data_prop>::ptr_t dp_;
   Expression<Data_range>::ptr_t dr_;
   std::string val_str_;

   void init(
            const Node_id prop,
            const Node_id val,
            Triple_store const& ts
   );


};

/**@brief  Object type expression generator for object property cardinality
restrictions
*******************************************************************************/
class Ot_opc_restriction : public Expression<Obj_type> {
public:
   Ot_opc_restriction(Expression_args const& ea, Triple_store const& ts);

   generated_t get(ReasoningKernel& k ) const ;

private:
   Expression<Obj_prop>::ptr_t op_;
   Expression<Obj_type>::ptr_t ot_;
   const Node_id card_type_;
   const unsigned n_;
};

/**@brief  Object type expression generator for object property cardinality
restrictions
*******************************************************************************/
class Ot_dpc_restriction : public Expression<Obj_type> {
public:
   Ot_dpc_restriction(Expression_args const& ea, Triple_store const& ts);

   generated_t get(ReasoningKernel& k ) const ;

private:
   Expression<Data_prop>::ptr_t dp_;
   Expression<Data_type>::ptr_t dt_;
   const Node_id card_type_;
   const unsigned n_;
};

/**@brief Generate object type from a list of types (owl:intersectionOf, owl:unionOf)
*******************************************************************************/
class Ot_type_list : public Expression<Obj_type> {
public:
   Ot_type_list(Expression_args const& ea, Triple_store const& ts)
   : type_(ea.pred1)
   {
      init(ea.obj1, ts);
   }

   Ot_type_list(const Node_id type, const Node_id seq, Triple_store const& ts)
   : type_(type)
   {
      init(seq, ts);
   }

   generated_t get(ReasoningKernel& k ) const;

private:
   const Node_id type_;
   boost::ptr_vector<Expression<Obj_type> > otl_;  /**< object type expressions list */

   void init(const Node_id seq, Triple_store const& ts);
};

/**@brief generate owl:oneOf type expression
*******************************************************************************/
class Ot_instance_list : public Expression<Obj_type> {
public:
   Ot_instance_list(const Node_id seq, Triple_store const& ts) {
      make_sequence(seq, ts);
   }

   Ot_instance_list(Expression_args const& ea, Triple_store const& ts) {
      make_sequence(ea.obj1, ts);
   }

   generated_t get(ReasoningKernel& k ) const;

private:
   std::vector<std::string> il_; /**< instance IRI list */

   void make_sequence(const Node_id seq, Triple_store const& ts);
};

}//namespace factpp
}//namespace logic
}//namespace owlcpp
#endif /* OBJ_TYPE_HPP_ */
