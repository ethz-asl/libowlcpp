/** @file "/owlcpp/binding/python/factpp_expression.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/

#include "boost/python.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "boost/noncopyable.hpp"
namespace bp = boost::python;
#include "factpp/tExpressionManager.hpp"

void export_factpp_expressions() {

   bp::class_<
      TDLExpression, boost::noncopyable
   >("TDLExpression", bp::no_init);

   bp::class_<
      TDLConceptExpression, bp::bases<TDLExpression>, boost::noncopyable
   >("TDLConceptExpression", bp::no_init);

   bp::class_<
      TDLRoleExpression, bp::bases<TDLExpression>, boost::noncopyable
   >("TDLRoleExpression", bp::no_init);

   bp::class_<
      TDLObjectRoleComplexExpression,
      bp::bases<TDLRoleExpression>,
      boost::noncopyable
   >("TDLObjectRoleComplexExpression", bp::no_init);

   bp::class_<
      TDLObjectRoleExpression,
      bp::bases<TDLObjectRoleComplexExpression>,
      boost::noncopyable
   >("TDLObjectRoleExpression", bp::no_init);

   bp::class_<
      TDLDataRoleExpression, bp::bases<TDLRoleExpression>, boost::noncopyable
   >("TDLDataRoleExpression", bp::no_init);

   bp::class_<
      TDLIndividualExpression, bp::bases<TDLExpression>, boost::noncopyable
   >("TDLIndividualExpression", bp::no_init);

   bp::class_<
      TDLIndividualName,
      bp::bases<TDLIndividualExpression>,
      bp::bases<TNamedEntity>,
      boost::noncopyable
   >("TDLIndividualExpression", bp::no_init);

   bp::class_<
      TDLDataExpression, bp::bases<TDLExpression>, boost::noncopyable
   >("TDLDataExpression", bp::no_init);

   bp::class_<
      TDLDataTypeExpression, bp::bases<TDLDataExpression>, boost::noncopyable
   >("TDLDataTypeExpression", bp::no_init);

   bp::class_<
      TDLDataValue, bp::bases<TDLDataExpression>, boost::noncopyable
   >("TDLDataValue", bp::no_init);

   bp::class_<
      TDLFacetExpression, bp::bases<TDLDataExpression>, boost::noncopyable
   >("TDLFacetExpression", bp::no_init);

   bp::class_<
      TDLDataTypeName, bp::bases<TDLDataTypeExpression>, boost::noncopyable
   >("TDLDataTypeName", bp::no_init);


   bp::class_<std::vector<TDLExpression const*> >("expression_vector")
   .def(bp::vector_indexing_suite<std::vector<TDLExpression const*> >());
}
