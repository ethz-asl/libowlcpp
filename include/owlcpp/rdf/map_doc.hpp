/** @file "/owlcpp/include/owlcpp/rdf/map_doc.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_DOC_HPP_
#define MAP_DOC_HPP_
#include "boost/assert.hpp"
#include "owlcpp/config.hpp"
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/mem_fun.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/range.hpp"
#include "boost/foreach.hpp"


#include "owlcpp/doc_id.hpp"
#include "owlcpp/detail/id_tracker.hpp"
#include "owlcpp/rdf/doc_meta.hpp"
#include "owlcpp/detail/member_iterator.hpp"
#include "owlcpp/terms/node_tags_system.hpp"
#include "owlcpp/exception.hpp"


namespace owlcpp{

/**@brief Store document metadata
*******************************************************************************/
class Map_doc {
   struct Doc_meta_wrap {
      Doc_meta_wrap(
               const Node_id ontology_iri,
               const Node_id version_iri,
               std::string const& path,
               const Doc_id doc
      )
      : dm_(ontology_iri, version_iri, path), id_(doc)
      {}

      Node_id ontology_iri() const {return dm_.ontology_iri;}
      Node_id version_iri() const {return dm_.version_iri;}
      std::string const& path() const {return dm_.path;}
      Doc_meta dm_;
      Doc_id id_;
   };

   typedef boost::multi_index_container<
            Doc_meta_wrap,
            boost::multi_index::indexed_by<
               boost::multi_index::hashed_unique<
                  boost::multi_index::tag<struct id_tag>,
                  boost::multi_index::member<
                     Doc_meta_wrap, Doc_id, &Doc_meta_wrap::id_
                  >
               >,
               boost::multi_index::hashed_non_unique<
                  boost::multi_index::tag<struct iri_tag>,
                  boost::multi_index::const_mem_fun<
                     Doc_meta_wrap, Node_id, &Doc_meta_wrap::ontology_iri
                  >
               >,
               boost::multi_index::hashed_non_unique<
                  boost::multi_index::tag<struct version_tag>,
                  boost::multi_index::const_mem_fun<
                     Doc_meta_wrap, Node_id, &Doc_meta_wrap::version_iri
                  >
               >,
               boost::multi_index::hashed_non_unique<
                  boost::multi_index::tag<struct path_tag>,
                  boost::multi_index::const_mem_fun<
                     Doc_meta_wrap, std::string const&, &Doc_meta_wrap::path
                  >
               >
            >
   > map_t;

   typedef map_t::index<id_tag>::type id_index_t;
   typedef map_t::index<path_tag>::type path_index_t;
   typedef map_t::index<iri_tag>::type iri_index_t;
   typedef map_t::index<version_tag>::type version_index_t;

public:
   typedef Member_iterator<
            map_t::const_iterator,
            const Doc_id,
            &Doc_meta_wrap::id_
   > const_iterator;
   typedef const_iterator iterator;

   typedef Member_iterator<
            path_index_t::const_iterator,
            const Doc_id,
            &Doc_meta_wrap::id_
   > path_iterator;
   typedef boost::iterator_range<path_iterator> path_range;

   typedef Member_iterator<
            iri_index_t::const_iterator,
            const Doc_id,
            &Doc_meta_wrap::id_
   > iri_iterator;
   typedef boost::iterator_range<iri_iterator> iri_range;

   typedef Member_iterator<
            version_index_t::const_iterator,
            const Doc_id,
            &Doc_meta_wrap::id_
   > version_iterator;
   typedef boost::iterator_range<version_iterator> version_range;

   struct Err : public base_exception {};

   std::size_t size() const {return m_.size();}
   const_iterator begin() const {return m_.begin();}
   const_iterator end() const {return m_.end();}
   iri_iterator end_iri() const {return m_.get<iri_tag>().end();}
   version_iterator end_version() const {return m_.get<version_tag>().end();}

   Doc_meta const& operator[](const Doc_id did) const {
      id_index_t const& index = m_.get<id_tag>();
      const id_index_t::const_iterator iter = index.find(did);
      BOOST_ASSERT( iter != index.end() );
      return iter->dm_;
   }

   Doc_meta const& at(const Doc_id did) const {
      id_index_t const& index = m_.get<id_tag>();
      const id_index_t::const_iterator iter = index.find(did);
      if( iter == index.end() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("invalid document ID")
               << Err::int1_t(did())
      );
      return iter->dm_;
   }

