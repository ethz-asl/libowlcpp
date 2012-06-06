/** @file "/owlcpp/include/owlcpp/rdf/detail/triple_index.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_INDEX_HPP_
#define TRIPLE_INDEX_HPP_
#include <vector>
#include "boost/assert.hpp"
#include "boost/iterator/indirect_iterator.hpp"
#include "boost/range.hpp"

#include "owlcpp/rdf/triple.hpp"
#include "owlcpp/rdf/detail/triple_query_tags.hpp"

namespace owlcpp{ namespace query_detail{
namespace mpl = boost::mpl;

struct Vector_index {
   typedef std::vector<Triple const*> index;
   typedef boost::indirect_iterator<index::const_iterator> iterator;
   typedef boost::iterator_range<iterator> range;
};

template<class> class Index;

template<> struct Index<Subj_tag> : public Vector_index {
   static range get_range<
};


}//namespace query_detail
}//namespace owlcpp
#endif /* TRIPLE_INDEX_HPP_ */
