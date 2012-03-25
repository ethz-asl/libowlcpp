/** @file "/owlcpp/include/owlcpp/logic/detail/node_declaration.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_DECLARATION_HPP_
#define NODE_DECLARATION_HPP_
#include "boost/foreach.hpp"
#include "boost/range.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/query_node.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/logic/detail/node_type.hpp"
#include "owlcpp/logic/detail/node_property.hpp"
#include "owlcpp/logic/exception.hpp"

namespace owlcpp{ namespace logic{

/**@brief 
*******************************************************************************/
template<class Decl> Decl declaration(const Node_id nid, Triple_store const& ts) {
   using namespace owlcpp::terms;
   Decl d;
   if( ts.is_standard(nid) ) {
      switch (nid()) {
      case T_owl_Thing::index:
      case T_owl_Nothing::index:
         d.set(T_owl_Class::id());
         break;

      case T_owl_topObjectProperty::index:
      case T_owl_bottomObjectProperty::index:
         d.set(T_owl_ObjectProperty::id());
         break;

      case T_rdfs_Literal::index:
         d.set(T_owl_DataRange::id());
         break;

      case T_owl_topDataProperty::index:
      case T_owl_bottomDataProperty::index:
         d.set(T_owl_DatatypeProperty::id());
         break;

      default: BOOST_THROW_EXCEPTION(
               Logic_err()
               << Logic_err::msg_t("unknown node type")
               << Logic_err::str1_t(to_string_short(nid, ts))
      );
      }
   }

   BOOST_FOREACH( Triple const& t, ts.triples().find(nid, any(), any(), any()) ) {
      if( t.predicate() == T_rdf_type::id() ) {
         d.set(t.object());
         continue;
      }
      d.set(t.predicate());
   }

   BOOST_FOREACH(
            Triple const& t,
            ts.triples().find(any(), T_owl_annotatedSource::id(), nid, any())) {

      const Node_id x = t.subject();
      if( ts[x].ns_id() != N_blank::id() ) BOOST_THROW_EXCEPTION(
               Logic_err()
               << Logic_err::msg_t("non-blank subject in _:x owl:annotatedSource y")
               << Logic_err::str1_t(to_string_short(nid, ts))
      );
      BOOST_FOREACH(
               Triple const& t,
               ts.triples().find(x, T_owl_annotatedTarget::id(), any(), any())) {
         d.set(t.object());
      }
   }
   return d;
}

/**@brief
*******************************************************************************/
template<class Decl>
void check_declaration(const Node_id nid, const Decl d, Triple_store const& ts) {
   const Decl nt = declaration<Decl>(nid, ts);
   if( nt != d ) BOOST_THROW_EXCEPTION(
               Logic_err()
               << Logic_err::msg_t(
                        "node is " + nt.to_string() +
                        "; should be " + d.to_string()
               )
               << Logic_err::str1_t(to_string_short(nid, ts))
   );
}

/**@brief
*******************************************************************************/
template<class Decl, class Range>
void check_seq_declaration(Range& r, const Decl d, Triple_store const& ts) {
   BOOST_FOREACH(const Node_id nid, r) {
      const Decl nt = declaration<Decl>(nid, ts);
      if( nt != d ) BOOST_THROW_EXCEPTION(
               Logic_err()
               << Logic_err::msg_t(
                        "node is " + nt.to_string() +
                        "; should be " + d.to_string()
               )
               << Logic_err::str1_t(to_string_short(nid, ts))
      );
   }
}

/**@brief
*******************************************************************************/
template<class Decl, class Range>
Decl check_seq_declaration(Range& r, Triple_store const& ts) {
   boost::sub_range<Range> bsr(r);
   if( ! bsr ) BOOST_THROW_EXCEPTION(
            Logic_err()
            << Logic_err::msg_t("empty sequence")
   );
   const Decl d = declaration<Decl>(bsr.front(), ts);
   if( d.is_none() ) BOOST_THROW_EXCEPTION(
            Logic_err()
            << Logic_err::msg_t("node " + Decl::name() + " declaration not found")
            << Logic_err::str1_t(to_string_short(bsr.front(), ts))
   );
   bsr.advance_begin(1);
   check_seq_declaration(bsr, d, ts);
   return d;
}

/**@brief
*******************************************************************************/
template<class Decl> Decl
check_same_declaration(const Node_id n1, const Node_id n2, Triple_store const& ts) {
   const Decl d1 = declaration<Decl>(n1, ts);
   if( d1.is_none() ) BOOST_THROW_EXCEPTION(
            Logic_err()
            << Logic_err::msg_t("node " + Decl::name() + " declaration not found")
            << Logic_err::str1_t(to_string_short(n1, ts))
   );
   const Decl d2 = declaration<Decl>(n2, ts);
   if( d1 != d2 ) BOOST_THROW_EXCEPTION(
               Logic_err()
               << Logic_err::msg_t("node type mismatch")
               << Logic_err::str1_t(d1.to_string())
               << Logic_err::str2_t(d2.to_string())
   );
   return d1;
}

}//namespace logic
}//namespace owlcpp
#endif /* NODE_DECLARATION_HPP_ */