   Doc_meta const* find(const Doc_id did) const {
      id_index_t const& index = m_.get<id_tag>();
      const id_index_t::const_iterator iter = index.find(did);
      if( iter == index.end() ) return 0;
      return &iter->dm_;
   }

   /**
    @param iri_id node ID of document's ontologyIRI
    @return iterator range for documents with @b id ontologyIRIs
   */
   iri_range find_iri(const Node_id iri_id) const {
      return m_.get<iri_tag>().equal_range(iri_id);
   }

   /**
    @param version_id node ID of document's versionIRI
    @return iterator range for documents with @b id versionIRIs
   */
   version_range find_version(const Node_id version_id) const {
      return m_.get<version_tag>().equal_range(version_id);
   }

   path_range find_path(std::string const& path) const {
      return m_.get<path_tag>().equal_range(path);
   }

   /**@brief Add document info: location, ontologyIRI, and versionIRI.
    @param iri ontologyIRI
    @param path document location
    @param vers versionIRI
    @return document ID and whether new document info was actually added
    @throw Err if an entry with the same non-empty @a path and different @a iri
    or @a version is already present.
    @details
    Duplicate document info entries are not allowed.

    If document info with same @a path, @a iri, and @a version already present,
    new info is not inserted and the ID of the existing document is returned.

    Since documents with same ontologyIRI and versionIRI may be found
    at different paths,
    multiple entries with different @a path and same @a iri or @a version
    can be added.

    Sometimes, document path is not known.
    Therefore multiple entries with empty @a path and different @a iri or
    @a version are allowed.
   */
   std::pair<Doc_id,bool> insert(
            const Node_id iri,
            std::string const& path = "",
            const Node_id vers = terms::empty_::id()
   ) {
      if( iri == terms::empty_::id() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("empty ontologyIRI is not allowed")
               << Err::str1_t(path)
      );
      if( iri == vers ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("ontologyIRI and versionIRI are same")
               << Err::str1_t(path)
      );

      Doc_id const* did = check_existing(iri, vers, path);
      if( did ) return std::make_pair(*did, false);
      const Doc_id id = idt_.get();
      m_.insert(Doc_meta_wrap(iri, vers, path, id));
      return std::make_pair(id, true);
   }

   void clear() {
      m_.clear();
      idt_ = detail::Id_tracker<Doc_id>();
   }

private:
   map_t m_;
   detail::Id_tracker<Doc_id> idt_;

   Doc_id const* check_existing(
            const Node_id iri,
            const Node_id vers,
            std::string const& path
   ) const {
      BOOST_ASSERT( ! is_empty(iri) );
      Doc_id const* idp = 0;

      //check version
      if( ! is_empty(vers) ) {
         BOOST_FOREACH(
                  Doc_meta_wrap const& dmw,
                  m_.get<version_tag>().equal_range(vers)
         ) {
            BOOST_ASSERT( ! is_empty(dmw.ontology_iri()) );
            if( dmw.ontology_iri() != iri ) BOOST_THROW_EXCEPTION(
                     Err()
                     << Err::msg_t("document with same versionIRI and "
                              "different ontologyIRI exists")
                     << Err::int1_t(dmw.id_())
            );
            if( path == dmw.path() ) idp = &dmw.id_;
         }
      }

      //check path
      if( ! path.empty() ) {
         BOOST_FOREACH(
                  Doc_meta_wrap const& dmw,
                  m_.get<path_tag>().equal_range(path)
         ) {
            if( dmw.ontology_iri() != iri || dmw.version_iri() != vers )
               BOOST_THROW_EXCEPTION(
                        Err()
                        << Err::msg_t("different versionIRI or ontologyIRI is "
                                 "recorded for same document")
                        << Err::str1_t(path)
                        << Err::int1_t(dmw.id_())
               );
            idp = &dmw.id_;
         }
      }

      if( is_empty(vers) && path.empty() ) {
         if( const iri_range r = find_iri(iri) ) {
            idp = &(*r.begin());
            BOOST_ASSERT( distance(r) == 1 );
         }
      }
      return idp;
   }

};

}//namespace owlcpp
#endif /* MAP_DOC_HPP_ */
