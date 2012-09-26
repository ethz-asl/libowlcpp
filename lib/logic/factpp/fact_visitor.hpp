/** @file "/owlcpp/lib/logic/factpp/fact_visitor.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010-1
*******************************************************************************/
#ifndef FACT_VISITOR_HPP_
#define FACT_VISITOR_HPP_

#include "../type_vector.hpp"
#include "boost/mpl/inherit.hpp"
#include "boost/mpl/inherit_linearly.hpp"
namespace bmp = boost::mpl;
namespace bmpp = boost::mpl::placeholders;

#include "loki/Visitor.h"
#include "factpp/Kernel.hpp"

#include "owlcpp/rdf/triple.hpp"
#include "owlcpp/node_base.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/print.hpp"
namespace ot = owlcpp::terms;
#include "owlcpp/logic/query_fact.hpp"

namespace owlcpp{

class Triple_store;

namespace detail{

/** Type with single pure virtual Visit method
*******************************************************************************/
template<class T> struct One_visit : public Loki::Visitor<Triple_std<T>, void, true> {};

/** Combine types for all visitable tags
*******************************************************************************/
typedef bmp::inherit_linearly<
      ot::mpl_vector_terms_visitable_t,
      bmp::inherit<One_visit<bmpp::_2>, bmpp::_1>
>::type visitor_t;

}//namespace detail


/** Pass RDF triples to FaCT++ kernel.
Visitor of triples.
*******************************************************************************/
class Triple_to_fact : public Loki::BaseVisitor, public detail::visitor_t {
public:
   struct Err : public base_exception {};
   struct Warn : public base_exception {};

   Triple_to_fact(const Triple_store& store, ReasoningKernel& kernel)
   : store_(store), kernel_(kernel ) {}

   /** Annotation or property assertion
   @param tr {instance, property, instance}
   */
   void Visit(const Triple_std<ot::T_empty_>& tr);

   /** @param tr {_:x, rdf:type, owl:AllDifferent},
    {_:x owl:members Seq} or {_:x owl:distinctMembers Seq}   */
   void Visit(const Triple_std<ot::T_owl_AllDifferent>& tr );

   /** @param tr {_:x, rdf:type, owl:AllDisjointClasses},
    {_:x owl:members Seq}   */
   void Visit(const Triple_std<ot::T_owl_AllDisjointClasses>& tr);

   /** @param tr {_:x, rdf:type, owl:AllDisjointProperties},
    {_:x owl:members Seq}   */
   void Visit(const Triple_std<ot::T_owl_AllDisjointProperties>& tr);

   /**  */
   void Visit(const Triple_std<ot::T_owl_allValuesFrom>& );

   /**  */
   void Visit(const Triple_std<ot::T_owl_AsymmetricProperty>& tr) {
      kernel_.setAsymmetric( property( tr.subject() ) );
   }

   /**  */
   void Visit(const Triple_std<ot::T_owl_cardinality>& tr);

   /**  */
   void Visit(const Triple_std<ot::T_owl_Class>& tr) {
      kernel_.declare( concept( tr.subject() ) );
   }

   /**  */
   void Visit(const Triple_std<ot::T_owl_complementOf>& );

   /**  */
   void Visit(const Triple_std<ot::T_owl_datatypeComplementOf>& );

   /** @param tr {instance1 differentFrom instance2} */
   void Visit(const Triple_std<ot::T_owl_differentFrom>& tr);

   /**  */
   void Visit(const Triple_std<ot::T_owl_disjointUnionOf>& );

   /**  */
   void Visit(const Triple_std<ot::T_owl_disjointWith>& );

   /**  */
   void Visit(const Triple_std<ot::T_owl_equivalentClass>& );

   /**  */
   void Visit(const Triple_std<ot::T_owl_equivalentProperty>& );

   /**  */
   void Visit(const Triple_std<ot::T_owl_FunctionalProperty>& tr) {
      kernel_.setOFunctional( property( tr.subject() ) );
   }

   /**  */
   void Visit(const Triple_std<ot::T_owl_hasKey>& );

   /**  */
   void Visit(const Triple_std<ot::T_owl_hasSelf>& );

   /**  */
   void Visit(const Triple_std<ot::T_owl_hasValue>& );

   /**  */
   void Visit(const Triple_std<ot::T_owl_intersectionOf>& );

