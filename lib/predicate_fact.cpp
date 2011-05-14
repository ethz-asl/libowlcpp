/** @file "/owl_cpp/lib/predicate_fact.cpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/

#include "owl_cpp/predicate_fact.hpp"
#include "owl_cpp/query_fact.hpp"
#include "owl_cpp/exception.hpp"

namespace owl_cpp {

/*
*******************************************************************************/
Can_relate::Can_relate(const TDLObjectRoleExpression* property,
         const TDLConceptExpression* c,
         ReasoningKernel& kernel
)
: kernel_(kernel),
  restriction_(kernel_.getExpressionManager()->Exists( property, c ))
{
   if( ! kernel_.isSatisfiable(restriction_) ) BOOST_THROW_EXCEPTION(
         base_exception()
         << fact_query_err::msg_t("restriction not satisfiable")
   );
}

/*
*******************************************************************************/
Can_relate::Can_relate(
      const std::string& p,
      const std::string& c,
      const Triple_store& store,
      ReasoningKernel& kernel
)
: kernel_(kernel)
{
   const TDLConceptExpression* ce = concept(c, store, kernel_);
   if( ! kernel_.isSatisfiable(ce) ) BOOST_THROW_EXCEPTION(
         fact_query_err()
         << fact_query_err::msg_t("concept not satisfiable")
         << fact_query_err::str1_t(c)
   );

   const TDLObjectRoleExpression* pe = property(p, store, kernel_);
   restriction_ = kernel_.getExpressionManager()->Exists(pe, ce);
   if( ! kernel_.isSatisfiable(restriction_) ) BOOST_THROW_EXCEPTION(
         fact_query_err()
         << fact_query_err::msg_t("restriction not satisfiable")
         << fact_query_err::str1_t(c)
         << fact_query_err::str2_t(p)
   );
}

/*
*******************************************************************************/
}//namespace owl_cpp
