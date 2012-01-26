/** @file "/owlcpp/include/owlcpp/rdf/doc_info_map.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef DOC_INFO_MAP_HPP_
#define DOC_INFO_MAP_HPP_
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
#include "owlcpp/detail/member_iterator.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class OWLCPP_RDF_DECL Doc_map {
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
               boost::multi_index::hashed_non_unique<
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

   typedef Member_iterator<path_iter_t, const id_type, &entry_t::id_> path_iterator;
   typedef boost::iterator_range<path_iterator> path_range;

   typedef Member_iterator<iri_iter_t, const id_type, &entry_t::id_> iri_iterator;
   typedef boost::iterator_range<iri_iterator> iri_range;

   typedef Member_iterator<version_iter_t, const id_type, &entry_t::id_> version_iterator;
   typedef boost::iterator_range<version_iterator> version_range;

   typedef Member_iterator<store_t::iterator, const id_type, &entry_t::id_> id_iterator;

   Doc_map();

   std::size_t size() const {return store_.size();}

   /**@brief Add document info: location, ontologyIRI, and versionIRI.
    @param path document location
    @param iri ontologyIRI
    @param version versionIRI
    @return document ID and whether new document info was actually added
    @throw Err if an entry with the same non-empty @a path and different @a iri or @a version
    is already present.
    @details
    Duplicate document info entries are not allowed.

    If document info with same @a path, @a iri, and @a version already present,
    new info is not inserted and the ID of the existing document is returned.

    Since documents with same ontologyIRI and versionIRI may be found
    at different paths,
    multiple entries with different @a path and same @a iri or @a version can be added.

    Sometimes, document path is not known.
    Therefore multiple entries with empty @a path and different @a iri or @a version
    are allowed.
   */
   std::pair<Doc_id,bool> insert(
            std::string const& path,
            const Node_id iri,
            const Node_id version
   );

   std::pair<Doc_id,bool> insert(std::string const& path, const Node_id iri);
   Node_id iri(const id_type id) const;
   Node_id const* version(const id_type id) const;
   std::string path(const id_type id) const;

   /**
    @param id node ID of document's OntologyIRI
    @return
   */
   iri_range find_iri(const Node_id id) const {
      iri_index_t const& iri_ind = store_.get<iri_tag>();
      return boost::make_iterator_range(
               iri_iterator(iri_ind.find(id)),
               iri_iterator(iri_ind.end())
      );
   }

   version_range find_version(const Node_id id) const {
      version_index_t const& v_ind = store_.get<version_tag>();
      return boost::make_iterator_range(
               version_iterator(v_ind.find(id)),
               version_iterator(v_ind.end())
      );
   }

   version_range find_path(std::string const& path) const {
      path_index_t const& pi = store_.get<path_tag>();
      return boost::make_iterator_range(
               path_iterator(pi.find(id)),
               path_iterator(pi.end())
      );
   }

   id_iterator begin() const {return id_iterator(store_.begin());}
   id_iterator end() const {return id_iterator(store_.end());}

private:
   detail::Id_tracker<id_type> tracker_;
   store_t store_;
};

}//namespace owlcpp
#endif /* DOC_INFO_MAP_HPP_ */
