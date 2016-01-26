/** @file "/owlcpp/lib/logic/factpp/query_fact.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef OWLCPP_LOGIC_SOURCE
#define OWLCPP_LOGIC_SOURCE
#endif
#include "owlcpp/logic/reasoner/query_fact.hpp"
#include "boost/assert.hpp"
#include "boost/shared_ptr.hpp"
#include "fact++/Kernel.h"
#include "owlcpp/logic/exception.hpp"
#include "owlcpp/terms/term_methods.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/rdf/triple_store.hpp"

//TODO: factor out boilerplate
namespace owlcpp {
namespace{

/**@brief Accumulate names
*******************************************************************************/
template<class AccessPolicy> class Accumulate_names {
   typedef std::vector<std::string> vector;
   typedef boost::shared_ptr<vector> pvector;
public:
   Accumulate_names() : v_(new vector()) {}

   pvector result() {return v_;}
   void clear() {v_->clear();}

   bool apply(TaxonomyVertex const& tv ) {
      store(tv.getPrimer());
      for (auto i: tv.synonyms()) {
         store(i);
      }
      return ! v_->empty();
   }

private:
   pvector v_;

   void store(ClassifiableEntry const* ce) {
      if( (! ce->isSystem()) && AccessPolicy::applicable(ce) )
         v_->push_back(AccessPolicy::name(ce));
   }
};

/**@brief Accumulate node IDs
*******************************************************************************/
template<class AccessPolicy> class Accumulate_ids {
   typedef std::vector<Node_id> vector;
   typedef boost::shared_ptr<vector> pvector;
public:
   explicit Accumulate_ids(Triple_store const& ts)
   : v_(new vector()), ts_(ts) {}

   pvector result() {return v_;}
   void clear() {v_->clear();}

   bool apply(TaxonomyVertex const& tv ) {
      store(tv.getPrimer());
      for (auto i: tv.synonyms()) {
         store(i);
      }
      return ! v_->empty();
   }

private:
   pvector v_;
   Triple_store const& ts_;

   void store(ClassifiableEntry const* ce) {
      if( ce->isSystem() || (! AccessPolicy::applicable(ce)) ) return;
      const std::string name = AccessPolicy::name(ce);
      if( Node_id const* pid = ts_.find_node_iri(name) ) {
         v_->push_back(*pid);
      } else {
         BOOST_THROW_EXCEPTION(
                  Logic_err()
                  << Logic_err::msg_t("Node IRI not found")
                  << Logic_err::str1_t(name)
         );
      }
   }
};

/**@brief Object type policy for FaCT actor
*******************************************************************************/
struct OT_policy {
   static bool applicable(ClassifiableEntry const* ce) {
      BOOST_ASSERT( dynamic_cast<TConcept const*>(ce) );
      return ! static_cast<TConcept const*>(ce)->isSingleton();
   }

   static std::string name(ClassifiableEntry const* ce) {
      if( ce->getId() >= 0 ) return ce->getName();
      if( ce->getName() == std::string("TOP") )
         return to_string_full(terms::owl_Thing());
      if( ce->getName() == std::string("BOTTOM") )
         return to_string_full(terms::owl_Nothing());

      BOOST_THROW_EXCEPTION(
               Logic_err()
               << Logic_err::msg_t("unknown object type node")
      );
   }
};

/**@brief Object instance policy for FaCT actor
*******************************************************************************/
struct OI_policy {
   static bool applicable(ClassifiableEntry const* ce) {
      BOOST_ASSERT( dynamic_cast<TConcept const*>(ce) );
      return static_cast<TConcept const*>(ce)->isSingleton();
   }

   static std::string name(ClassifiableEntry const* ce) {return ce->getName();}
};

/**@brief Object property policy for FaCT actor
*******************************************************************************/
struct OP_policy {
   static bool applicable(ClassifiableEntry const* ce) {
      BOOST_ASSERT( dynamic_cast<TRole const*>(ce) );
      return ce->getId() >= 0;
   }

   static std::string name(ClassifiableEntry const* ce) {return ce->getName();}
};

/**@brief Data property policy for FaCT actor
*******************************************************************************/
struct DP_policy {
   static bool applicable(ClassifiableEntry const* ce) {
      BOOST_ASSERT( dynamic_cast<TRole const*>(ce) );
      return ce->getId() > 0;
   }

   static std::string name(ClassifiableEntry const* ce) {return ce->getName();}
};

}//namespace anonymous

