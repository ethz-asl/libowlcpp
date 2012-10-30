/** @file "/owlcpp/lib/rdf/print_id.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_RDF_SOURCE
#define OWLCPP_RDF_SOURCE
#endif
#include "owlcpp/rdf/print_id.hpp"

#include "boost/lexical_cast.hpp"

#include "owlcpp/ns_id.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/doc_id.hpp"

namespace owlcpp {

/*
*******************************************************************************/
std::string to_string(const Ns_id nsid) {
   return "Ns" + boost::lexical_cast<std::string>(nsid());
}

/*
*******************************************************************************/
std::string to_string(const Node_id nid) {
   return "Node" + boost::lexical_cast<std::string>(nid());
}

/*
*******************************************************************************/
std::string to_string(const Doc_id did) {
   return "Doc" + boost::lexical_cast<std::string>(did());
}

}//namespace owlcpp
