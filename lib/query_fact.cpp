/** @file "/owl_cpp/lib/query_fact.cpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#include "owl_cpp/query_fact.hpp"
#include "Kernel.h"
#include "owl_cpp/triple_store.hpp"
#include "owl_cpp/query_nodes.hpp"
#include "owl_cpp/terms/term_tags.hpp"
namespace ot = owl_cpp::terms;

namespace owl_cpp {

/**
*******************************************************************************/
TDLConceptExpression* concept(
      const Node_id nid,
      const Triple_store& store,
      ReasoningKernel& kernel
) {
   const std::string name = short_name(nid, store);
   return kernel.getExpressionManager()->Concept(name);
}

/*
*******************************************************************************/
TDLConceptExpression* concept(
      const std::string& uri,
      const std::string& class_term,
      const Triple_store& store,
      ReasoningKernel& kernel
) {
   const Node_id nid = find_node(uri, class_term, store);
   return concept(nid, store, kernel);
}

/*
*******************************************************************************/
TDLConceptExpression* concept(
      const std::string& iri,
      const Triple_store& store,
      ReasoningKernel& kernel
) {
   const Node_id n = find_node(iri, store);
   return concept(n, store, kernel);
}


/*
*******************************************************************************/
TDLIndividualExpression* instance(
      const Node_id nid,
      const Triple_store& store,
      ReasoningKernel& kernel
) {
   const std::string name = short_name(nid, store);
   return kernel.getExpressionManager()->Individual(name);
}

/*
*******************************************************************************/
TDLIndividualExpression* instance(
      const std::string& uri,
      const std::string& instance_term,
      const Triple_store& store,
      ReasoningKernel& kernel
) {
   const Node_id nid = find_node(uri, instance_term, store);
   return instance(nid, store, kernel);
}

/*
*******************************************************************************/
TDLObjectRoleExpression* property(
      const Node_id nid,
      const Triple_store& store,
      ReasoningKernel& kernel
) {
   const std::string name = short_name(nid, store);
   return kernel.getExpressionManager()->ObjectRole(name);
}

/*
*******************************************************************************/
TDLObjectRoleExpression* property(
      const std::string& uri,
      const std::string& property_term,
      const Triple_store& store,
      ReasoningKernel& kernel
) {
   const Node_id nid = find_node(uri, property_term, store);
   return property(nid, store, kernel);
}

/*
*******************************************************************************/
TDLObjectRoleExpression* property(
      const std::string& iri,
      const Triple_store& store,
      ReasoningKernel& kernel
) {
   const b::tuple<std::string,std::string> t = Triple_store::split(iri);
   const Node_id nid = find_node(t.get<0>(), t.get<1>(), store);
   return property(nid, store, kernel);
}

/*
*******************************************************************************/
bool is_subclass(
      const std::string& c1,
      const std::string& c2,
      const Triple_store& store,
      ReasoningKernel& kernel
) {
   return kernel.isSubsumedBy(
         concept(c1, store, kernel),
         concept(c2, store, kernel)
   );
}

/*
*******************************************************************************/
bool can_relate(
      const TDLConceptExpression* c1e,
      const TDLObjectRoleExpression* pe,
      const TDLConceptExpression* c2e,
      ReasoningKernel& kernel
) {
   const TDLConceptExpression* restriction =
         kernel.getExpressionManager()->Exists( pe, c2e );
   TDLAxiom* a = kernel.impliesConcepts(restriction, c1e);
   const bool satisfiable = kernel.isSatisfiable(restriction);
   kernel.retract(a);
   return satisfiable;
}

/*
*******************************************************************************/
bool can_relate(
      const std::string& c1,
      const std::string& p,
      const std::string& c2,
      const Triple_store& store,
      ReasoningKernel& kernel
) {
   const TDLConceptExpression* c1e = concept(c1, store, kernel);
   const TDLObjectRoleExpression* pe = property(p, store, kernel);
   const TDLConceptExpression* c2e = concept(c2, store, kernel);

   if( ! kernel.isSatisfiable(c1e) ) BOOST_THROW_EXCEPTION(
         fact_query_err()
         << fact_query_err::msg_t("concept not satisfiable")
         << fact_query_err::str1_t(c1)
   );

   if( ! kernel.isSatisfiable(c2e) ) BOOST_THROW_EXCEPTION(
         fact_query_err()
         << fact_query_err::msg_t("concept not satisfiable")
         << fact_query_err::str1_t(c2)
   );

   const TDLConceptExpression* restriction =
         kernel.getExpressionManager()->Exists( pe, c2e );
   TDLAxiom* a = kernel.impliesConcepts(restriction, c1e);
   const bool satisfiable = kernel.isSatisfiable(restriction);
   kernel.retract(a);
   return satisfiable;
}

namespace{
/** Collect FaCT++ expression names that satisfy all predicates in a vector
*******************************************************************************/
class Actor_collector {
   typedef std::vector<std::string> stor_t;
   typedef std::vector<const CE_predicate*> pred_v_t;
public:
   Actor_collector(
         const pred_v_t& pred,
         TExpressionManager& em,
         stor_t& cv
   ) : pred_(pred), em_(em), cv_(cv) {}

   bool apply(const TaxonomyVertex& node) {
      const ClassifiableEntry* c = node.getPrimer();
      for(std::size_t i = 0; i != pred_.size(); ++i) {
         if( ! pred_[i]->operator()(c) ) return true;
      }
      cv_.push_back(c->getName());
      return true;
   }

private:
   const pred_v_t& pred_;
   TExpressionManager& em_;
   stor_t& cv_;
};
}//namespace anonymous

/*
*******************************************************************************/
void get_sub_super(
      const CE_predicate& pred,
      std::vector<std::string>& cv,
      const TDLConceptExpression* ce,
      const bool super,
      ReasoningKernel& kernel,
      const bool direct
) {
   typedef std::vector<const CE_predicate*> pred_v_t;
   pred_v_t pred_v(1, &pred);
   Actor_collector cc(pred_v, *kernel.getExpressionManager(), cv);
   if( super ) kernel.getSupConcepts(ce, direct, cc);
   else kernel.getSubConcepts(ce, direct, cc);
}

/*
*******************************************************************************/
void get_sub_super(
      const std::vector<const CE_predicate*>& pred,
      std::vector<std::string>& cv,
      const TDLConceptExpression* ce,
      const bool super,
      ReasoningKernel& kernel,
      const bool direct
) {
   Actor_collector cc(pred, *kernel.getExpressionManager(), cv);
   if( super ) kernel.getSupConcepts(ce, direct, cc);
   else kernel.getSubConcepts(ce, direct, cc);
}

/*
*******************************************************************************/
void set_top_bottom(ReasoningKernel& kernel) {
   static const std::string top_o( short_name(ot::T_owl_topObjectProperty()) );
   static const std::string bottom_o( short_name(ot::T_owl_bottomObjectProperty()) );
   static const std::string top_d( short_name(ot::T_owl_topDataProperty()) );
   static const std::string bottom_d( short_name(ot::T_owl_bottomDataProperty()) );
   kernel.setTopBottomRoleNames(
         top_o.c_str(),
         bottom_o.c_str(),
         top_d.c_str(),
         bottom_d.c_str()
   );
}

/*
*******************************************************************************/
}//namespace owl_cpp
