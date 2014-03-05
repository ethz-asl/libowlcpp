/** @file "/owlcpp/include/owlcpp/logic/reasoner/query_fact.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef QUERY_FACT_HPP_
#define QUERY_FACT_HPP_
#include <vector>
#include <string>
#include "owlcpp/logic/config.hpp"
#include "boost/range.hpp"
#include "boost/shared_container_iterator.hpp"

class ReasoningKernel;
class TDLConceptExpression;
class TDLIndividualExpression;
class TDLObjectRoleExpression;
class TDLDataRoleExpression;

namespace owlcpp{
class Node_id;
class Triple_store;

template<class T> struct FaCT_query{
   typedef boost::shared_container_iterator<std::vector<T> >
   iterator;
   typedef boost::iterator_range<iterator> range;
};

/**@brief find sub object types
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<std::string>::range
get_sub(
         TDLConceptExpression const* ote,
         ReasoningKernel& k,
         const bool direct_only = true
);

/**@brief find super object types
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<std::string>::range
get_super(
         TDLConceptExpression const* ote,
         ReasoningKernel& k,
         const bool direct_only = true
);

/**@brief find equivalent object types
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<std::string>::range
get_same(
         TDLConceptExpression const* ote,
         ReasoningKernel& k
);

/**@brief find instances of an object type
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<std::string>::range
get_instances(
         TDLConceptExpression const* ote,
         ReasoningKernel& k,
         const bool direct_only = true
);

/**@brief find types of an object instance
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<std::string>::range
get_super(
         TDLIndividualExpression const* oie,
         ReasoningKernel& k,
         const bool direct_only = true
);

/**@brief find same object instances
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<std::string>::range
get_same(
         TDLIndividualExpression const* oie,
         ReasoningKernel& k
);

/**@brief find sub object properties
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<std::string>::range
get_sub(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         const bool direct_only = true
);

/**@brief find super object properties
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<std::string>::range
get_super(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         const bool direct_only = true
);

/**@brief find equivalent object properties
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<std::string>::range
get_same(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k
);

/**@brief find object types in object property domain
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<std::string>::range
get_domain(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         const bool direct_only = true
);

/**@brief find object types in object property range
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<std::string>::range
get_range(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         const bool direct_only = true
);

/**@brief find sub data properties
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<std::string>::range
get_sub(
         TDLDataRoleExpression const* dpe,
         ReasoningKernel& k,
         const bool direct_only = true
);

/**@brief find super data properties
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<std::string>::range
get_super(
         TDLDataRoleExpression const* dpe,
         ReasoningKernel& k,
         const bool direct_only = true
);

/**@brief find equivalent data properties
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<std::string>::range
get_same(
         TDLDataRoleExpression const* dpe,
         ReasoningKernel& k
);

/**@brief find object types in data property domain
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<std::string>::range
get_domain(
         TDLDataRoleExpression const* dpe,
         ReasoningKernel& k,
         const bool direct_only = true
);

/**@brief find sub object types
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<Node_id>::range
get_sub(
         TDLConceptExpression const* ote,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only = true
);

/**@brief find super object types
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<Node_id>::range
get_super(
         TDLConceptExpression const* ote,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only = true
);

/**@brief find equivalent object types
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<Node_id>::range
get_same(
         TDLConceptExpression const* ote,
         ReasoningKernel& k,
         Triple_store const& ts
);

/**@brief find instances of an object type
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<Node_id>::range
get_instances(
         TDLConceptExpression const* ote,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only = true
);

/**@brief find types of an object instance
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<Node_id>::range
get_super(
         TDLIndividualExpression const* oie,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only = true
);

/**@brief find same object instances
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<Node_id>::range
get_same(
         TDLIndividualExpression const* oie,
         ReasoningKernel& k,
         Triple_store const& ts
);

/**@brief find sub object properties
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<Node_id>::range
get_sub(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only = true
);

/**@brief find super object properties
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<Node_id>::range
get_super(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only = true
);

/**@brief find equivalent object properties
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<Node_id>::range
get_same(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         Triple_store const& ts
);

/**@brief find object types in object property domain
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<Node_id>::range
get_domain(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only = true
);

/**@brief find object types in object property range
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<Node_id>::range
get_range(
         TDLObjectRoleExpression const* ope,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only = true
);

/**@brief find sub data properties
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<Node_id>::range
get_sub(
         TDLDataRoleExpression const* dpe,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only = true
);

/**@brief find super data properties
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<Node_id>::range
get_super(
         TDLDataRoleExpression const* dpe,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only = true
);

/**@brief find equivalent data properties
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<Node_id>::range
get_same(
         TDLDataRoleExpression const* dpe,
         ReasoningKernel& k,
         Triple_store const& ts
);

/**@brief find object types in data property domain
*******************************************************************************/
OWLCPP_LOGIC_DECL FaCT_query<Node_id>::range
get_domain(
         TDLDataRoleExpression const* dpe,
         ReasoningKernel& k,
         Triple_store const& ts,
         const bool direct_only = true
);

}//namespace owlcpp
#endif /* QUERY_FACT_HPP_ */
