/** @file "/owlcpp/include/owlcpp/logic/detail/triple_to_fact_adaptor.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_TO_FACT_ADAPTOR_HPP_
#define TRIPLE_TO_FACT_ADAPTOR_HPP_
#include "boost/foreach.hpp"
#include "boost/range.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/query_node.hpp"
#include "owlcpp/logic/exception.hpp"
#include "owlcpp/logic/detail/node_type.hpp"
#include "owlcpp/logic/detail/node_property.hpp"
#include "owlcpp/logic/config.hpp"
#include "factpp/config.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

class /*FACTPP_KERNEL_DECL*/ ReasoningKernel;
class TDLConceptExpression;
class TDLIndividualExpression;
class TDLObjectRoleExpression;
class TDLDataRoleExpression;
class TDLDataTypeExpression;
class TExpressionManager;
class TDLDataValue;
class TDLAxiom;

namespace owlcpp{ namespace logic{ namespace factpp{

/**@brief 
*******************************************************************************/
class OWLCPP_REASONER_DECL Adaptor_triple {
public:
   struct Err : public Logic_err {};

   Adaptor_triple(Triple_store const& ts, ReasoningKernel& k, bool lax)
   : lax_(lax), ts_(ts), k_(k)
   {}

   void submit(Triple const& t) {axiom(t);}

   template<class Range> void submit(Range r) {
      BOOST_FOREACH(Triple const& t, r) {
         try{
            submit(t);
         } catch(...) {
            BOOST_THROW_EXCEPTION(
                     Err()
                     << Err::msg_t("error submitting triple")
                     << Err::str1_t(
                              to_string_short(t.subject(), ts_) + ' ' +
                              to_string_short(t.predicate(), ts_) + ' ' +
                              to_string_short(t.object(), ts_)
                     )
                     << Err::nested_t(boost::current_exception())
            );
         }
      }
   }

   TDLAxiom* axiom(Triple const& t);

private:
   bool lax_;
   Triple_store const& ts_;
   ReasoningKernel& k_;

   TDLAxiom* axiom_rdf_type(Triple const& t);

   /** x *:y z, where *:y is object or data property
    @param t RDF triple
    @return axiom pointer
   */
   TDLAxiom* axiom_custom_predicate(Triple const& t);

   TDLAxiom* axiom_from_seq(
            const Node_id op,
            const Node_id seq_nid,
            const std::size_t min_len,
            const Node_id subj = owlcpp::terms::T_empty_::id()
   );

   TExpressionManager& e_m();

   template<class Decl> Decl check_same_declaration(const Node_id n1, const Node_id n2) const {
      const Decl nt1 = declaration<Decl>(n1);
      const Decl nt2 = declaration<Decl>(n2);
      if( nt1 != nt2 ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("node type mismatch")
                  << Err::str1_t(nt1.to_string())
                  << Err::str2_t(nt2.to_string())
      );
      return nt1;
   }

   template<class Decl, class Range> Decl check_seq_declaration(Range& r) const {
      boost::sub_range<Range> bsr(r);
      if( ! bsr ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("empty sequence")
      );
      const Decl d = declaration<Decl>(bsr.front());
      bsr.advance_begin(1);
      check_seq_declaration(bsr, d);
      return d;
   }

   template<class Decl, class Range> void check_seq_declaration(Range& r, const Decl d) const {
      BOOST_FOREACH(const Node_id nid, r) {
         const Decl nt = declaration<Decl>(nid);
         if( nt != d ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t(
                           "node declared as " + nt.to_string() +
                           "; should be " + d.to_string()
                  )
                  << Err::str1_t(ts_.string(nid))
         );
      }
   }

   template<class Decl> void check_declaration(const Node_id nid, const Decl d) const {
      const Decl nt = declaration<Decl>(nid);
      if( nt != d ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t(
                           "node declared as " + nt.to_string() +
                           "; should be " + d.to_string()
                  )
                  << Err::str1_t(ts_.string(nid))
      );
   }

   template<class Decl> Decl declaration(const Node_id nid) const {
      using namespace owlcpp::terms;
      Decl d;
      BOOST_FOREACH(
               Triple const& t,
               ts_.triples().find(nid, T_rdf_type::id(), any(), any())) {
         d.set(t.object());
      }

      BOOST_FOREACH(
               Triple const& t,
               ts_.triples().find(any(), T_owl_annotatedSource::id(), nid, any())) {

         const Node_id x = t.subject();
         if( ts_[x].ns_id() != N_blank::id() ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("non-blank subject in _:x owl:annotatedSource y")
                  << Err::str1_t(ts_.string(nid))
         );
         BOOST_FOREACH(
                  Triple const& t,
                  ts_.triples().find(x, T_owl_annotatedTarget::id(), any(), any())) {
            d.set(t.object());
         }
      }
      if( d.is_none() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("node " + Decl::name() + " declaration not found")
               << Err::str1_t(ts_.string(nid))
      );
      return d;
   }

   /**@param t triple x y z */
   void submit_custom_triple(Triple const& t);

   TDLConceptExpression* obj_type(const Node_id nid);

   TDLIndividualExpression* obj_value(const Node_id nid);

   /** make instance of a class */
   TDLIndividualExpression* instance_of(const Node_id inst, const Node_id cls);

   TDLObjectRoleExpression* obj_property(const Node_id nid);

   TDLDataRoleExpression* data_property(const Node_id nid);

   TDLDataTypeExpression* data_type(const Node_id nid);

   TDLDataValue const* data_value(const Node_id nid);

   TDLAxiom* negative_property_assertion(const Node_id nid);
};

}//namespace factpp
}//namespace logic
}//namespace owlcpp
#endif /* TRIPLE_TO_FACT_ADAPTOR_HPP_ */
