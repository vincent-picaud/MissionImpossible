test_array = [
  ['tape_test','tape_exe','tape.cpp'],
  ['ad_test','ad_exe','ad.cpp'],
  ['tape_vector_test','tape_vector_exe','tape_vector.cpp'],
  ['derivatives_test','derivatives_exe','derivatives.cpp'],
  ['underlying_type_test','underlying_type_exe','underlying_type.cpp'],
  ['nested_2nd_order_test','nested_2nd_order_exe','nested_2nd_order.cpp'],
  ['mission_impossible_tape_test','mission_impossible_tape_exe','mission_impossible_tape.cpp'],
  ['functions_test','functions_exe','functions.cpp'],
  ['forward_mode_test','forward_mode_exe','forward_mode.cpp'],
  ['Hv_test','Hv_exe','Hv.cpp'],
  ['third_order_test','third_order_exe','third_order.cpp'],
  ['min_extra_tests_test','min_extra_tests_exe','min_extra_tests.cpp'],
]

foreach test : test_array
  test(test.get(0),
       executable(test.get(1),
		  test.get(2),
		  dependencies
 		  : [ MissionImpossible_dep, gtest_dep ]))
endforeach