   /**  */
   void Visit(const Triple_std<ot::T_owl_InverseFunctionalProperty>& tr) {
      kernel_.setInverseFunctional( property( tr.subject() ) );
   }

   /**  */
   void Visit(const Triple_std<ot::T_owl_inverseOf>& tr) {
      kernel_.setInverseRoles( property( tr.subject() ), property( tr.object() ) );
   }

   /**  */
   void Visit(const Triple_std<ot::T_owl_IrreflexiveProperty>& tr) {
      kernel_.setIrreflexive( property( tr.subject() ) );
   }

   /**  */
   void Visit(const Triple_std<ot::T_owl_maxCardinality>& tr);

   /**  */
   void Visit(const Triple_std<ot::T_owl_maxQualifiedCardinality>& tr);

   /**  */
   void Visit(const Triple_std<ot::T_owl_minCardinality>& tr);

   /**  */
   void Visit(const Triple_std<ot::T_owl_minQualifiedCardinality>& tr);

   /**  */
   void Visit(const Triple_std<ot::T_owl_NamedIndividual>& tr ) {
      kernel_.declare( instance( tr.subject() ) );
   }

   /**  */
   void Visit(const Triple_std<ot::T_owl_NegativePropertyAssertion>& );

   /**  */
   void Visit(const Triple_std<ot::T_owl_ObjectProperty>& triple) {
      kernel_.declare( property( triple.subject() ) );
   }

   /** @param tr {class owl:oneOf sequence} */
   void Visit(const Triple_std<ot::T_owl_oneOf>& tr);

   /**  */
   void Visit(const Triple_std<ot::T_owl_propertyChainAxiom>& tr);

   /**  */
   void Visit(const Triple_std<ot::T_owl_propertyDisjointWith>& );

   /**  */
   void Visit(const Triple_std<ot::T_owl_qualifiedCardinality>& tr);

   /**  */
   void Visit(const Triple_std<ot::T_owl_ReflexiveProperty>& tr) {
      kernel_.setReflexive( property( tr.subject() ) );
   }

   /**  */
   void Visit(const Triple_std<ot::T_owl_SymmetricProperty>& tr) {
      kernel_.setSymmetric( property( tr.subject() ) );
   }

   /**  */
   void Visit(const Triple_std<ot::T_owl_sameAs>& );

   /**  */
   void Visit(const Triple_std<ot::T_owl_someValuesFrom>& );

   /**  */
   void Visit(const Triple_std<ot::T_owl_TransitiveProperty>& tr) {
      kernel_.setTransitive( property( tr.subject() ) );
   }

   /** Subject equivalent to union of classes in object list */
   void Visit(const Triple_std<ot::T_owl_unionOf>& );

   /**  */
   void Visit(const Triple_std<ot::T_owl_withRestrictions>& );

   /**  */
   void Visit(const Triple_std<ot::T_rdfs_Datatype>& );

   /**  */
   void Visit(const Triple_std<ot::T_rdfs_domain>& tr) {
      kernel_.setODomain( property(tr.subject()), concept(tr.object()) );
   }

   /**  */
   void Visit(const Triple_std<ot::T_rdfs_range>& tr){
      kernel_.setORange( property(tr.subject()), concept(tr.object()) );
   }

   /**  */
   void Visit(const Triple_std<ot::T_rdfs_subClassOf>& tr) {
      kernel_.impliesConcepts(
            concept( tr.subject() ),
            concept( tr.object() )
      );
   }

   /**  */
   void Visit(const Triple_std<ot::T_rdfs_subPropertyOf>& tr) {
      kernel_.impliesORoles(
            property( tr.subject() ),
            property( tr.object() )
      );
   }

private:
   const Triple_store& store_;
   ReasoningKernel& kernel_;

//   TExpressionManager* e_manager();
   TExpressionManager& e_manager() { return *kernel_.getExpressionManager(); }

   /** */
   TDLConceptExpression* concept(const Node_id nid){
      return owlcpp::concept(nid, store_, kernel_);
   }

   /** */
   TDLIndividualExpression* instance(const Node_id nid){
      return owlcpp::instance(nid, store_, kernel_);
   }

   /** make instance of a class */
   TDLIndividualExpression* instance(const Node_id i, const Node_id c);

   /** */
   TDLObjectRoleExpression* property(const Node_id nid){
      return owlcpp::property(nid, store_, kernel_);
   }
};


}//namespace owlcpp

#endif /* FACT_VISITOR_HPP_ */
