/** @file "/owlcpp/include/owlcpp/rdf/detail/map_id_object.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012-3
*******************************************************************************/
#ifndef MAP_ID_OBJECT_HPP_
#define MAP_ID_OBJECT_HPP_
#include <vector>
#include "boost/assert.hpp"
#include "boost/foreach.hpp"
#include "boost/unordered_map.hpp"
#include "boost/concept_check.hpp"
#include "owlcpp/rdf/exception.hpp"

namespace owlcpp{ namespace detail{

/**@brief 
*******************************************************************************/
template<class Obj, class Id> class Map_id_object{
public:
   typedef Map_id_object self_type;
   typedef Obj obj_type;
   typedef Id id_type;

private:
   typedef boost::unordered_map<obj_type, id_type> map_t;
   typedef typename map_t::value_type map_value_t;
   typedef std::vector<obj_type const*> vector_t;

public:

   typedef typename map_t::iterator iterator;
   typedef typename map_t::const_iterator const_iterator;
   typedef Rdf_err Err;

   Map_id_object(const id_type id0)
   : vid_(), map_(), erased_(), id0_(id0)
   {}

   Map_id_object(self_type const& mio)
   : vid_(mio.vid_.size()),
     map_(),
     erased_(mio.erased_),
     id0_(mio.id0_)
   {
      copy(mio);
   }

   self_type& operator=(self_type const& mio) {
      if( this != &mio ) {
         clear();
         id0_ = mio.id0_;
         vid_.resize(mio.vid_.size());
         copy(mio);
         erased_ = mio.erased_;
      }
      return *this;
   }

   std::size_t size() const { return map_.size(); }
   const_iterator begin() const {return map_.begin();}
   const_iterator end() const {return map_.end();}
   bool empty() const {return map_.empty();}

   obj_type const& operator[](const id_type id) const {
      BOOST_ASSERT( vpos(id) < vid_.size() );
      BOOST_ASSERT( vid_[vpos(id)] );
      return *vid_[vpos(id)];
   }

   obj_type const& at(const id_type id) const {
      if( obj_type const* obj = find(id) ) return *obj;
      BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("invalid ID")
               << Err::int1_t(id())
      );
   }

   obj_type const* find(const id_type id) const {
      if( id < id0_ ) return 0;
      const std::size_t n = vpos(id);
      if( n < vid_.size() ) return vid_[n];
      return 0;
   }

   id_type const* find(obj_type const& obj) const {
      const const_iterator i = map_.find(obj);
      return i == map_.end() ? 0 : &i->second;
   }

   id_type insert(obj_type const& obj) {
      std::pair<iterator,bool> ip = map_.emplace(obj, id_type());
      if( ip.second ) {
         const id_type id = make_id(ip.first);
         ip.first->second = id;
         return id;
      }
      return ip.first->second;
   }

   void insert(const id_type id, obj_type const& obj) {
      //todo: check erased IDs
      if( id < id0_ ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("invalid ID")
               << Err::int1_t(id())
      );

      if( id_type const* id0 = find(obj) ) {
         if( *id0 == id ) return;
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("object already mapped to different ID")
                  << Err::int1_t(id())
                  << Err::int2_t((*id0)())
         );
      }

      if( find(id) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("ID is not available")
               << Err::int1_t(id())
      );

      std::pair<const_iterator,bool> ip = map_.emplace(obj, id);
      BOOST_ASSERT(ip.second);
      //ignore erased_
      const std::size_t n = vpos(id);
      if( n < vid_.size() ) {
         vid_[n] = &ip.first->first;
      } else {
         vid_.resize(n + 1, 0);
         vid_.back() = &ip.first->first;
      }
   }

   void remove(const id_type id) {
      BOOST_ASSERT(find(id));
      const std::size_t pos = vpos(id);
      const std::size_t n = map_.erase(*vid_[pos]);
      boost::ignore_unused_variable_warning(n);
      BOOST_ASSERT(n);
      erased_.push_back(id);
      vid_[pos] = 0;
   }

   void clear() {
      erased_.clear();
      map_.clear();
      vid_.clear();
   }

private:
   vector_t vid_;
   map_t map_;
   std::vector<id_type> erased_;
   id_type id0_;

   std::size_t vpos(const id_type id) const {
      BOOST_ASSERT(id >= id0_ && "invalid ID");
      return id() - id0_();
   }

   void copy(self_type const& mio) {
      BOOST_ASSERT(id0_ == mio.id0_ && "base ID should be same");
      BOOST_FOREACH(map_value_t const& p, mio.map_) {
         std::pair<const_iterator,bool> ip = map_.insert(p);
         BOOST_ASSERT(ip.second && "duplicate object");
         vid_[vpos(p.second)] = &(ip.first->first);
      }
   }

   id_type pos2id(const std::size_t n) {return id_type(n + id0_());}

   id_type make_id(const const_iterator i) {
      obj_type const*const p = &i->first;
      if( erased_.empty() ) {
         const id_type id = pos2id(vid_.size());
         vid_.push_back(p);
         return id;
      }
      const id_type id = erased_.back();
      erased_.pop_back();
      const std::size_t n = vpos(id);
      BOOST_ASSERT(vid_.size() > n && ! vid_[n]);
      vid_[n] = p;
      return id;
   }

};

}//namespace detail
}//namespace owlcpp
#endif /* MAP_ID_OBJECT_HPP_ */
