/** @file "/owlcpp/include/owlcpp/terms/detail/uri_typename_macro.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef URI_TYPENAME_MACRO_HPP_
#define URI_TYPENAME_MACRO_HPP_

/**@brief First symbols of namespace type names
*******************************************************************************/
#define OWLCPP_NS_TYPE_PREFIX N_

/**@brief Generate namespace type name
*******************************************************************************/
#define OWLCPP_NAMESPACE_TYPE_NAME(p) \
   BOOST_PP_CAT(OWLCPP_NS_TYPE_PREFIX, BOOST_PP_SEQ_ELEM(0,p))

#endif /* URI_TYPENAME_MACRO_HPP_ */
