/** @file "/owlcpp/include/owlcpp/rdf/doc_id.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef DOC_ID_HPP_
#define DOC_ID_HPP_
#include "owlcpp/detail/comparable_value.hpp"

namespace owlcpp{

/**@brief Document identifier
*******************************************************************************/
class Doc_id : public detail::unsigned_val_t {
public:
   explicit Doc_id(const unsigned x) : detail::unsigned_val_t(x) {}
   Doc_id() : detail::unsigned_val_t(0) {}
};


}//namespace owlcpp
#endif /* DOC_ID_HPP_ */
