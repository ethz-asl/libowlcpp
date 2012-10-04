/** @file "/owlcpp/include/owlcpp/rdf/node_fwd.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef NODE_FWD_HPP_
#define NODE_FWD_HPP_
#include "boost/cstdint.hpp"

namespace owlcpp{ namespace detail{
template<class> class Node_literal;
}//namespace detail

class Node;
class Node_iri;
class Node_blank;
typedef detail::Node_literal<bool> Node_bool;
typedef detail::Node_literal<boost::intmax_t> Node_int;
typedef detail::Node_literal<boost::uintmax_t> Node_unsigned;
typedef detail::Node_literal<double> Node_double;
typedef detail::Node_literal<std::string> Node_string;

}//namespace owlcpp
#endif /* NODE_FWD_HPP_ */
