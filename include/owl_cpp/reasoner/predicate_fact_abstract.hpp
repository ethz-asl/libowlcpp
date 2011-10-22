/** @file "/owl_cpp/include/owl_cpp/predicate_fact_abstract.hpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef PREDICATE_FACT_ABSTRACT_HPP_
#define PREDICATE_FACT_ABSTRACT_HPP_
class ClassifiableEntry;
class TDLConceptExpression;

namespace owl_cpp{

/**@brief Abstract predicate for selecting FaCT nodes
*******************************************************************************/
struct CE_predicate {
   virtual bool operator()(const ClassifiableEntry*) const = 0;
   typedef CE_predicate type;
   virtual ~CE_predicate() {}
};

/** Abstract predicate for checking FaCT++ concept expressions
*******************************************************************************/
struct C_predicate {
   virtual bool operator()(const TDLConceptExpression*) const = 0;
   virtual ~C_predicate() {}
};

}//namespace owl_cpp
#endif /* PREDICATE_FACT_ABSTRACT_HPP_ */
