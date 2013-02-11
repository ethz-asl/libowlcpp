/** @file "/owlcpp/binding/python/factpp_expression.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/

#include "boost/python.hpp"
#include "boost/noncopyable.hpp"
namespace bp = boost::python;
#include "factpp/tExpressionManager.hpp"

struct TDLExpression_wrap: TDLExpression, bp::wrapper<TDLExpression> {
   void accept(DLExpressionVisitor& visitor) const {
      this->get_override("accept")();
   }
};

struct TDLConceptExpression_wrap:
         public TDLConceptExpression, public bp::wrapper<TDLConceptExpression> {
   void accept(DLExpressionVisitor& visitor) const {
      this->get_override("accept")();
   }
};

void export_factpp_expressions() {
   bp::class_<TDLExpression_wrap, boost::noncopyable>(
            "TDLExpression", bp::no_init
   );
   bp::class_<TDLConceptExpression_wrap, bp::bases<TDLExpression_wrap>, boost::noncopyable>(
            "TDLConceptExpression", bp::no_init
   );

/*
   bp::class_<TDLExpression>(
            "TDLExpression", bp::init<>()
   )
      .def(
               "clear",
               &TExpressionManager::clear,
               "clear the ontology"
      )
   ;
*/
}
