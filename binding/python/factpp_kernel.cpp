/** @file "/owlcpp/binding/python/factpp_kernel.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/

#include "boost/python.hpp"
#include "boost/noncopyable.hpp"
namespace bp = boost::python;
#include "factpp/Kernel.hpp"

void export_factpp_kernel() {
   bp::class_<ReasoningKernel, boost::noncopyable>(
            "ReasoningKernel", bp::init<>()
   )
   .def(
            "getExpressionManager",
            &ReasoningKernel::getExpressionManager,
            bp::return_internal_reference<>(),
            "get access to an expression manager"
   )
   .def(
            "newKB",
            &ReasoningKernel::newKB,
            "create new KB"
   )
   .def(
            "releaseKB",
            &ReasoningKernel::releaseKB,
            "delete existed KB"
   )
   .def(
            "clearKB",
            &ReasoningKernel::clearKB,
            "reset current KB"
   )

   .def(
            "declare",
            &ReasoningKernel::declare,
            bp::return_internal_reference<>(),
            "axiom declare(x)"
   )

   .def(
            "impliesConcepts",
            &ReasoningKernel::impliesConcepts,
            bp::return_internal_reference<>(),
            (bp::arg("C"), bp::arg("D")),
            "axiom C [= D"
   )
   .def(
            "equalConcepts",
            &ReasoningKernel::equalConcepts,
            bp::return_internal_reference<>(),
            "axiom C1 = ... = Cn"
   )
   .def(
            "disjointConcepts",
            &ReasoningKernel::disjointConcepts,
            bp::return_internal_reference<>(),
            "axiom C1 != ... != Cn"
   )
   .def(
            "disjointUnion",
            &ReasoningKernel::disjointUnion,
            bp::return_internal_reference<>(),
            "axiom C = C1 or ... or Cn; C1 != ... != Cn"
   )


   .def(
            "setInverseRoles",
            &ReasoningKernel::setInverseRoles,
            bp::return_internal_reference<>(),
            (bp::arg("R"), bp::arg("S")),
            "R = Inverse(S)"
   )
   .def(
            "impliesORoles",
            &ReasoningKernel::impliesORoles,
            bp::return_internal_reference<>(),
            (bp::arg("R"), bp::arg("S")),
            "axiom (R [= S)"
   )
   .def(
            "impliesDRoles",
            &ReasoningKernel::impliesDRoles,
            bp::return_internal_reference<>(),
            (bp::arg("R"), bp::arg("S")),
            "axiom (R [= S)"
   )
   .def(
            "equalORoles",
            &ReasoningKernel::equalORoles,
            bp::return_internal_reference<>(),
            "axiom R1 = R2 = ..."
   )
   .def(
            "equalDRoles",
            &ReasoningKernel::equalDRoles,
            bp::return_internal_reference<>(),
            "axiom R1 = R2 = ..."
   )
   .def(
            "disjointORoles",
            &ReasoningKernel::disjointORoles,
            bp::return_internal_reference<>(),
            "axiom R1 != R2 != ..."
   )
   .def(
            "disjointDRoles",
            &ReasoningKernel::disjointDRoles,
            bp::return_internal_reference<>(),
            "axiom R1 != R2 != ..."
   )


   .def(
            "setODomain",
            &ReasoningKernel::setODomain,
            bp::return_internal_reference<>(),
            (bp::arg("R"), bp::arg("C")),
            "Domain (R C)"
   )
   .def(
            "setDDomain",
            &ReasoningKernel::setDDomain,
            bp::return_internal_reference<>(),
            (bp::arg("R"), bp::arg("C")),
            "Domain (R C)"
   )
   .def(
            "setORange",
            &ReasoningKernel::setORange,
            bp::return_internal_reference<>(),
            (bp::arg("R"), bp::arg("C")),
            "Range (R C)"
   )
   .def(
            "setDRange",
            &ReasoningKernel::setDRange,
            bp::return_internal_reference<>(),
            (bp::arg("R"), bp::arg("E")),
            "Range (R E)"
   )


   .def(
            "setTransitive",
            &ReasoningKernel::setTransitive,
            bp::return_internal_reference<>(),
            (bp::arg("R")),
            "Transitive (R)"
   )
   .def(
            "setReflexive",
            &ReasoningKernel::setReflexive,
            bp::return_internal_reference<>(),
            (bp::arg("R")),
            "Reflexive (R)"
   )
   .def(
            "setIrreflexive",
            &ReasoningKernel::setIrreflexive,
            bp::return_internal_reference<>(),
            (bp::arg("R")),
            "Irreflexive (R): Domain(R) = \\neg ER.Self"
   )
   .def(
            "setSymmetric",
            &ReasoningKernel::setSymmetric,
            bp::return_internal_reference<>(),
            (bp::arg("R")),
            "Symmetric (R): R [= R^-"
   )
   .def(
            "setAsymmetric",
            &ReasoningKernel::setAsymmetric,
            bp::return_internal_reference<>(),
            (bp::arg("R")),
            "Asymmetric (R): disjoint(R,R^-)"
   )
   .def(
            "setOFunctional",
            &ReasoningKernel::setOFunctional,
            bp::return_internal_reference<>(),
            (bp::arg("R")),
            "Functional (R)"
   )
   .def(
            "setDFunctional",
            &ReasoningKernel::setDFunctional,
            bp::return_internal_reference<>(),
            (bp::arg("R")),
            "Functional (R)"
   )
   .def(
            "setInverseFunctional",
            &ReasoningKernel::setInverseFunctional,
            bp::return_internal_reference<>(),
            (bp::arg("R")),
            "InverseFunctional (R)"
   )


   .def(
            "instanceOf",
            &ReasoningKernel::instanceOf,
            bp::return_internal_reference<>(),
            (bp::arg("I"), bp::arg("C")),
            "axiom I e C"
   )
   .def(
            "relatedTo",
            &ReasoningKernel::relatedTo,
            bp::return_internal_reference<>(),
            (bp::arg("I"), bp::arg("R"), bp::arg("J")),
            "axiom <I,J>:R"
   )
   .def(
            "relatedToNot",
            &ReasoningKernel::relatedToNot,
            bp::return_internal_reference<>(),
            (bp::arg("I"), bp::arg("R"), bp::arg("J")),
            "axiom <I,J>:\\neg R"
   )
   .def(
            "valueOf",
            &ReasoningKernel::valueOf,
            bp::return_internal_reference<>(),
            (bp::arg("I"), bp::arg("A"), bp::arg("V")),
            "axiom (value I A V)"
   )
   .def(
            "valueOfNot",
            &ReasoningKernel::valueOfNot,
            bp::return_internal_reference<>(),
            (bp::arg("I"), bp::arg("A"), bp::arg("V")),
            "axiom <I,V>:\\neg A"
   )
   .def(
            "processSame",
            &ReasoningKernel::processSame,
            bp::return_internal_reference<>(),
            "same individuals"
   )
   .def(
            "processDifferent",
            &ReasoningKernel::processDifferent,
            bp::return_internal_reference<>(),
            "different individuals"
   )
   .def(
            "setFairnessConstraint",
            &ReasoningKernel::setFairnessConstraint,
            bp::return_internal_reference<>(),
            "let all concept expressions in the ArgQueue to be fairness constraints"
   )


   .def(
            "retract",
            &ReasoningKernel::retract,
            bp::return_internal_reference<>(),
            (bp::arg("axiom")),
            "retract an axiom"
   )


   .def(
            "isKBConsistent",
            &ReasoningKernel::isKBConsistent,
            "return consistency status of KB"
   )
   .def(
            "preprocessKB",
            &ReasoningKernel::preprocessKB,
            "ensure that KB is preprocessed/consistence checked"
   )
   .def(
            "classifyKB",
            &ReasoningKernel::classifyKB,
            "ensure that KB is classified"
   )
   .def(
            "realiseKB",
            &ReasoningKernel::realiseKB,
            "ensure that KB is realised"
   )


   .def(
            "isFunctional",
            static_cast<
            bool (ReasoningKernel::*)(TDLObjectRoleExpression const*)
            >(&ReasoningKernel::isFunctional),
            (bp::arg("R")),
            "return true iff object role is functional"
   )
   .def(
            "isFunctional",
            static_cast<
            bool (ReasoningKernel::*)(TDLDataRoleExpression const*)
            >(&ReasoningKernel::isFunctional),
            (bp::arg("R")),
            "return true iff data role is functional"
   )
   .def(
            "isInverseFunctional",
            &ReasoningKernel::isInverseFunctional,
            (bp::arg("R")),
            "return true iff role is inverse-functional"
   )
   .def(
            "isTransitive",
            &ReasoningKernel::isTransitive,
            (bp::arg("R")),
            "return true iff role is transitive"
   )
   .def(
            "isSymmetric",
            &ReasoningKernel::isSymmetric,
            (bp::arg("R")),
            "return true iff role is symmetric"
   )
   .def(
            "isAsymmetric",
            &ReasoningKernel::isAsymmetric,
            (bp::arg("R")),
            "return true iff role is asymmetric"
   )
   .def(
            "isReflexive",
            &ReasoningKernel::isReflexive,
            (bp::arg("R")),
            "return true iff role is reflexive"
   )
   .def(
            "isIrreflexive",
            &ReasoningKernel::isIrreflexive,
            (bp::arg("R")),
            "return true iff role is irreflexive"
   )
   .def(
            "isSubRoles",
            static_cast<
            bool (ReasoningKernel::*)
            (TDLObjectRoleExpression const*, TDLObjectRoleExpression const*)
            >(&ReasoningKernel::isSubRoles),
            (bp::arg("R"), bp::arg("S")),
            "return true if R is a sub-role of S"
   )
   .def(
            "isSubRoles",
            static_cast<
            bool (ReasoningKernel::*)
            (TDLDataRoleExpression const*, TDLDataRoleExpression const*)
            >(&ReasoningKernel::isSubRoles),
            (bp::arg("R"), bp::arg("S")),
            "return true if R is a sub-role of S"
   )
   .def(
            "isDisjointRoles",
            static_cast<
            bool (ReasoningKernel::*)
            (TDLObjectRoleExpression const*, TDLObjectRoleExpression const*)
            >(&ReasoningKernel::isDisjointRoles),
            (bp::arg("R"), bp::arg("S")),
            "return true iff two roles are disjoint"
   )
   .def(
            "isDisjointRoles",
            static_cast<
            bool (ReasoningKernel::*)
            (TDLDataRoleExpression const*, TDLDataRoleExpression const*)
            >(&ReasoningKernel::isDisjointRoles),
            (bp::arg("R"), bp::arg("S")),
            "return true iff two roles are disjoint"
   )
   .def(
            "isDisjointRoles",
            static_cast<
            bool (ReasoningKernel::*)()
            >(&ReasoningKernel::isDisjointRoles),
            "return true iff all the roles in a arg-list are pairwise disjoint"
   )
   .def(
            "isSubChain",
            &ReasoningKernel::isSubChain,
            (bp::arg("R")),
            "return true if R is a super-role of a chain holding in the args"
   )
   .def(
            "isSatisfiable",
            &ReasoningKernel::isSatisfiable,
            (bp::arg("C")),
            "return true iff C is satisfiable"
   )
   .def(
            "isSubsumedBy",
            &ReasoningKernel::isSubsumedBy,
            (bp::arg("C"), bp::arg("D")),
            "return true iff C [= D holds"
   )
   .def(
            "isDisjoint",
            &ReasoningKernel::isDisjoint,
            (bp::arg("C"), bp::arg("D")),
            "return true iff C is disjoint with D; that is, (C and D) is unsatisfiable"
   )
   .def(
            "isEquivalent",
            &ReasoningKernel::isEquivalent,
            (bp::arg("C"), bp::arg("D")),
            "return true iff C is equivalent to D"
   )
   .def(
            "isSameIndividuals",
            &ReasoningKernel::isSameIndividuals,
            (bp::arg("I"), bp::arg("J")),
            "return true iff I and J refer to the same individual"
   )
   .def(
            "isInstance",
            &ReasoningKernel::isInstance,
            (bp::arg("I"), bp::arg("C")),
            "return true iff individual I is instance of given [complex] C"
   )
   ;
}
