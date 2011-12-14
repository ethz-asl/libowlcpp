/** @file "/owl_cpp/include/owl_cpp/triple.hpp"
part of owl_cpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010-1
*******************************************************************************/
#ifndef TRIPLE_HPP_
#define TRIPLE_HPP_
#include <cassert>
#include "boost/array.hpp"
namespace b = boost;
#include "loki/Visitor.h"
namespace L = Loki;
#include "owl_cpp/node_id.hpp"
#include "owl_cpp/terms/term_tags.hpp"
namespace ot = owl_cpp::terms;
#include "owl_cpp/exception.hpp"

namespace owl_cpp{

/**
*******************************************************************************/
template<class R, class Visited> struct Unsupported_type {
   static R OnUnknownVisitor(const Visited& triple, L::BaseVisitor& ) {
      switch ( triple.predicate()() ) {
      case ot::T_owl_annotatedProperty::index:
      case ot::T_owl_annotatedSource::index:
      case ot::T_owl_annotatedTarget::index:
      case ot::T_owl_assertionProperty::index:
      case ot::T_owl_distinctMembers::index:
      case ot::T_owl_imports::index:
      case ot::T_owl_members::index:
      case ot::T_owl_onClass::index:
      case ot::T_owl_onProperty::index:
      case ot::T_owl_sourceIndividual::index:
      case ot::T_owl_targetIndividual::index:
      case ot::T_owl_versionInfo::index:
      case ot::T_owl_versionIRI::index:
      case ot::T_rdf_first::index:
      case ot::T_rdf_rest::index:
      case ot::T_rdfs_comment::index:
      case ot::T_rdfs_isDefinedBy::index:
      case ot::T_rdfs_label::index:
         return;
      case ot::T_rdf_type::index:
         switch ( triple.object()() ) {
         case ot::T_owl_AnnotationProperty::index:
         case ot::T_owl_Axiom::index:
         case ot::T_owl_Ontology::index:
         case ot::T_owl_Restriction::index:
            return;
         default:
            break;
         }
         break;
         default:
            break;
      }
      BOOST_THROW_EXCEPTION(
            base_exception()
            << base_exception::msg_t( "unsupported triple" )
      );
   }
};

/** OWL triple interface
*******************************************************************************/
class Triple : public L::BaseVisitable<void, Unsupported_type, true> {
protected:
   Triple(const Node_id subj, const Node_id pred, const Node_id obj) {
      arr_[0] = subj;
      arr_[1] = pred;
      arr_[2] = obj;
   }

public:
   typedef Triple* id_t;

   bool operator<(const Triple& t) const {
      if( subject() != t.subject() ) return subject() < t.subject();
      if( predicate() != t.predicate() ) return predicate() < t.predicate();
      return object() < t.object();
   }

   bool operator==(const Triple& t) const {
      return
      subject() == t.subject()&&
      predicate() == t.predicate() &&
      object() == t.object();
   }

   template<unsigned I> Node_id get() const {return arr_[I];}
   Node_id subject()    const {return get<0>();}
   Node_id predicate()  const {return get<1>();}
   Node_id object()     const {return get<2>();}

   virtual ~Triple() {}
   LOKI_DEFINE_CONST_VISITABLE()
private:
   b::array<Node_id, 3> arr_;
};


/** Triple that uses one of the standard OWL predicates
*******************************************************************************/
template<class T>struct Triple_std : public Triple {
   typedef T tag_t;
   Triple_std(const Node_id subj, Node_id pred, const Node_id obj)
   : Triple(subj, pred, obj) {
      //triple's type tag should be either same as predicate,
      //same as object if predicate is rdf:type, or generic
      assert(
            ( tag_t::index == pred() ) ||
            ( tag_t::index == obj() && ot::T_rdf_type::index == pred() ) ||
            ( tag_t::index == ot::T__generic::index )
      );
   }
   LOKI_DEFINE_CONST_VISITABLE()
};


}//namespace owl_cpp

#endif /* TRIPLE_HPP_ */
