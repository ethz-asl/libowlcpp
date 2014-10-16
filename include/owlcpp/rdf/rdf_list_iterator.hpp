/** @file "/owlcpp/include/owlcpp/rdf/rdf_list_iterator.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef RDF_LIST_ITERATOR_HPP_
#define RDF_LIST_ITERATOR_HPP_
#include "boost/iterator/iterator_facade.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"
#include "owlcpp/rdf/print_node.hpp"

namespace owlcpp{

/**@brief Iterates over members of RDF list
@details Triple store provides additional information for troubleshooting.
*******************************************************************************/
class Rdf_list_iter_s
         : public boost::iterator_facade<
              Rdf_list_iter_s,
              Node_id,
              boost::forward_traversal_tag,
              Node_id
           >
{
public:
   struct Err : public Rdf_err {};

   Rdf_list_iter_s()
   : ts_(0), nid_(terms::rdf_nil::id())
   {}

   Rdf_list_iter_s(const Node_id nid, Triple_store const& ts)
   : ts_(&ts), nid_(nid)
   {}

private:
   Triple_store const* ts_;
   Node_id nid_;

   friend class boost::iterator_core_access;

   void increment() {
      const Triple_store::query_b<1,1,0,0>::range r = ts_->find_triple(
               nid_,
               terms::rdf_rest::id(),
               any,
               any
      );
      if( ! r ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("rdf:rest triple not found")
               << Err::str1_t(to_string(nid_, *ts_))
      );
      const Triple t = r.front();
      if( boost::distance(r) > 1 ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("multiple rdf:rest triples")
               << Err::str1_t(to_string(nid_, *ts_))
      );
      nid_ = t.obj_;
   }

   Node_id dereference() const {
      const Triple_store::query_b<1,1,0,0>::range r = ts_->find_triple(
               nid_,
               terms::rdf_first::id(),
               any,
               any
      );
      if( ! r ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("rdf:first triple not found")
               << Err::str1_t(to_string(nid_, *ts_))
      );
      const Triple t = r.front();
      if( boost::distance(r) > 1 ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("multiple rdf:first triples")
               << Err::str1_t(to_string(nid_, *ts_))
      );
      return t.obj_;
   }

   bool equal(Rdf_list_iter_s const& i) const {return nid_ == i.nid_;}
};

}//namespace owlcpp
#endif /* RDF_LIST_ITERATOR_HPP_ */
