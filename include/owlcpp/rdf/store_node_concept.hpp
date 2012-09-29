/** @file "/owlcpp/include/owlcpp/rdf/store_node_concept.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef STORE_NODE_CONCEPT_HPP_
#define STORE_NODE_CONCEPT_HPP_
#include "boost/concept/assert.hpp"
#include "owlcpp/rdf/detail/map_traits.hpp"

namespace owlcpp{

template<class T> class Store_node_concept {
public:
   typedef detail::Map_traits<T> traits;
   typedef typename traits::map_ns_type map_ns_type;
   typedef typename traits::map_node_type map_node_type;

   BOOST_CONCEPT_USAGE(Store_node_concept) {
      map_ns_type const& map_ns = t.map_ns();
      boost::ignore_unused_variable_warning(map_ns);
   }
private:
   T t;
};

}//namespace owlcpp
#endif /* STORE_NODE_CONCEPT_HPP_ */
