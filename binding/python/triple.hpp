/** @file "/owlcpp/binding/python/triple.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef TRIPLE_PY_HPP_
#define TRIPLE_PY_HPP_
#include "owlcpp/rdf/triple.hpp"

namespace owlcpp{ namespace py{

/**@brief 
*******************************************************************************/
struct Triple : public owlcpp::Triple {
   Triple(
            const owlcpp::Node_id subj, const owlcpp::Node_id pred,
            const owlcpp::Node_id obj, const owlcpp::Doc_id doc
   ){
      subj_ = subj;
      pred_ = pred;
      obj_ = obj;
      doc_ = doc;
   }
};

}//namespace py
}//namespace owlcpp
#endif /* TRIPLE_PY_HPP_ */
