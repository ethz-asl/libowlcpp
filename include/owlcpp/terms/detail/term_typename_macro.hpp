/** @file "/owlcpp/include/owlcpp/terms/detail/term_typename_macro.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TERM_TYPENAME_MACRO_HPP_
#define TERM_TYPENAME_MACRO_HPP_

/**@brief Generate type name @details e.g.,
OWLCPP_TERM_TYPE_NAME((rdfs)(subClassOf)) produces T_rdfs_subClassOf
*******************************************************************************/
#define OWLCPP_TERM_TYPE_NAME(n) \
      BOOST_PP_CAT( \
               BOOST_PP_SEQ_ELEM(0,n), BOOST_PP_CAT( \
                        _, BOOST_PP_SEQ_ELEM(1,n) \
               ) \
            ) \
/* */

#endif /* TERM_TYPENAME_MACRO_HPP_ */
