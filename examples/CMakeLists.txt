add_executable(ad_complex ad_complex.cpp)
target_link_libraries(ad_complex MissionImpossible::MissionImpossible)

add_executable(Hv Hv.cpp)
target_link_libraries(Hv MissionImpossible::MissionImpossible)

add_executable(Jacobian Jacobian.cpp)
target_link_libraries(Jacobian MissionImpossible::MissionImpossible)

add_executable(local_tape local_tape.cpp)
target_link_libraries(local_tape MissionImpossible::MissionImpossible)

add_executable(nested nested.cpp)
target_link_libraries(nested MissionImpossible::MissionImpossible)

add_executable(tiny_network tiny_network.cpp)
target_link_libraries(tiny_network MissionImpossible::MissionImpossible)
