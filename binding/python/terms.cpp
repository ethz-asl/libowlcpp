/** @file "/owlcpp/binding/python/terms.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#include "boost/python.hpp"
namespace bp = boost::python;
#include "boost/mpl/for_each.hpp"
namespace bmp = boost::mpl;
#include <map>

#include "rdf/iri_tag_vector.hpp"
#include "rdf/node_tag_vector_system.hpp"
#include "rdf/node_tag_vector_owl.hpp"
namespace t = owlcpp::terms;

template<class T> struct Ns_tag_name {
   static std::string get() {return T::prefix();}
};
template<> struct Ns_tag_name<t::empty> {
   static std::string get() {return "empty";}
};
template<> struct Ns_tag_name<t::blank> {
   static std::string get() {return "blank";}
};

typedef std::map<owlcpp::Ns_id, bp::object> map_t;

class Ns_tag_inserter {
public:
   explicit Ns_tag_inserter(map_t& map) : map_(&map){}
   template<class Ns> static std::string iri() {return Ns::iri();}
   template<class Ns> static std::string pref() {return Ns::prefix();}

   template<class T> void operator()(T const& t) const {
      (*map_)[T::id()] =
      bp::class_<T>(Ns_tag_name<T>::get().c_str())
         .add_static_property("id", &T::id)
         .add_static_property("iri", &iri<T>)
         .add_static_property("prefix", &pref<T>)
         ;
   }

private:
   mutable map_t* map_;
};

template<class T> struct Term_tag_name {
   static std::string get() {
      return
               Ns_tag_name<typename T::ns_type>::get() +
               "_" +
               T::fragment()
      ;
   }
};

class Term_tag_inserter {
public:
   Term_tag_inserter(map_t const& map) : map_(map) {}
   template<class T> static char const* fragment() {return T::fragment().c_str();}
   template<class T> void operator()(T const& t) const {
      bp::class_<T>(Term_tag_name<T>::get().c_str())
         .add_static_property("id", &T::id)
         .add_static_property("fragment", &fragment<T>)
         .add_static_property("ns_type", map_.find(T::ns_type::id())->second)
         ;
   }
private:
   map_t const& map_;
};

BOOST_PYTHON_MODULE(terms) {
   map_t map;
   Ns_tag_inserter nti(map);
   bmp::for_each<t::mpl_vector_iris_t>(nti);

   Term_tag_inserter tti(map);
   bmp::for_each<t::mpl_vector_terms_system_t>(tti);
   bmp::for_each<t::mpl_vector_terms_rdfs_t>(tti);
   bmp::for_each<t::mpl_vector_terms_rdf_t>(tti);
   bmp::for_each<t::mpl_vector_terms_xsd_t>(tti);
   bmp::for_each<t::mpl_vector_terms_owl1_t>(tti);
   bmp::for_each<t::mpl_vector_terms_owl2_t>(tti);

}
