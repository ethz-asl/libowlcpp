/** @file "/owlcpp/include/owlcpp/rdf/detail/iri_tag_inserter.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef IRI_TAG_INSERTER_HPP_
#define IRI_TAG_INSERTER_HPP_

#include "owlcpp/rdf/iri_store.hpp"

namespace owlcpp{ namespace detail{

/**@brief Insert standard IRI tags into IRI store
*******************************************************************************/
class Iri_tag_inserter {
   Iri_tag_inserter();
public:
   Iri_tag_inserter(Iri_store& store) : store_(&store) {}

   template<class T> void operator()(const T&) const {
      store_->insert(T::id(), T::iri(), T::prefix());
   }
private:
   mutable Iri_store* store_;
};

}//namespace detail

}//namespace owlcpp
#endif /* IRI_TAG_INSERTER_HPP_ */
