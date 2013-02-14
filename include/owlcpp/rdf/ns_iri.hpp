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
   explicit Ns_iri(char const* nsiri) : str_(nsiri) {}
   explicit Ns_iri(std::string const& nsiri) : str_(nsiri) {}
   Ns_iri(char const* nsiri, const std::size_t n) : str_(nsiri, n) {}

   bool operator==(Ns_iri const& n) const {return str_ == n.str_;}
   bool operator==(std::string const& str) const {return str_ == str;}

   std::string const& str() const {return str_;}

private:
   std::string str_;
};

template<class Ch, class Tr> inline std::basic_ostream<Ch,Tr>&
   operator<<(std::basic_ostream<Ch,Tr>& os, Ns_iri const& ns_iri)
{return os << ns_iri.str();}

inline std::size_t hash_value(Ns_iri const& n)
{return boost::hash_value(n.str());}

/**@brief remove fragment identifier from the rest of IRI string
 @param[in] iri IRI string
 @param[out] n position of the first character in fragment identifier
 or iri.size() if fragment not found
 @return Ns_iri without the fragment identifier
*/
inline Ns_iri remove_fragment(std::string const& iri, std::size_t& n) {
   n = iri.find('#');
   if( n == std::string::npos ) {
      n = iri.size();
      return Ns_iri(iri);
   }
   return Ns_iri(iri.substr(0,n++));
}

/**@brief remove fragment identifier from the rest of the IRI
 @param[in] iri IRI string
 @return Ns_iri without the fragment identifier
*/
inline Ns_iri remove_fragment(std::string const& iri) {
   std::size_t n;
   return remove_fragment(iri, n);
}

inline std::string add_fragment(Ns_iri const& nsiri, std::string const& frag) {
   return nsiri.str() + '#' + frag;
}


}//namespace owlcpp
#endif /* NS_IRI_HPP_ */
