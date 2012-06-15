/** @file "/owlcpp/include/owlcpp/doc_id.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef DOC_ID_HPP_
#define DOC_ID_HPP_
#include "owlcpp/detail/object_id_base.hpp"

namespace owlcpp{

/**@brief Document identifier
*******************************************************************************/
class Doc_id : public detail::Base_id<Doc_id, unsigned> {
public:
   explicit Doc_id(const unsigned x) : base(x) {}
   Doc_id() : base(0) {}
};


}//namespace owlcpp
#endif /* DOC_ID_HPP_ */
