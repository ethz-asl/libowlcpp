/** @file "/owlcpp/lib/logic/find_node_declaration.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef LOGIC_FIND_NODE_DECLARATION_HPP_
#define LOGIC_FIND_NODE_DECLARATION_HPP_
#include "boost/foreach.hpp"
#include "boost/range.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/print_node.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/logic/exception.hpp"

namespace owlcpp{ namespace logic{

/**@brief 
*******************************************************************************/
template<class Decl>
inline Decl declaration(const Node_id nid, Triple_store const& ts) {
   using namespace owlcpp::terms;
   Decl d;
   try {
      if( ts.is_standard(nid) ) d.set(nid);

      BOOST_FOREACH( Triple const& t, ts.find_triple(nid, any, any, any) ) {
         if( t.pred_ == rdf_type::id() ) {
            d.set(t.obj_);
            continue;
         }
         d.set(t.pred_);
      }

      BOOST_FOREACH(
               Triple const& t,
               ts.find_triple(any, owl_annotatedSource::id(), nid, any)
      ) {
         const Node_id x = t.subj_;
         if( ts[x].ns_id() != blank::id() ) BOOST_THROW_EXCEPTION(
                  Logic_err()
                  << Logic_err::msg_t("non-blank subject in _:x owl:annotatedSource y")
                  << Logic_err::node_id_t(x)
         );
         BOOST_FOREACH(
                  Triple const& t,
                  ts.find_triple(x, owl_annotatedTarget::id(), any, any)
         ) {
            d.set(t.obj_);
         }
      }
   } catch( Logic_err const& e) {
      if(
               Node_id const* nidp =
                        boost::get_error_info<Logic_err::node_id_t>(e)
      ) {
         e << Logic_err::str1_t(to_string(*nidp, ts));
      }

      Logic_err e2;
      e2
      << Logic_err::msg_t("node declaration error")
      << Logic_err::str1_t(to_string(nid, ts))
      << Logic_err::nested_t(boost::copy_exception(e))
      ;
      BOOST_THROW_EXCEPTION(e2);
   }
   return d;
}

/**@brief
*******************************************************************************/
template<class Decl> inline void
check_declaration(const Node_id nid, const Decl d, Triple_store const& ts) {
   const Decl nt = declaration<Decl>(nid, ts);
   if( nt != d ) BOOST_THROW_EXCEPTION(
               Logic_err()
               << Logic_err::msg_t(
                        "node is " + nt.to_string() +
                        "; should be " + d.to_string()
               )
               << Logic_err::str1_t(to_string(nid, ts))
   );
}

/**@brief
*******************************************************************************/
template<class Decl, class Range> inline void
check_seq_declaration(Range& r, const Decl d, Triple_store const& ts) {
   BOOST_FOREACH(const Node_id nid, r) {
      const Decl nt = declaration<Decl>(nid, ts);
      if( nt != d ) BOOST_THROW_EXCEPTION(
               Logic_err()
               << Logic_err::msg_t(
                        "node is " + nt.to_string() +
                        "; should be " + d.to_string()
               )
               << Logic_err::str1_t(to_string(nid, ts))
      );
   }
}

/**@brief
*******************************************************************************/
template<class Decl, class Range>
inline Decl check_seq_declaration(Range& r, Triple_store const& ts) {
   boost::sub_range<Range> bsr(r);
   if( ! bsr ) BOOST_THROW_EXCEPTION(
            Logic_err()
            << Logic_err::msg_t("empty sequence")
   );
   const Decl d = declaration<Decl>(bsr.front(), ts);
   if( d.is_none() ) BOOST_THROW_EXCEPTION(
            Logic_err()
            << Logic_err::msg_t("node " + Decl::name() + " declaration not found")
            << Logic_err::str1_t(to_string(bsr.front(), ts))
   );
   bsr.advance_begin(1);
   check_seq_declaration(bsr, d, ts);
   return d;
}

/**@brief
*******************************************************************************/
template<class Decl> inline Decl
check_same_declaration(const Node_id n1, const Node_id n2, Triple_store const& ts) {
   const Decl d1 = declaration<Decl>(n1, ts);
   if( d1.is_none() ) BOOST_THROW_EXCEPTION(
            Logic_err()
            << Logic_err::msg_t("node " + Decl::name() + " declaration not found")
            << Logic_err::str1_t(to_string(n1, ts))
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
#endif /* LOGIC_FIND_NODE_DECLARATION_HPP_ */
