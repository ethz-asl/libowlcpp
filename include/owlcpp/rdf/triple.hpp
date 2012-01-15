/** @file "/owlcpp/include/owlcpp/rdf/triple.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_HPP_
#define TRIPLE_HPP_

#include "owlcpp/rdf/doc_id.hpp"
#include "owlcpp/node_id.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
struct Triple {
   Node_id subj_, pred_, obj_;
   Doc_id doc_;

   Triple(
            const Node_id subj, const Node_id pred,
            const Node_id obj, const Doc_id doc
   ) : subj_(subj), pred_(pred), obj_(obj), doc_(doc)
   {}

   Node_id subject() const {return subj_;}
   Node_id predicate() const {return pred_;}
   Node_id object() const {return obj_;}
   Doc_id document() const {return doc_;}

};

}//namespace owlcpp
#endif /* TRIPLE_HPP_ */
