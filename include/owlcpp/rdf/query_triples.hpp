/** @file "/owlcpp/include/owlcpp/query_triples.hpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef QUERY_TRIPLES_HPP_
#define QUERY_TRIPLES_HPP_
#include "boost/tuple/tuple.hpp"
#include "boost/iterator/filter_iterator.hpp"
#include "boost/range.hpp"
namespace b = boost;

#include "owlcpp/rdf/config.hpp"
#include "owlcpp/rdf/triple_store.hpp"
#include "owlcpp/rdf/triple.hpp"
#include "owlcpp/exception.hpp"

namespace owlcpp{

struct blank{};

namespace detail{

/** Check for matching node ID
*******************************************************************************/
template<class> class Check;

template<> class Check<Node_id> {
public:
   Check(const Node_id id) : id_(id) {}
   bool operator()(const Node_id id) const {return id == id_;}
private:
   Node_id id_;
};

template<> struct Check<blank> {
   Check(const blank) {}
   bool operator()(const Node_id) const {return true;}
};

/**
*******************************************************************************/
template<class N1, class N2, class N3> class Triple_predicate {
public:
   typedef N1 type_1;
   typedef N2 type_2;
   typedef N3 type_3;
private:
   typedef b::tuple<Check<N1>,Check<N2>,Check<N3> > tuple_t;
public:
   Triple_predicate(const N1 n1, const N2 n2, const N3 n3)
   : t_(Check<N1>(n1),Check<N2>(n2),Check<N3>(n3)) {}

   bool operator()(const Triple& triple) const {
      return
            t_.template get<0>()(triple.get<0>()) &&
            t_.template get<1>()(triple.get<1>()) &&
            t_.template get<2>()(triple.get<2>());
   }
private:
   tuple_t t_;
};


/**specialize class to retrieve a range for a specific subject ID
*******************************************************************************/
template<> struct Range_retriever<Node_id> {
   typedef Triple_store::triple_range_t triple_range_t;
   static Triple_store::triple_range_t get(const Node_id nid,const Triple_store& store)
   {return store.subject_in(nid);}
};

/**specialize class to retrieve a range all triples
*******************************************************************************/
template<> struct Range_retriever<blank> {
   typedef Triple_store::triple_range_t triple_range_t;
   static Triple_store::triple_range_t get(const blank,const Triple_store& store)
   {return store.triples();}
};

}//namespace detail

/**@brief Retrieve single-pass range of queried triples from triple store
*******************************************************************************/
template<class N1, class N2, class N3> class Triples_query {
   typedef detail::Range_retriever<N1> range_getter_t;
   typedef typename range_getter_t::triple_range_t range_1_t;
   typedef typename range_1_t::iterator iter_1_t;
   typedef detail::Triple_predicate<N1,N2,N3> predicate_t;
   typedef b::filter_iterator<predicate_t, iter_1_t> iter_2_t;
public:
   typedef b::iterator_range<iter_2_t> q_range_t;
   static q_range_t make(const N1 nid1, const N2 nid2, const N3 nid3, const Triple_store& store) {
      const predicate_t pred(nid1, nid2, nid3);
      range_1_t range = range_getter_t::get(nid1, store);
      iter_2_t i1(pred, b::begin(range), b::end(range));
      iter_2_t i2(pred, b::end(range),   b::end(range));
      return q_range_t(i1, i2);
   }
};

/**@return single-pass range of queried triples
*******************************************************************************/
template<class N1, class N2, class N3>
inline typename Triples_query<N1,N2,N3>::q_range_t
find_triples(const N1 nid1, const N2 nid2, const N3 nid3, const Triple_store& store) {
   return Triples_query<N1,N2,N3>::make(nid1,nid2,nid3,store);
}

/**@return the first triple found or throw
*******************************************************************************/
template<class N1, class N2, class N3> inline const Triple&
find_first_triple(const N1 nid1, const N2 nid2, const N3 nid3, const Triple_store& store) {
   typename Triples_query<N1,N2,N3>::q_range_t range =
         Triples_query<N1,N2,N3>::make(nid1,nid2,nid3,store);
   if( ! range ) {
      BOOST_THROW_EXCEPTION(
            base_exception()
            << base_exception::msg_t("no matches")
      //TODO: print query
      );
   }
   return range.front();
}

/** Find members of RDF collection
@param node node ID pointing to RDF collection
@param store ontology storage
@return empty vector if node is not a collection
*******************************************************************************/
OWLCPP_RDF_DECL std::vector<Node_id> rdf_collection(const Node_id node, const Triple_store& store);

/** @return object of triple {x rdfs:label "some label"} or "" if none exists
*******************************************************************************/
inline std::string find_label(const Node_id id, const Triple_store& store) {
   Triples_query<Node_id,Node_id,blank>::q_range_t range =
         find_triples(id, ot::T_rdfs_label::id(), blank(), store);
   if( ! range ) return "";
   return store[range.front().object()].value_str();
}

/**@return true if iri ontology IRI or version IRI is in triple store
*******************************************************************************/
OWLCPP_RDF_DECL bool have_ontology_id(const std::string& iri, const Triple_store& store);

/**@return number of loaded ontologies
*******************************************************************************/
OWLCPP_RDF_DECL std::size_t num_ontologies(const Triple_store& store);

}//namespace owlcpp
#endif /* QUERY_TRIPLES_HPP_ */
