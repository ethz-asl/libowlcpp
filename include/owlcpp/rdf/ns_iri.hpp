/** @file "/owlcpp/include/owlcpp/rdf/ns_iri.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef NS_IRI_HPP_
#define NS_IRI_HPP_
#include <string>
#include <iosfwd>
#include "boost/functional/hash.hpp"

namespace owlcpp{

/**@brief Namespace IRI
*******************************************************************************/
class Ns_iri {
public:
   explicit Ns_iri(std::string const& str) : str_(str) {}

   Ns_iri& operator=(Ns_iri const& nsiri) {
      str_ = nsiri.str_;
      return *this;
   }

   Ns_iri& operator=(std::string const& nsiri) {
      str_ = nsiri;
      return *this;
   }

   std::string const& str() const {return str_;}

private:
   std::string str_;
};

template<class Ch, class Tr> inline std::basic_ostream<Ch,Tr>&
   operator<<(std::basic_ostream<Ch,Tr>& os, Ns_iri const& ns_iri)
{return os << ns_iri.str();}

inline bool operator==(Ns_iri const& n1, Ns_iri const& n2)
{return n1.str() == n2.str();}

inline std::size_t hash_value(Ns_iri const& n)
{return boost::hash_value<std::string>(n.str());}


}//namespace owlcpp
#endif /* NS_IRI_HPP_ */
