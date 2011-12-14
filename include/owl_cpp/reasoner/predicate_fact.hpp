/** @file "/owl_cpp/include/owl_cpp/predicate_fact.hpp" 
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef PREDICATE_FACT_HPP_
#define PREDICATE_FACT_HPP_
#include <string>
#include <cstring>
#include "predicate_fact_abstract.hpp"
#include "factpp/Kernel.hpp"
#include "owl_cpp/rdf/query_nodes.hpp"

namespace owl_cpp{
class Triple_store;

/**@brief
*******************************************************************************/
struct Select_all : public CE_predicate {
   bool operator()(const ClassifiableEntry*) const {return true;}
};

/**@brief
*******************************************************************************/
template<bool Instance> struct Is_instance : public CE_predicate {
   bool operator()(const ClassifiableEntry* ce) const {
      const TConcept* c = reinterpret_cast<const TConcept*>(ce);
      return Instance == c->isSingleton();
   }
};

/**@brief Subclass/Superclass predicate
*******************************************************************************/
template<bool super> class Super_sub : public CE_predicate {
public:
   Super_sub(const TDLConceptExpression* ce, ReasoningKernel& kernel)
   : ce_(ce), kernel_(kernel) {}

   bool operator()(const ClassifiableEntry* ce) const {
      TExpressionManager& em = *kernel_.getExpressionManager();
      const TDLConceptExpression* c = em.Concept(ce->getName());
      return super ?
            kernel_.isSubsumedBy(ce_, c) :
            kernel_.isSubsumedBy(c, ce_);
//      return super ?
//            detail::is_subsumed_by(ce_, detail::get_ce(ce, kernel_), kernel_) :
//            detail::is_subsumed_by(detail::get_ce(ce, kernel_), ce_, kernel_);
   }

private:
   const TDLConceptExpression* ce_;
   ReasoningKernel& kernel_;
};

/**@brief Test for equivalent concepts
*******************************************************************************/
class Equivalent : public CE_predicate {
public:
   Equivalent(const TDLConceptExpression* ce, ReasoningKernel& kernel)
   : ce_(ce), kernel_(kernel) {}

   bool operator()(const ClassifiableEntry* ce) const {
      TExpressionManager& em = *kernel_.getExpressionManager();
      const TDLConceptExpression* c = em.Concept(ce->getName());
      return kernel_.isEquivalent(c, ce_);
   }

private:
   const TDLConceptExpression* ce_;
   ReasoningKernel& kernel_;
};


/**@brief Test whether node has a certain short name
*******************************************************************************/
template<bool IS> class Is_name : public CE_predicate {
public:
   Is_name(const std::string& name) : name_(name) {}

   bool operator() (const ClassifiableEntry* ce) const {
      return IS == (name_ == ce->getName());
   }
private:
   const std::string name_;
};

/**@brief Test whether node short name begins with a certain string
*******************************************************************************/
template<bool IS> class Begins_with : public CE_predicate {
public:
   Begins_with(const std::string& str) : str_(str) {}

   bool operator() (const ClassifiableEntry* ce) const {
      const std::string sname(ce->getName());
      const bool bw =
            ( sname.size() >= str_.size() ) &&
            ( sname.compare(0, str_.size(), str_) == 0 );
      return IS == bw;
   }
private:
   const std::string str_;
};

/**@brief Predicate tests whether class X can relate
through a given property P to another given class C.
*******************************************************************************/
class Can_relate : public C_predicate {
   Can_relate();
   Can_relate(const Can_relate&);
public:
   Can_relate(
         const TDLObjectRoleExpression* property,
         const TDLConceptExpression* c,
         ReasoningKernel& kernel
   );

   Can_relate(
         const std::string& property,
         const std::string& c,
         const Triple_store& store,
         ReasoningKernel& kernel
   );

   /**@return true if x.P(C) is satisfiable  */
   bool operator()(const TDLConceptExpression* x) const {
      return ! kernel_.isDisjoint(restriction_, x);
   }

   ~Can_relate() {}
private:
   ReasoningKernel& kernel_;
   const TDLConceptExpression* restriction_;
};

}//namespace owl_cpp

#endif /* PREDICATE_FACT_HPP_ */
