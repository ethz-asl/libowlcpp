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
#include "owlcpp/doc_id.hpp"
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
               const Doc_id id,
               std::string const& path,
               const Node_id iri_id,
               const Node_id version_id
      ) : id_(id), path_(path), iri_id_(iri_id), version_id_(version_id)
      {}
      Doc_id id_;
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
                     entry_t, Doc_id, &entry_t::id_
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
   typedef id_index_t::const_iterator id_iter_t;

   typedef store_t::index<path_tag>::type path_index_t;

   typedef store_t::index<iri_tag>::type iri_index_t;

   typedef store_t::index<version_tag>::type version_index_t;

public:
   struct Err : public base_exception {};

   typedef Member_iterator<path_index_t::const_iterator, const Doc_id, &entry_t::id_> path_iterator;
   typedef boost::iterator_range<path_iterator> path_range;

   typedef Member_iterator<iri_index_t::const_iterator, const Doc_id, &entry_t::id_> iri_iterator;
   typedef boost::iterator_range<iri_iterator> iri_range;

   typedef Member_iterator<version_index_t::const_iterator, const Doc_id, &entry_t::id_> version_iterator;
   typedef boost::iterator_range<version_iterator> version_range;

   typedef Member_iterator<store_t::iterator, const Doc_id, &entry_t::id_> id_iterator;

//   Doc_map() : tracker_(), store_() {
//         tracker_.reserve(Doc_id());
//      }

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
   ) {
      Doc_id const* did = find_existing(path, iri, version);
      if( did ) return std::make_pair(*did, false);
      return std::make_pair(insert_private(path, iri, version), true);
   }

   void modify(
            const Doc_id did,
            std::string const& path,
            const Node_id iri,
            const Node_id version
   ) {
      id_index_t& index = store_.get<id_tag>();
      id_iter_t i = index.find(did);
      BOOST_ASSERT(i != index.end());
      const entry_t entry = *i;
      index.erase(i);
      try {
         if( find_existing(path, iri, version) ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("document already exists")
                  << Err::str1_t(path)
                  << Err::int1_t(iri())
                  << Err::int2_t(version())
         );
      } catch(Err const& e) {
         store_.insert(entry);
         throw;
      }
      store_.insert(entry_t(did, path, iri, version));
   }

   std::pair<Doc_id,bool> insert(std::string const& path, const Node_id iri);
   Doc_id insert_new();
   Node_id iri(const Doc_id id) const;
   Node_id version(const Doc_id id) const;
   std::string path(const Doc_id id) const;
   void remove(const Doc_id id);

   /**
    @param id node ID of document's OntologyIRI
    @return
   */
   iri_range find_iri(const Node_id id) const {
      std::pair<iri_index_t::const_iterator,iri_index_t::const_iterator> p =
               store_.get<iri_tag>().equal_range(id);
      return boost::make_iterator_range(
               iri_iterator(p.first),
               iri_iterator(p.second)
      );
   }

   version_range find_version(const Node_id id) const {
      std::pair<version_index_t::const_iterator,version_index_t::const_iterator> p =
               store_.get<version_tag>().equal_range(id);
      return boost::make_iterator_range(
               version_iterator(p.first),
               version_iterator(p.second)
      );
   }

   path_range find_path(std::string const& path) const {
      std::pair<path_index_t::const_iterator,path_index_t::const_iterator> p =
               store_.get<path_tag>().equal_range(path);
      return boost::make_iterator_range(
               path_iterator(p.first),
               path_iterator(p.second)
      );
   }

   id_iterator begin() const {return id_iterator(store_.begin());}
   id_iterator end() const {return id_iterator(store_.end());}

private:
   detail::Id_tracker<Doc_id> tracker_;
   store_t store_;

   Doc_id const* find_existing(
            std::string const& path,
            const Node_id iri,
            const Node_id version
   ) const;

   Doc_id insert_private(
            std::string const& path,
            const Node_id iri,
            const Node_id version
   ) {
      const Doc_id id = tracker_.get();
      BOOST_ASSERT(store_.get<id_tag>().find(id) == store_.get<id_tag>().end());
      store_.insert(entry_t(id, path, iri, version));
      return id;
   }

};

}//namespace owlcpp
#endif /* DOC_INFO_MAP_HPP_ */
