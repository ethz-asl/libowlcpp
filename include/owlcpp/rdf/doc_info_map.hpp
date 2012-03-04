/** @file "/owlcpp/include/owlcpp/rdf/doc_info_map.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef DOC_INFO_MAP_HPP_
#define DOC_INFO_MAP_HPP_
#include <string>
#include "boost/assert.hpp"
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/range.hpp"
#include "boost/foreach.hpp"

#include "owlcpp/node_id.hpp"
#include "owlcpp/exception.hpp"
#include "owlcpp/doc_id.hpp"
#include "owlcpp/detail/id_tracker.hpp"
#include "owlcpp/detail/member_iterator.hpp"
#include "owlcpp/terms/node_tags_system.hpp"

namespace owlcpp{

/**@brief 
*******************************************************************************/
class Doc_map {
public:
   typedef Doc_id id_type;
private:
   struct entry_t {
      entry_t(
               const Doc_id id,
               const Node_id iri_id,
               const Node_id version_id,
               std::string const& path
      ) : id_(id), iri_id_(iri_id), version_id_(version_id), path_(path)
      {}
      Doc_id id_;
      Node_id iri_id_;
      Node_id version_id_;
      std::string path_;
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

   typedef Member_iterator<store_t::const_iterator, const Doc_id, &entry_t::id_> const_iterator;
   typedef const_iterator iterator;

   std::size_t size() const {return store_.size();}
   const_iterator begin() const {return store_.begin();}
   const_iterator end() const {return store_.end();}

   /**@brief Add document info: location, ontologyIRI, and versionIRI.
    @param path document location
    @param iri ontologyIRI
    @param vers versionIRI
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
            const Node_id vers
   ) {
      Doc_id const* did = check_entry(path, iri, vers);
      if( did ) return std::make_pair(*did, false);
      return std::make_pair(insert_private(path, iri, vers), true);
   }

   /**
    @param id document ID; invalid @b id results in undefined behavior
    @return node ID for ontology IRI
   */
   Node_id ontology_iri(const Doc_id id) const {
      BOOST_ASSERT(store_.get<id_tag>().find(id) != store_.get<id_tag>().end());
      return store_.get<id_tag>().find(id)->iri_id_;
   }

   /**
    @param id document ID
    @return node ID for document version or empty node ID if version is not defined
   */
   Node_id version_iri(const Doc_id id) const {
      BOOST_ASSERT(store_.get<id_tag>().find(id) != store_.get<id_tag>().end());
      return store_.get<id_tag>().find(id)->version_id_;
   }

   /**
    @param id document ID
    @return document's path or empty string if not defined
   */
   std::string path(const Doc_id id) const {
      BOOST_ASSERT(store_.get<id_tag>().find(id) != store_.get<id_tag>().end());
      return store_.get<id_tag>().find(id)->path_;
   }

   void remove(const Doc_id id) {
      BOOST_ASSERT(store_.get<id_tag>().find(id) != store_.get<id_tag>().end());
      store_.get<id_tag>().erase(id);
      tracker_.push(id);
   }

   /**
    @param iri_id node ID of document's ontologyIRI
    @return iterator range for documents with @b id ontologyIRIs
   */
   iri_range find_iri(const Node_id iri_id) const {
      return store_.get<iri_tag>().equal_range(iri_id);
   }

   /**
    @param version_id node ID of document's versionIRI
    @return iterator range for documents with @b id versionIRIs
   */
   version_range find_version(const Node_id version_id) const {
      return store_.get<version_tag>().equal_range(version_id);
   }

   path_range find_path(std::string const& path) const {
      return store_.get<path_tag>().equal_range(path);
   }


private:
   detail::Id_tracker<Doc_id> tracker_;
   store_t store_;

   Doc_id const* check_entry(
            std::string const& p,
            const Node_id iri,
            const Node_id vers
   ) const {
      if( iri == terms::T_empty_::id() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("empty ontologyIRI is not allowed")
      );
      if( iri == vers ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("ontologyIRI and versionIRI are same")
      );

      //multiple entries with empty paths are allowed
      if( p.empty() ) return check_entry(iri, vers);

      const boost::iterator_range<path_index_t::const_iterator> r =
               store_.get<path_tag>().equal_range(p);
      if( ! r ) return 0;
      BOOST_ASSERT(distance(r) < 2);
      if( r.front().iri_id_ != iri ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("different IRI at same location")
               << Err::str1_t(p)
               << Err::int1_t(iri())
               << Err::int2_t(r.front().iri_id_())
      );
      if( r.front().version_id_ != vers ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("different ontology version in same location")
               << Err::str1_t(p)
               << Err::int1_t(vers())
               << Err::int2_t(r.front().version_id_())
      );

      return &r.front().id_;
   }

   Doc_id const* check_entry(
            const Node_id iri,
            const Node_id vers
   ) const {
      if( vers == terms::T_empty_::id() ) {
         BOOST_FOREACH(entry_t const& e, store_.get<iri_tag>().equal_range(iri)) {
            if( e.path_.empty() && e.version_id_ == vers ) return &e.id_;
         }
         return 0;
      }

      BOOST_FOREACH(entry_t const& e, store_.get<version_tag>().equal_range(vers)) {
         if( e.path_.empty() ) {
            if( e.iri_id_ == iri ) return &e.id_;
            BOOST_THROW_EXCEPTION(
                     Err()
                     << Err::msg_t("existing document has same versionIRI but different ontologyIRI")
                     << Err::int1_t(e.iri_id_())
            );
         }
      }
      return 0;
   }

   Doc_id insert_private(
            std::string const& p,
            const Node_id iri,
            const Node_id vers
   ) {
      const Doc_id id = tracker_.get();
      BOOST_ASSERT(store_.get<id_tag>().find(id) == store_.get<id_tag>().end());
      store_.insert(entry_t(id, iri, vers, p));
      return id;
   }

};

}//namespace owlcpp
#endif /* DOC_INFO_MAP_HPP_ */
