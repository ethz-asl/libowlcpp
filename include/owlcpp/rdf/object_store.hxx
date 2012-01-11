/** @file "/owlcpp/include/owlcpp/rdf/object_store.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef OBJECT_STORE_HPP_
#define OBJECT_STORE_HPP_
#include <vector>
#include <cassert>
#include <memory>
#include <functional>
#include "boost/unordered_map.hpp"
#include "boost/range.hpp"
#include "boost/iterator/indirect_iterator.hpp"
#include "boost/iterator/counting_iterator.hpp"
#include "boost/iterator/transform_iterator.hpp"
namespace b = boost;
#include "boost/foreach.hpp"

#include "owlcpp/exception.hpp"

namespace owlcpp{

template<class Obj> struct Stored_obj_traits {
   typedef Obj obj_t;
   typedef typename obj_t::id_t id_t;
};

/** Store polymorphic objects and retrieve by ID
*******************************************************************************/
template<class Obj> class Obj_store {
   Obj_store(const Obj_store&); //no copying
   Obj_store& operator=(const Obj_store&);
public:
   typedef Obj obj_t;
   typedef typename Stored_obj_traits<obj_t>::id_t id_t;
private:
   typedef std::vector<obj_t*> obj_stor_t;
   typedef typename obj_stor_t::const_iterator stor_iter_t;

  struct Equal : std::binary_function<obj_t*, obj_t*,bool> {
    bool operator()(obj_t const* o1, obj_t const* o2) const {
       return (*o1) == (*o2);
    }
  };

  struct Hash : std::unary_function<obj_t*, std::size_t> {
    std::size_t operator()(obj_t const* o) const {
       std::size_t seed = 0;
       boost::hash_combine(seed, *o);
       return seed;
    }
  };

  struct Id_maker : public std::unary_function<unsigned, id_t> {
    id_t operator()(const unsigned n) const {return id_t(n);}
  };

   typedef b::unordered_map<const obj_t*, id_t, Hash, Equal> map_stor_t;
   typedef typename map_stor_t::const_iterator map_citer_t;
   typedef b::indirect_iterator<stor_iter_t, obj_t> obj_iter_t;
   typedef b::counting_iterator<unsigned> count_iter_t;
   typedef b::transform_iterator<Id_maker, count_iter_t> id_iter_t;
public:
   typedef b::iterator_range<obj_iter_t> obj_range_t;
   typedef b::iterator_range<id_iter_t> id_range_t;
   typedef b::iterator_range<map_citer_t> find_range_t;

   Obj_store() : map_(), array_() {}

   std::size_t size() const {return array_.size();}

   find_range_t find(const obj_t& t) const {
      return b::make_iterator_range( map_.equal_range(&t) );
   }

   bool have(const id_t id) const {
      if( array_.size() <= id() ) return false;
      if( array_[id()] == 0 ) return false;
      return true;
   }

   /**@brief Store the object if same object is not already stored.
    * @details No copying is performed.
    * Internally, the object is stored as a heap-allocated pointer.
    * @param obj object for storing is taken as auto_ptr
    * to highlight that it has to be heap-allocated
    * @return stored object ID
    */
   id_t insert(std::auto_ptr<obj_t> obj) {
      const map_citer_t i = map_.find(obj.get());
      if( i == map_.end() ) {
         id_t id(array_.size());
         array_.push_back(obj.get());
         map_.insert( std::make_pair(obj.get(), id) );
         obj.release();
         return id;
      }
      return i->second;
   }

   template<class T> id_t insert(const T& t) {
      typedef typename map_stor_t::iterator map_iter_t;
      const std::pair<map_iter_t, map_iter_t> ip = map_.equal_range(&t);
      if( ip.first != ip.second ) return ip.first->second;
      T* p = new T(t);
      const id_t id(array_.size());
      map_.insert(ip.first, std::make_pair(p, id) );
      array_.push_back(p);
      return id;
   }

   /**
    * Insert object with a specific ID;
    * @param t should be subclass of obj_t
    * @param id
    * @return id
    */
   template<class T> id_t insert(const T& t, const id_t id) {
      assert( ! find(t) );
      assert( ! have(id) );
      if( array_.size() <= id() ) {
         array_.resize( id() + 1, 0);
      }
      array_[id()] = new T(t);
      map_.insert( std::make_pair( array_[id()], id ) );
      return id;
   }

   const obj_t& get(const id_t id) const {
      assert( have(id) );
      return *array_[id()];
   }

   ~Obj_store() {
      BOOST_FOREACH(obj_t* obj, array_) {
         if( obj ) delete obj;
      }
   }

   obj_range_t objects() const {
      return b::make_iterator_range(
            obj_iter_t(array_.begin()),
            obj_iter_t(array_.end())
      );
   }

   id_range_t object_ids() const {
      return b::make_iterator_range(
            id_iter_t(count_iter_t(0), Id_maker()),
            id_iter_t(count_iter_t(array_.size()), Id_maker())
      );
   }

private:
   map_stor_t map_;
   obj_stor_t array_;
};

}//namespace owlcpp
#endif /* OBJECT_STORE_HPP_ */
