/** @file "/owlcpp/include/owlcpp/detail/object_store.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OBJECT_STORE_HPP_
#define OBJECT_STORE_HPP_
#include <map>
#include <vector>
#include "boost/assert.hpp"
#include "owlcpp/exception.hpp"

namespace owlcpp{ namespace detail{


/**@brief 
*******************************************************************************/
template<
   class Obj,
   class Id = unsigned,
   class Map = std::map<Obj, Id>,
   class Vector = std::vector<typename Map::iterator>
> class Object_store {
   typedef Map map_t;
public:
   typedef Obj object_type;
   typedef unsigned id_type;
   typedef typename map_t::const_iterator const_iterator;
   typedef typename map_t::iterator iterator;
private:
   typedef Vector vector_t;
public:
   typedef base_exception Err;

   std::size_t size() const {
      BOOST_ASSERT(vector_.size() >= map_.size());
      return map_.size();
   }
   const_iterator begin() const {return map_.begin();}
   const_iterator end() const {return map_.end();}

   bool have(const id_type id) const {return id < vector_.size() && vector_[id] != map_.end();}

   const_iterator operator[](const id_type id) const {
      BOOST_ASSERT(have(id));
      return vector_[id];
   }

   const_iterator at(const id_type id) const {
      if( ! have(id) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("invalid ID")
               << Err::int1_t(id)
      );
      return vector_[id];
   }

   const_iterator find(object_type const& obj) const {return map_.find(obj);}

   id_type insert(object_type const& obj) {
      const iterator i = map_.find(obj);
      if( i != map_.end() ) return i->second;
      const id_type id(vector_.size());
      vector_.push_back(map_.insert(std::make_pair(obj, id)).first);
      return id;
   }

   id_type insert(const id_type id, object_type const& obj) {
      if( id >= vector_.size() ) {
         vector_.resize(id + 1, map_.end());
      } else {
         const iterator i = vector_[id];
         if( i != map_.end() ) {
            if( i->first != obj ) BOOST_THROW_EXCEPTION(
                     Err()
                     << Err::msg_t("duplicate ID")
                     << Err::int1_t(id)
            );

            BOOST_ASSERT(map_.find(obj) == i);
            return id;
         }
      }
      BOOST_ASSERT(map_.find(obj) == map_.end());
      vector_[id] = map_.insert(std::make_pair(obj, id)).first;
      return id;
   }

   void remove(const id_type id) {
      BOOST_ASSERT(vector_.at(id) != map_.end());
      const iterator i = vector_[id];
      map_.remove(i);
   }

   void clear() {
      map_.clear();
      vector_.clear();
   }

private:
   map_t map_;
   vector_t vector_;
};

}//namespace detail
}//namespace owlcpp
#endif /* OBJECT_STORE_HPP_ */
