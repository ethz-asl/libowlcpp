/** @file "/owlcpp/include/owlcpp/rdf/rdf_list_iterator.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef RDF_LIST_ITERATOR_HPP_
#define RDF_LIST_ITERATOR_HPP_
#include "boost/iterator/iterator_facade.hpp"
#include "owlcpp/rdf/triple_map.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{

/**@brief Iterates over members of RDF list
@details Example of a list:
@code
_:c1 rdf:first <ex:aaa>
_:c1 rdf:rest _:c2
_:c2 rdf:first <ex:bbb>
_:c2 rdf:rest rdf:nil
@endcode
Non well-formed list produces undefined behavior.
*******************************************************************************/
class Rdf_list_iterator
: public boost::iterator_facade<
  Rdf_list_iterator,
  Node_id const,
  boost::forward_traversal_tag
  >
{
public:
   Rdf_list_iterator()
   : tm_(0), nid_(terms::T_rdf_nil::id())
   {}

   Rdf_list_iterator(const Node_id nid, Triple_map const& tm)
   : tm_(&tm), nid_(nid)
   {}

private:
   Triple_map const* tm_;
   Node_id nid_;

   friend class boost::iterator_core_access;

   void increment() {
      const Query<1,1,0,0>::range r = tm_->find(
               nid_,
               terms::T_rdf_rest::id(),
               blank(),
               blank()
      );
      BOOST_ASSERT( r );
      nid_ = r.front().object();
      BOOST_ASSERT( distance(r) == 1U );
   }

   Node_id const& dereference() const {
      const Query<1,1,0,0>::range r = tm_->find(
               nid_,
               terms::T_rdf_first::id(),
               blank(),
               blank()
      );
      BOOST_ASSERT( r );
      Node_id const& n = r.front().obj_;
      BOOST_ASSERT( distance(r) == 1U );
      return n;
   }

   bool equal(Rdf_list_iterator const& i) const {return nid_ == i.nid_;}
};

}//namespace owlcpp
#endif /* RDF_LIST_ITERATOR_HPP_ */
