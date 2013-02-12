/** @file "/owlcpp/binding/python/factpp_expression_manager.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/

#include "boost/python.hpp"
#include "boost/noncopyable.hpp"
namespace bp = boost::python;
#include "factpp/tExpressionManager.hpp"

void export_factpp_expression_manager() {
   bp::class_<TExpressionManager, boost::noncopyable>(
            "TExpressionManager", bp::init<>()
   )
   .def(
            "clear",
            &TExpressionManager::clear,
            "clear the ontology"
   )
   .def(
            "isUniversalRole",
            static_cast<
            bool (TExpressionManager::*)(TDLObjectRoleExpression const*) const
               >(&TExpressionManager::isUniversalRole),
            (bp::arg("R")),
            "return true iff R is a top object role"
   )
   .def(
            "isUniversalRole",
            static_cast<
            bool (TExpressionManager::*)(TDLDataRoleExpression const*) const
               >(&TExpressionManager::isUniversalRole),
            (bp::arg("R")),
            "return true iff R is a top data role"
   )
   .def(
            "isEmptyRole",
            static_cast<
            bool (TExpressionManager::*)(TDLObjectRoleExpression const*) const
               >(&TExpressionManager::isEmptyRole),
            (bp::arg("R")),
            "return true iff R is a bottom object role"
   )
   .def(
            "isEmptyRole",
            static_cast<
            bool (TExpressionManager::*)(TDLDataRoleExpression const*) const
               >(&TExpressionManager::isEmptyRole),
            (bp::arg("R")),
            "return true iff R is a bottom data role"
   )
   .def(
            "nConcepts",
            &TExpressionManager::nConcepts,
            "get number of registered concepts"
   )
   .def(
            "nIndividuals",
            &TExpressionManager::nIndividuals,
            "get number of registered individuals"
   )
   .def(
            "nORoles",
            &TExpressionManager::nORoles,
            "get number of registered object roles"
   )
   .def(
            "nDRoles",
            &TExpressionManager::nDRoles,
            "get number of registered data roles"
   )
   .def(
            "newArgList",
            &TExpressionManager::newArgList,
            "opens new argument list"
   )
   .def(
            "addArg",
            &TExpressionManager::addArg,
            (bp::arg("arg")),
            "add argument ARG to the current argument list"
   )
   .def(
            "getArgList",
            &TExpressionManager::getArgList,
            bp::return_internal_reference<>(),
            "get the latest argument list"
   )
   .def(
            "Top",
            &TExpressionManager::Top,
            bp::return_internal_reference<>(),
            "get TOP concept"
   )
   .def(
            "Bottom",
            &TExpressionManager::Bottom,
            bp::return_internal_reference<>(),
            "get BOTTOM concept"
   )
   .def(
            "Concept",
            &TExpressionManager::Concept,
            bp::return_internal_reference<>(),
            (bp::arg("name")),
            "get named concept"
   )
   .def(
            "Not",
            &TExpressionManager::Not,
            bp::return_internal_reference<>(),
            (bp::arg("C")),
            "get negation of a concept C"
   )
   .def(
            "And",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)()
               >(&TExpressionManager::And),
            bp::return_internal_reference<>(),
            "get an n-ary conjunction expression; take the arguments from the "
            "last argument list"
   )
   .def(
            "And",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)
            (TDLConceptExpression const*, TDLConceptExpression const*)
               >(&TExpressionManager::And),
            bp::return_internal_reference<>(),
            (bp::arg("C"), bp::arg("D")),
            "return C and D"
   )
   .def(
            "Or",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)()
               >(&TExpressionManager::Or),
            bp::return_internal_reference<>(),
            "get an n-ary disjunction expression; take the arguments from the "
            "last argument list"
   )
   .def(
            "Or",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)
            (TDLConceptExpression const*, TDLConceptExpression const*)
               >(&TExpressionManager::Or),
            bp::return_internal_reference<>(),
            (bp::arg("C"), bp::arg("D")),
            "return C or D"
   )
   .def(
            "OneOf",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)()
               >(&TExpressionManager::OneOf),
            bp::return_internal_reference<>(),
            "get an n-ary one-of expression; take the arguments from the "
            "last argument list"
   )
   .def(
            "OneOf",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)
            (TDLIndividualExpression const*)
               >(&TExpressionManager::OneOf),
            bp::return_internal_reference<>(),
            (bp::arg("I")),
            "return concept {I} for the individual I"
   )
   .def(
            "SelfReference",
            &TExpressionManager::SelfReference,
            bp::return_internal_reference<>(),
            (bp::arg("R")),
            "get self-reference restriction of an object role R"
   )
   .def(
            "Value",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)
            (TDLObjectRoleExpression const*, TDLIndividualExpression const*)
               >(&TExpressionManager::Value),
            bp::return_internal_reference<>(),
            (bp::arg("R"), bp::arg("I")),
            "get value restriction wrt an object role R and an individual I"
   )
   .def(
            "Exists",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)
            (TDLObjectRoleExpression const*, TDLConceptExpression const*)
               >(&TExpressionManager::Exists),
            bp::return_internal_reference<>(),
            (bp::arg("R"), bp::arg("C")),
            "get existential restriction wrt an object role R and a concept C"
   )
   .def(
            "Forall",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)
            (TDLObjectRoleExpression const*, TDLConceptExpression const*)
               >(&TExpressionManager::Forall),
            bp::return_internal_reference<>(),
            (bp::arg("R"), bp::arg("C")),
            "get universal restriction wrt an object role R and a concept C"
   )
   .def(
            "MinCardinality",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)
            (unsigned int, TDLObjectRoleExpression const*, TDLConceptExpression const*)
               >(&TExpressionManager::MinCardinality),
            bp::return_internal_reference<>(),
            (bp::arg("n"), bp::arg("R"), bp::arg("C")),
            "get min cardinality restriction wrt number N, an object role R "
            "and a concept C"
   )
   .def(
            "MaxCardinality",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)
            (unsigned int, TDLObjectRoleExpression const*, TDLConceptExpression const*)
               >(&TExpressionManager::MaxCardinality),
            bp::return_internal_reference<>(),
            (bp::arg("n"), bp::arg("R"), bp::arg("C")),
            "get max cardinality restriction wrt number N, an object role R "
            "and a concept C"
   )
   .def(
            "Cardinality",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)
            (unsigned int, TDLObjectRoleExpression const*, TDLConceptExpression const*)
               >(&TExpressionManager::Cardinality),
            bp::return_internal_reference<>(),
            (bp::arg("n"), bp::arg("R"), bp::arg("C")),
            "get exact cardinality restriction wrt number N, an object role R "
            "and a concept C"
   )
   .def(
            "Value",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)
            (TDLDataRoleExpression const*, TDLDataValue const*)
               >(&TExpressionManager::Value),
            bp::return_internal_reference<>(),
            (bp::arg("R"), bp::arg("V")),
            "get value restriction wrt a data role R and a data value V"
   )
   .def(
            "Exists",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)
            (TDLDataRoleExpression const*, TDLDataExpression const*)
               >(&TExpressionManager::Exists),
            bp::return_internal_reference<>(),
            (bp::arg("R"), bp::arg("E")),
            "get existential restriction wrt a data role R and a data expression E"
   )
   .def(
            "Forall",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)
            (TDLDataRoleExpression const*, TDLDataExpression const*)
               >(&TExpressionManager::Forall),
            bp::return_internal_reference<>(),
            (bp::arg("R"), bp::arg("E")),
            "get universal restriction wrt a data role R and a data expression E"
   )
   .def(
            "MinCardinality",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)
            (unsigned int, TDLDataRoleExpression const*, TDLDataExpression const*)
               >(&TExpressionManager::MinCardinality),
            bp::return_internal_reference<>(),
            (bp::arg("n"), bp::arg("R"), bp::arg("E")),
            "get min cardinality restriction wrt number N, a data role R and a "
            "data expression E"
   )
   .def(
            "MaxCardinality",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)
            (unsigned int, TDLDataRoleExpression const*, TDLDataExpression const*)
               >(&TExpressionManager::MaxCardinality),
            bp::return_internal_reference<>(),
            (bp::arg("n"), bp::arg("R"), bp::arg("E")),
            "get max cardinality restriction wrt number N, a data role R and a "
            "data expression E"
   )
   .def(
            "Cardinality",
            static_cast<
            TDLConceptExpression* (TExpressionManager::*)
            (unsigned int, TDLDataRoleExpression const*, TDLDataExpression const*)
               >(&TExpressionManager::Cardinality),
            bp::return_internal_reference<>(),
            (bp::arg("n"), bp::arg("R"), bp::arg("E")),
            "get exact cardinality restriction wrt number N, a data role R and "
            "a data expression E"
   )
   .def(
            "Individual",
            &TExpressionManager::Individual,
            bp::return_internal_reference<>(),
            (bp::arg("name")),
            "get named individual"
   )
   .def(
            "ObjectRoleTop",
            &TExpressionManager::ObjectRoleTop,
            bp::return_internal_reference<>(),
            "get TOP object role"
   )
   .def(
            "ObjectRoleBottom",
            &TExpressionManager::ObjectRoleBottom,
            bp::return_internal_reference<>(),
            "get BOTTOM object role"
   )
   .def(
            "ObjectRole",
            &TExpressionManager::ObjectRole,
            bp::return_internal_reference<>(),
            (bp::arg("name")),
            "get named object role"
   )
   .def(
            "Inverse",
            &TExpressionManager::Inverse,
            bp::return_internal_reference<>(),
            (bp::arg("R")),
            "get an inverse of a given object role expression R"
   )
   .def(
            "Compose",
            &TExpressionManager::Compose,
            bp::return_internal_reference<>(),
            "get a role chain corresponding to R1 o ... o Rn; take the "
            "arguments from the last argument list"
   )
   .def(
            "ProjectFrom",
            &TExpressionManager::ProjectFrom,
            bp::return_internal_reference<>(),
            (bp::arg("R"), bp::arg("C")),
            "get a expression corresponding to R projected from C"
   )
   .def(
            "ProjectInto",
            &TExpressionManager::ProjectInto,
            bp::return_internal_reference<>(),
            (bp::arg("R"), bp::arg("C")),
            "get a expression corresponding to R projected into C"
   )
   .def(
            "DataRoleTop",
            &TExpressionManager::DataRoleTop,
            bp::return_internal_reference<>(),
            "get TOP data role"
   )
   .def(
            "DataRoleBottom",
            &TExpressionManager::DataRoleBottom,
            bp::return_internal_reference<>(),
            "get BOTTOM data role"
   )
   .def(
            "DataRole",
            &TExpressionManager::DataRole,
            bp::return_internal_reference<>(),
            (bp::arg("name")),
            "get named data role"
   )
   .def(
            "DataTop",
            &TExpressionManager::DataTop,
            bp::return_internal_reference<>(),
            "get TOP data element"
   )
   .def(
            "DataBottom",
            &TExpressionManager::DataBottom,
            bp::return_internal_reference<>(),
            "get BOTTOM data element"
   )
   .def(
            "DataType",
            &TExpressionManager::DataType,
            bp::return_internal_reference<>(),
            (bp::arg("name")),
            "get named data type"
   )
   .def(
            "getStrDataType",
            &TExpressionManager::getStrDataType,
            bp::return_internal_reference<>(),
            "get basic string data type"
   )
   .def(
            "getIntDataType",
            &TExpressionManager::getIntDataType,
            bp::return_internal_reference<>(),
            "get basic integer data type"
   )
   .def(
            "getRealDataType",
            &TExpressionManager::getRealDataType,
            bp::return_internal_reference<>(),
            "get basic floating point data type"
   )
   .def(
            "getBoolDataType",
            &TExpressionManager::getBoolDataType,
            bp::return_internal_reference<>(),
            "get basic boolean data type"
   )
   .def(
            "getTimeDataType",
            &TExpressionManager::getTimeDataType,
            bp::return_internal_reference<>(),
            "get basic date-time data type"
   )
   .def(
            "RestrictedType",
            &TExpressionManager::RestrictedType,
            bp::return_internal_reference<>(),
            (bp::arg("type"), bp::arg("facet")),
            "TODO:"
   )
   .def(
            "DataValue",
            &TExpressionManager::DataValue,
            bp::return_internal_reference<>(),
            (bp::arg("value"), bp::arg("type")),
            "get data value with given VALUE and TYPE"
   )
   .def(
            "DataNot",
            &TExpressionManager::DataNot,
            bp::return_internal_reference<>(),
            (bp::arg("E")),
            "get negation of a data expression E"
   )
   .def(
            "DataAnd",
            &TExpressionManager::DataAnd,
            bp::return_internal_reference<>(),
            "get an n-ary data conjunction expression; take the arguments from "
            "the last argument list"
   )
   .def(
            "DataOr",
            &TExpressionManager::DataOr,
            bp::return_internal_reference<>(),
            "get an n-ary data disjunction expression; take the arguments from "
            "the last argument list"
   )
   .def(
            "DataOneOf",
            &TExpressionManager::DataOneOf,
            bp::return_internal_reference<>(),
            "get an n-ary data one-of expression; take the arguments from the "
            "last argument list"
   )
   .def(
            "FacetMinInclusive",
            &TExpressionManager::FacetMinInclusive,
            bp::return_internal_reference<>(),
            (bp::arg("V")),
            "get minInclusive facet with a given VALUE"
   )
   .def(
            "FacetMinExclusive",
            &TExpressionManager::FacetMinExclusive,
            bp::return_internal_reference<>(),
            (bp::arg("V")),
            "get minExclusive facet with a given VALUE"
   )
   .def(
            "FacetMaxInclusive",
            &TExpressionManager::FacetMaxInclusive,
            bp::return_internal_reference<>(),
            (bp::arg("V")),
            "get maxInclusive facet with a given VALUE"
   )
   .def(
            "FacetMaxExclusive",
            &TExpressionManager::FacetMaxExclusive,
            bp::return_internal_reference<>(),
            (bp::arg("V")),
            "get maxExclusive facet with a given VALUE"
   )
/*
   .def(
            "XXX",
            &TExpressionManager::XXX,
            bp::return_internal_reference<>(),
            (bp::arg("XXX"), bp::arg("XXX")),
            "TODO"
   )
   .def(
            "XXX",
            &TExpressionManager::XXX,
            bp::return_internal_reference<>(),
            "TODO"
   )
   .def(
            "XXX",
            &TExpressionManager::XXX,
            bp::return_internal_reference<>(),
            (bp::arg("XXX")),
            "TODO"
   )
   .def(
            "XXX",
            static_cast<
            XXX (TExpressionManager::*)(XXX const*)
               >(&TExpressionManager::XXX),
            bp::return_internal_reference<>(),
            "TODO"
   )
*/
   ;
}
