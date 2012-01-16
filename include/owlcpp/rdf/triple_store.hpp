/** @file "/owlcpp/include/owlcpp/rdf/triple_store.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef TRIPLE_STORE_HPP_
#define TRIPLE_STORE_HPP_

#include "owlcpp/rdf/triple_map.hpp"
#include "owlcpp/rdf/node_store_owl.hpp"
#include "owlcpp/rdf/iri_store_owl.hpp"
#include "owlcpp/rdf/doc_info_store.hpp"
#include "owlcpp/rdf/config.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class OWLCPP_RDF_DECL Triple_store {
public:

   Iri_store_owl const& iris() const {return iri_;}
   Node_store_owl const& nodes() const {return node_;}
   Doc_store const& documents() const {return doc_;}
   Triple_map const& triples() const {return triple_;}

   Node_id insert_reference(std::string const&);
   Node_id insert_literal(std::string const&);
   Node_id insert_blank(std::string const&);
   Doc_id insert_doc(std::string const& path, std::string const& iri, std::string const& version);
   void insert_triple(const Node_id subj, const Node_id pred, const Node_id obj, const Doc_id doc);

private:
   Iri_store_owl iri_;
   Node_store_owl node_;
   Doc_store doc_;
   Triple_map triple_;
};

}//namespace owlcpp
#endif /* TRIPLE_STORE_HPP_ */