/*
*******************************************************************************/
FaCT_query<std::string>::range get_sub(
         TDLConceptExpression const* ote,
         ReasoningKernel& k,
         const bool direct_only
) {
   Accumulate_names<OT_policy> a;
   k.getSubConcepts(ote, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<std::string>::range
get_super(
         TDLConceptExpression const* ote,
         ReasoningKernel& k,
         const bool direct_only
) {
   Accumulate_names<OT_policy> a;
   k.getSupConcepts(ote, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<std::string>::range
get_same(
         TDLConceptExpression const* ote,
         ReasoningKernel& k
) {
   Accumulate_names<OT_policy> a;
   k.getEquivalentConcepts(ote, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<std::string>::range
get_instances(
         TDLConceptExpression const* ote,
         ReasoningKernel& k,
         const bool direct_only
) {
   Accumulate_names<OI_policy> a;
   if( direct_only ) k.getDirectInstances(ote, a);
   else k.getInstances(ote, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<std::string>::range
get_super(
         TDLIndividualExpression const* oie,
         ReasoningKernel& k,
         const bool direct_only
) {
   Accumulate_names<OT_policy> a;
   k.getTypes(oie, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<std::string>::range
get_same(
         TDLIndividualExpression const* oie,
         ReasoningKernel& k
) {
   Accumulate_names<OI_policy> a;
   k.getSameAs(oie, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<std::string>::range
get_sub(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         const bool direct_only
) {
   Accumulate_names<OP_policy> a;
   k.getSubRoles(ope, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<std::string>::range
get_super(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         const bool direct_only
) {
   Accumulate_names<OP_policy> a;
   k.getSupRoles(ope, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<std::string>::range
get_same(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k
) {
   Accumulate_names<OP_policy> a;
   k.getEquivalentRoles(ope, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<std::string>::range
get_domain(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         const bool direct_only
) {
   Accumulate_names<OT_policy> a;
   k.getORoleDomain(ope, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<std::string>::range
get_range(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         const bool direct_only
) {
   Accumulate_names<OT_policy> a;
   k.getRoleRange(ope, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<std::string>::range
get_sub(
         TDLDataRoleExpression const* dpe,
         ReasoningKernel& k,
         const bool direct_only
) {
   Accumulate_names<DP_policy> a;
   k.getSubRoles(dpe, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<std::string>::range
get_super(
         TDLDataRoleExpression const* dpe,
         ReasoningKernel& k,
         const bool direct_only
) {
   Accumulate_names<DP_policy> a;
   k.getSupRoles(dpe, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<std::string>::range
get_same(
         TDLDataRoleExpression const* dpe,
         ReasoningKernel& k
) {
   Accumulate_names<DP_policy> a;
   k.getEquivalentRoles(dpe, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<std::string>::range
get_domain(
         TDLDataRoleExpression const* dpe,
         ReasoningKernel& k,
         const bool direct_only
) {
   Accumulate_names<OT_policy> a;
   k.getDRoleDomain(dpe, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<Node_id>::range get_sub(
         TDLConceptExpression const* ote,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only
) {
   Accumulate_ids<OT_policy> a(ts);
   k.getSubConcepts(ote, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<Node_id>::range
get_super(
         TDLConceptExpression const* ote,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only
) {
   Accumulate_ids<OT_policy> a(ts);
   k.getSupConcepts(ote, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<Node_id>::range
get_same(
         TDLConceptExpression const* ote,
         ReasoningKernel& k,
         Triple_store const& ts
) {
   Accumulate_ids<OT_policy> a(ts);
   k.getEquivalentConcepts(ote, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<Node_id>::range
get_instances(
         TDLConceptExpression const* ote,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only
) {
   Accumulate_ids<OI_policy> a(ts);
   if( direct_only ) k.getDirectInstances(ote, a);
   else k.getInstances(ote, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<Node_id>::range
get_super(
         TDLIndividualExpression const* oie,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only
) {
   Accumulate_ids<OT_policy> a(ts);
   k.getTypes(oie, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<Node_id>::range
get_same(
         TDLIndividualExpression const* oie,
         ReasoningKernel& k,
         Triple_store const& ts
) {
   Accumulate_ids<OI_policy> a(ts);
   k.getSameAs(oie, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<Node_id>::range
get_sub(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only
) {
   Accumulate_ids<OP_policy> a(ts);
   k.getSubRoles(ope, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<Node_id>::range
get_super(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only
) {
   Accumulate_ids<OP_policy> a(ts);
   k.getSupRoles(ope, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<Node_id>::range
get_same(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         Triple_store const& ts
) {
   Accumulate_ids<OP_policy> a(ts);
   k.getEquivalentRoles(ope, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<Node_id>::range
get_domain(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only
) {
   Accumulate_ids<OT_policy> a(ts);
   k.getORoleDomain(ope, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<Node_id>::range
get_range(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only
) {
   Accumulate_ids<OT_policy> a(ts);
   k.getRoleRange(ope, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<Node_id>::range
get_sub(
         TDLDataRoleExpression const* dpe,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only
) {
   Accumulate_ids<DP_policy> a(ts);
   k.getSubRoles(dpe, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<Node_id>::range
get_super(
         TDLDataRoleExpression const* dpe,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only
) {
   Accumulate_ids<DP_policy> a(ts);
   k.getSupRoles(dpe, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<Node_id>::range
get_same(
         TDLDataRoleExpression const* dpe,
         ReasoningKernel& k,
         Triple_store const& ts
) {
   Accumulate_ids<DP_policy> a(ts);
   k.getEquivalentRoles(dpe, a);
   return boost::make_shared_container_range(a.result());
}

/*
*******************************************************************************/
FaCT_query<Node_id>::range
get_domain(
         TDLDataRoleExpression const* dpe,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only
) {
   Accumulate_ids<OT_policy> a(ts);
   k.getDRoleDomain(dpe, direct_only, a);
   return boost::make_shared_container_range(a.result());
}

}//namespace owlcpp
