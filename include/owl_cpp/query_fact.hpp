/** @file "/owl_cpp/include/owl_cpp/query_fact.hpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#ifndef QUERY_FACT_HPP_
#define QUERY_FACT_HPP_
#include <string>
#include <vector>
#include "owl_cpp/node_id.hpp"
#include "owl_cpp/exception.hpp"
#include "predicate_fact_abstract.hpp"

class ReasoningKernel;
class TDLConceptExpression;
class TDLIndividualExpression;
class TDLObjectRoleExpression;

namespace owl_cpp{

struct fact_query_err : public base_exception {};

class Triple_store;

/**
*******************************************************************************/
TDLConceptExpression* concept(
      const Node_id nid,
      const Triple_store& store,
      ReasoningKernel& kernel
);

/**
*******************************************************************************/
TDLConceptExpression* concept(
      const std::string& uri,
      const std::string& class_term,
      const Triple_store& store,
      ReasoningKernel& kernel
);

/**
@throw exception if not present in store
*******************************************************************************/
TDLConceptExpression* concept(
      const std::string& iri,
      const Triple_store& store,
      ReasoningKernel& kernel
);

/**
*******************************************************************************/
TDLIndividualExpression* instance(
      const Node_id nid,
      const Triple_store& store,
      ReasoningKernel& kernel
);


/**
*******************************************************************************/
TDLIndividualExpression* instance(
      const std::string& uri,
      const std::string& instance_term,
      const Triple_store& store,
      ReasoningKernel& kernel
);

/*
*******************************************************************************/
TDLObjectRoleExpression* property(
      const Node_id nid,
      const Triple_store& store,
      ReasoningKernel& kernel
);


/**
*******************************************************************************/
TDLObjectRoleExpression* property(
      const std::string& uri,
      const std::string& property_term,
      const Triple_store& store,
      ReasoningKernel& kernel
);

/**
*******************************************************************************/
TDLObjectRoleExpression* property(
      const std::string& iri,
      const Triple_store& store,
      ReasoningKernel& kernel
);

/**
*******************************************************************************/
bool is_subclass(
      const std::string& sub,
      const std::string& super,
      const Triple_store& store,
      ReasoningKernel& kernel
);

/** Given individuals c1i->c1 and c2i->c2,
determine if relationship c1i.p(c2i) is satisfiable
@return true if c2 can be related to c1 by p
*******************************************************************************/
bool can_relate(
      const std::string& c1,
      const std::string& p,
      const std::string& c2,
      const Triple_store& store,
      ReasoningKernel& kernel
);

/**
*******************************************************************************/
bool can_relate(
      const TDLConceptExpression* c1e,
      const TDLObjectRoleExpression* pe,
      const TDLConceptExpression* c2e,
      ReasoningKernel& kernel
);

/**
*******************************************************************************/
void get_sub_super(
      const CE_predicate& pred,
      std::vector<std::string>& cv,
      const TDLConceptExpression* ce,
      const bool super,
      ReasoningKernel& kernel,
      const bool direct = false
);

/** Find sub/super classes of @a ce that satisfy all conditions in @a pred
@param pred vector of predicates
@param[out] cv names of discovered class will be inserted
@param ce search for ancestors/descendants of this class
@param super search for super-classes if true
@param kernel reasoning kernel
@param direct search for direct ancestors/descendants if true
*******************************************************************************/
void get_sub_super(
      const std::vector<const CE_predicate*>& pred,
      std::vector<std::string>& cv,
      const TDLConceptExpression* ce,
      const bool super,
      ReasoningKernel& kernel,
      const bool direct = false
);

/**@brief set top/bottom property names as short OWL names
*******************************************************************************/
void set_top_bottom(ReasoningKernel&);


/**
*******************************************************************************/
}//namespace owl_cpp

#endif /* QUERY_FACT_HPP_ */
