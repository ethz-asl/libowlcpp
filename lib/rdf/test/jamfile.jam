# owlcpp/lib/rdf/test/jamfile.jam
# part of owlcpp project.
# Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
# Copyright Mikhail K Levin 2011

import path ;
local smpl_dir = [ path.native $(SMPL_DATA) ] ;
local tmp_dir = [ path.native $(TEMP_DIR) ] ;
project owlcpp/lib/rdf/test
   : 
   : requirements
      <include>../
      <library>/boost//unit_test_framework
      <toolset>msvc:<threading>multi
;

import testing ;

rule run-all ( sources * : requirements * )
{
   for local source in $(sources)
   {
       return [ run $(source) : : : $(requirements) ] ;
   }
}

test-suite simple_tests 
   :
      [ run-all [ glob *run.cpp ] : 
      <library>/owlcpp//rdf
      ]
;

test-suite other_tests
   :
      [ compile-fail object_id_01_cfail.cpp ]
;

