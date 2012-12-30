/** @file "/owlcpp/include/owlcpp/rdf/detail/map_id_object.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_ID_OBJECT_HPP_
#define MAP_ID_OBJECT_HPP_
#include <vector>
#include "boost/assert.hpp"
#include "boost/foreach.hpp"
#include "boost/unordered_map.hpp"
#include "boost/concept_check.hpp"

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
   typedef std::vector<obj_type const*> vector_t;

public:
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

   self_type& operator=(self_type const& mio);



private:
   vector_t vid_;
   map_t map_;
   std::vector<id_type> erased_;
   id_type id0_;

   void copy(self_type const& mio) {
      BOOST_ASSERT(id0_ == mio.id0_ && "base ID should be same");

   }
};


}//namespace detail
}//namespace owlcpp
#endif /* MAP_ID_OBJECT_HPP_ */
