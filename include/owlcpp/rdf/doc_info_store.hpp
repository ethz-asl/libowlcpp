/** @file "/owlcpp/include/owlcpp/rdf/doc_info_store.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef DOC_INFO_STORE_HPP_
#define DOC_INFO_STORE_HPP_
#include <string>
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/range.hpp"

#include "owlcpp/rdf/config.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/rdf/doc_id.hpp"
#include "owlcpp/detail/id_tracker.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class OWLCPP_RDF_DECL Doc_store {
public:
   typedef Doc_id id_type;
private:
   struct entry_t {
      entry_t(
               const id_type id,
               std::string const& path,
               const Node_id iri_id,
               const Node_id version_id
      ) : id_(id), path_(path), iri_id_(iri_id), version_id_(version_id)
      {}
      id_type id_;
      std::string path_;
      Node_id iri_id_;
      Node_id version_id_;
   };

   typedef boost::multi_index_container<
            entry_t,
            boost::multi_index::indexed_by<
               boost::multi_index::hashed_unique<
                  boost::multi_index::tag<struct id_tag>,
                  boost::multi_index::member<
                     entry_t, id_type, &entry_t::id_
                  >
               >,
               boost::multi_index::hashed_unique<
                  boost::multi_index::tag<struct path_tag>,
                  boost::multi_index::member<
                     entry_t, std::string, &entry_t::path_
                  >
               >,
               boost::multi_index::hashed_non_unique<
                  boost::multi_index::tag<struct iri_tag>,
                  boost::multi_index::member<
                     entry_t, Node_id, &entry_t::iri_id_
                  >
               >,
               boost::multi_index::hashed_non_unique<
                  boost::multi_index::tag<struct version_tag>,
                  boost::multi_index::member<
                     entry_t, Node_id, &entry_t::version_id_
                  >
               >
            >
         > store_t;

   typedef store_t::index<id_tag>::type id_index_t;
   typedef id_index_t::iterator id_iter_t;

   typedef store_t::index<path_tag>::type path_index_t;
   typedef path_index_t::iterator path_iter_t;

   typedef store_t::index<iri_tag>::type iri_index_t;
   typedef iri_index_t::iterator iri_iter_t;

   typedef store_t::index<version_tag>::type version_index_t;
   typedef version_index_t::iterator version_iter_t;



public:
   struct Err : public base_exception {};
   typedef boost::iterator_range<iri_iter_t> iri_range_t;
   typedef boost::iterator_range<version_iter_t> version_range_t;

   Doc_store() : tracker_(), store_() {}

   id_type insert(std::string const& path, const Node_id iri, const Node_id ver);
   id_type insert(std::string const& path, const Node_id iri);
   Node_id iri(const id_type id) const;
   Node_id version(const id_type id) const;

   /**
    @param nid node ID
    @return pointer to document ID for the first document that has VersionIRI nid or,
    if not found, for the first document with OntologyIRI nid, or NULL if not found.
   */
   id_type const* find(const Node_id id) const;

private:
   detail::Id_tracker<id_type> tracker_;
   store_t store_;
};

}//namespace owlcpp
#endif /* DOC_INFO_STORE_HPP_ */
