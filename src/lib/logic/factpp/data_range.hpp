/** @file "/owlcpp/lib/logic/factpp/data_range.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef DATA_RANGE_HPP_
#define DATA_RANGE_HPP_
#include "boost/ptr_container/ptr_vector.hpp"
#include "expression.hpp"

namespace owlcpp{
class Node_literal;

namespace logic{ namespace factpp{

/**@brief FacetRestriction
*******************************************************************************/
class Facet_restriction : public Expression<Data_facet> {
public:
   Facet_restriction(
            const Node_id facet,
            Node_literal const& val,
            Triple_store const& ts
   );

   generated_t get(ReasoningKernel& k ) const;

private:
   const Node_id facet_;
   Expression<Data_inst>::ptr_t val_;
};

/**@brief
*******************************************************************************/
class Dr_standard : public Expression<Data_range> {
public:
   Dr_standard(Expression_args const& ea, Triple_store const& ts);

   generated_t get(ReasoningKernel& k ) const;

private:
   const Node_id nid_;
};

/**@brief DatatypeRestriction
*******************************************************************************/
class Dt_restriction : public Expression<Data_range> {
public:
   Dt_restriction(Expression_args const& ea, Triple_store const& ts);

   generated_t get(ReasoningKernel& k ) const;

private:
   Expression<Data_type>::ptr_t dt_;
   boost::ptr_vector<Expression<Data_facet> > facets_;
};

/**@brief DataIntersectionOf or DataUnionOf
*******************************************************************************/
class Dt_junction : public Expression<Data_range> {
public:
   Dt_junction(Expression_args const& ea, Triple_store const& ts);

   generated_t get(ReasoningKernel& k ) const;

private:
   const Node_id nid_;
   boost::ptr_vector<Expression<Data_range> > l_;
};

/**@brief DataOneOf
*******************************************************************************/
class Dt_oneof : public Expression<Data_range> {
public:
   Dt_oneof(Expression_args const& ea, Triple_store const& ts);

   generated_t get(ReasoningKernel& k ) const;

private:
   boost::ptr_vector<Expression<Data_inst> > l_;
};

/**@brief DataComplementOf
*******************************************************************************/
class Dt_complement : public Expression<Data_range> {
public:
   Dt_complement(Expression_args const& ea, Triple_store const& ts);

   generated_t get(ReasoningKernel& k ) const;

private:
   Expression<Data_range>::ptr_t de_;
};

}//namespace factpp
}//namespace logic
}//namespace owlcpp
#endif /* DATA_RANGE_HPP_ */
