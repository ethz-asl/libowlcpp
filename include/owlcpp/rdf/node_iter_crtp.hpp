/** @file "/owlcpp/include/owlcpp/rdf/node_iter_crtp.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_ITER_CRTP_HPP_
#define NODE_ITER_CRTP_HPP_
#include "owlcpp/rdf/node_map_base.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
template<class Super> struct Node_iter_crtp {
   typedef Node_map_base::const_iterator const_iterator;
   typedef Node_map_base::iterator iterator;
   typedef Node_map_base::range range;
   typedef Node_map_base::node_iterator node_iterator;
   typedef Node_map_base::node_range node_range;
   typedef Node_map_base::ns_iterator ns_iterator;
   typedef Node_map_base::ns_range ns_range;

   std::size_t size() const {return static_cast<Super const&>(*this).base_map().size();}
   const_iterator begin() const {return static_cast<Super const&>(*this).base_map().begin();}
   const_iterator end() const {return static_cast<Super const&>(*this).base_map().end();}
};


}//namespace owlcpp
#endif /* NODE_ITER_CRTP_HPP_ */
