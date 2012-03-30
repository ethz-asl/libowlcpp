/** @file "/owlcpp/include/owlcpp/logic/query_fact.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef QUERY_FACT_HPP_
#define QUERY_FACT_HPP_
#include <string>
#include <vector>
#include "owlcpp/logic/config.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/exception.hpp"
#include "predicate_fact_abstract.hpp"

class ReasoningKernel;
class TDLConceptExpression;
class TDLIndividualExpression;
class TDLObjectRoleExpression;

namespace owlcpp{

struct fact_query_err : public base_exception {};

class Triple_store;

/**
*******************************************************************************/
OWLCPP_LOGIC_DECL TDLConceptExpression* concept(
      const Node_id nid,
      const Triple_store& store,
      ReasoningKernel& kernel
);

/**
*******************************************************************************/
OWLCPP_LOGIC_DECL TDLConceptExpression* concept(
      const std::string& uri,
      const std::string& class_term,
      const Triple_store& store,
      ReasoningKernel& kernel
);

/**
@throw exception if not present in store
*******************************************************************************/
OWLCPP_LOGIC_DECL TDLConceptExpression* concept(
      const std::string& iri,
      const Triple_store& store,
      ReasoningKernel& kernel
);

/**
*******************************************************************************/
OWLCPP_LOGIC_DECL TDLIndividualExpression* instance(
      const Node_id nid,
      const Triple_store& store,
      ReasoningKernel& kernel
);


/**
*******************************************************************************/
OWLCPP_LOGIC_DECL TDLIndividualExpression* instance(
      const std::string& uri,
      const std::string& instance_term,
      const Triple_store& store,
      ReasoningKernel& kernel
);

/*
*******************************************************************************/
OWLCPP_LOGIC_DECL TDLObjectRoleExpression* property(
      const Node_id nid,
      const Triple_store& store,
      ReasoningKernel& kernel
);


/**
*******************************************************************************/
OWLCPP_LOGIC_DECL TDLObjectRoleExpression* property(
      const std::string& uri,
      const std::string& property_term,
      const Triple_store& store,
      ReasoningKernel& kernel
);

/**
*******************************************************************************/
OWLCPP_LOGIC_DECL TDLObjectRoleExpression* property(
      const std::string& iri,
      const Triple_store& store,
      ReasoningKernel& kernel
);

/**
*******************************************************************************/
OWLCPP_LOGIC_DECL bool is_subclass(
      const std::string& sub,
      const std::string& super,
      const Triple_store& store,
      ReasoningKernel& kernel
);

/** Given individuals c1i->c1 and c2i->c2,
determine if relationship c1i.p(c2i) is satisfiable
@return true if c2 can be related to c1 by p
*******************************************************************************/
OWLCPP_LOGIC_DECL bool can_relate(
      const std::string& c1,
      const std::string& p,
      const std::string& c2,
      const Triple_store& store,
      ReasoningKernel& kernel
);

/**
*******************************************************************************/
OWLCPP_LOGIC_DECL bool can_relate(
      const TDLConceptExpression* c1e,
      const TDLObjectRoleExpression* pe,
      const TDLConceptExpression* c2e,
      ReasoningKernel& kernel
);

/**
*******************************************************************************/
OWLCPP_LOGIC_DECL void get_sub_super(
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
OWLCPP_LOGIC_DECL void get_sub_super(
      const std::vector<const CE_predicate*>& pred,
      std::vector<std::string>& cv,
      const TDLConceptExpression* ce,
      const bool super,
      ReasoningKernel& kernel,
      const bool direct = false
);

/**@brief set top/bottom property names as short OWL names
*******************************************************************************/
OWLCPP_LOGIC_DECL void set_top_bottom(ReasoningKernel&);


/**
*******************************************************************************/
}//namespace owlcpp

#endif /* QUERY_FACT_HPP_ */
