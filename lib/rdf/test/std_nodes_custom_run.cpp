/** @file "/owlcpp/lib/rdf/test/std_nodes_custom_run.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#define BOOST_TEST_MODULE std_nodes_custom_run
#include "boost/test/unit_test.hpp"
#include "boost/mpl/for_each.hpp"
#include "boost/preprocessor/repetition/repeat_from_to.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/map_std.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/terms/node_iri_tag_macro.hpp"

#include "iri_tag_vector.hpp"
#include "node_tag_vector_system.hpp"
#include "node_tag_vector_owl.hpp"

namespace owlcpp{ namespace test{

OWLCPP_NAMESPACE_TAG_TYPE(
         custom, "cst", "http://custom-ns.net", OWLCPP_NAMESPACE_TAGS_TOTAL
);


#define TUPLE(z, n, text) ((custom)(BOOST_PP_CAT(text, n)))

#define CUSTOM_TERMS_1_N0_ OWLCPP_NODE_TAGS_TOTAL
#define CUSTOM_TERMS_1 BOOST_PP_REPEAT_FROM_TO_1ST(0, 50, TUPLE, term)
#define CUSTOM_TERMS_2_N0_ (OWLCPP_NODE_TAGS_TOTAL + 50)
#define CUSTOM_TERMS_2 BOOST_PP_REPEAT_FROM_TO_1ST(50, 100, TUPLE, term)
#define CUSTOM_TERMS_3_N0_ (OWLCPP_NODE_TAGS_TOTAL + 100)
#define CUSTOM_TERMS_3 BOOST_PP_REPEAT_FROM_TO_1ST(100, 150, TUPLE, term)
#define CUSTOM_TERMS_4_N0_ (OWLCPP_NODE_TAGS_TOTAL + 150)
#define CUSTOM_TERMS_4 BOOST_PP_REPEAT_FROM_TO_1ST(150, 200, TUPLE, term)

#define OWLCPP_MANY_NODE_TAG_TYPE_MACRO_(r, n0, i, e) \
OWLCPP_NODE_TAG_TYPE( \
         OWLCPP_NODE_TYPE_NAME(e), \
         BOOST_PP_SEQ_ELEM(0,e), \
         BOOST_PP_STRINGIZE(BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_REVERSE(e))), \
         (n0 + i) \
); \
BOOST_PP_EMPTY() \
/* */

#define OWLCPP_GENERATE_MANY_NODE_TAGS(seq, n0) \
         BOOST_PP_SEQ_FOR_EACH_I(OWLCPP_MANY_NODE_TAG_TYPE_MACRO_, n0, seq)


OWLCPP_GENERATE_MANY_NODE_TAGS(CUSTOM_TERMS_1, CUSTOM_TERMS_1_N0_)
OWLCPP_GENERATE_MANY_NODE_TAGS(CUSTOM_TERMS_2, CUSTOM_TERMS_2_N0_)
OWLCPP_GENERATE_MANY_NODE_TAGS(CUSTOM_TERMS_3, CUSTOM_TERMS_3_N0_)
OWLCPP_GENERATE_MANY_NODE_TAGS(CUSTOM_TERMS_4, CUSTOM_TERMS_4_N0_)
#include OWLCPP_MPL_VECTORnn_HPP(CUSTOM_TERMS_1)

typedef OWLCPP_TERM_MPL_VECTOR(CUSTOM_TERMS_1) mpl_vector_terms_custom1_t;
typedef OWLCPP_TERM_MPL_VECTOR(CUSTOM_TERMS_2) mpl_vector_terms_custom2_t;
typedef OWLCPP_TERM_MPL_VECTOR(CUSTOM_TERMS_3) mpl_vector_terms_custom3_t;
typedef OWLCPP_TERM_MPL_VECTOR(CUSTOM_TERMS_4) mpl_vector_terms_custom4_t;

struct Custom_nodes {
   void operator()(Map_std& map) const {
      owlcpp::detail::Ns_inserter ns_ins(map);
      boost::mpl::for_each<terms::mpl_vector_iris_t>(ns_ins);

      owlcpp::detail::Node_inserter node_ins(map);
      boost::mpl::for_each<terms::mpl_vector_terms_rdfs_t>(node_ins);
      boost::mpl::for_each<terms::mpl_vector_terms_rdf_t>(node_ins);
      boost::mpl::for_each<terms::mpl_vector_terms_xsd_t>(node_ins);
      boost::mpl::for_each<terms::mpl_vector_terms_owl1_t>(node_ins);
      boost::mpl::for_each<terms::mpl_vector_terms_owl2_t>(node_ins);
      boost::mpl::for_each<mpl_vector_terms_custom1_t>(node_ins);
      boost::mpl::for_each<mpl_vector_terms_custom2_t>(node_ins);
      boost::mpl::for_each<mpl_vector_terms_custom3_t>(node_ins);
      boost::mpl::for_each<mpl_vector_terms_custom4_t>(node_ins);
   }
};

namespace t = owlcpp::terms;

/** Standard map with OWL nodes plus 200 user-defined ones
*******************************************************************************/
BOOST_AUTO_TEST_CASE( case01 ) {
   Map_std const& ms = Map_std::get(Custom_nodes());

   BOOST_REQUIRE( ms.find(t::owl_Class::id()) );
   BOOST_CHECK_EQUAL(ms[t::owl_Class::id()].fragment(), "Class");
   BOOST_CHECK_GT(custom_term190::id()(), 300U);
   BOOST_REQUIRE( ms.find(custom_term190::id()) );
   BOOST_CHECK_EQUAL(ms[custom_term190::id()].fragment(), "term190");
}

}//namespace test
}//namespace owlcpp
