/** @file "/owlcpp/include/owlcpp/rdf/ns_iri.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef NS_IRI_HPP_
#define NS_IRI_HPP_
#include <string>

namespace owlcpp{

/**@brief Namespace IRI
*******************************************************************************/
class Ns_iri {
public:
   explicit Ns_iri(std::string const& str) : str_(str) {}

   std::string const& str() const {return str_;}
private:
   std::string str_;
};

}//namespace owlcpp
#endif /* NS_IRI_HPP_ */
