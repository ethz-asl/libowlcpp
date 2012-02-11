/** @file "/owlcpp/lib/io/triple_store_temp.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_STORE_TEMP_HPP_
#define TRIPLE_STORE_TEMP_HPP_
#include "owlcpp/rdf/node_store_iri_base.hpp"
#include "owlcpp/rdf/node_store_aux_base.hpp"
#include "owlcpp/rdf/doc_store_base.hpp"
#include "owlcpp/rdf/triple_map.hpp"
#include "owlcpp/rdf/node_map.hpp"
#include "owlcpp/rdf/iri_map.hpp"
#include "owlcpp/rdf/doc_info_map.hpp"

namespace owlcpp{ namespace detail{

/**@brief 
*******************************************************************************/
class Triple_store_temp :
public Node_store_iri_base<Triple_store_temp>,
public Node_store_aux_base<Triple_store_temp>,
public Doc_store_base<Triple_store_temp>
{
public:
   Iri_map& iris() {return iri_;}
   Iri_map const& iris() const {return iri_;}
   Node_map& nodes() {return node_;}
   Node_map const& nodes() const {return node_;}
   Doc_map const& documents() const {return doc_;}
   Doc_map& documents() {return doc_;}
   Triple_map const& triples() const {return triple_;}

private:

};

}//namespace detail
}//namespace owlcpp
#endif /* TRIPLE_STORE_TEMP_HPP_ */
