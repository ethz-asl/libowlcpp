/** @file "/owlcpp/include/owlcpp/rdf/triple.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_HPP_
#define TRIPLE_HPP_
#include <iosfwd>
#include "owlcpp/doc_id.hpp"
#include "owlcpp/node_id.hpp"

namespace owlcpp{

/**@brief RDF triple plus document ID
*******************************************************************************/
struct Triple {
   static Triple make(
            const Node_id subj, const Node_id pred,
            const Node_id obj, const Doc_id doc
   ) {
      Triple t;
      t.subj_ = subj;
      t.pred_ = pred;
      t.obj_ = obj;
      t.doc_ = doc;
      return t;
   }

   Node_id subj_, pred_, obj_;
   Doc_id doc_;

   bool operator==(Triple const& t) const {
      return subj_==t.subj_ && pred_==t.pred_ && obj_==t.obj_ && doc_==t.doc_;
   }

   bool operator<(Triple const& t) const {
      if( subj_ < t.subj_ ) return true;
      if( t.subj_ < subj_ ) return false;
      if( pred_ < t.pred_ ) return true;
      if( t.pred_ < pred_ ) return false;
      if( obj_ < t.obj_ ) return true;
      if( t.obj_ < obj_ ) return false;
      return doc_ < t.doc_;
   }

   bool operator!=(Triple const& t) const {return !(*this == t);}
};

/**
*******************************************************************************/
template<class ChT, class Tr> inline std::basic_ostream<ChT,Tr>& operator<<(
      std::basic_ostream<ChT,Tr>& os,
      Triple const& t
) {
   return os << t.subj_ << ',' << t.pred_ << ',' << t.obj_ << ',' << t.doc_;
}

}//namespace owlcpp
#endif /* TRIPLE_HPP_ */
