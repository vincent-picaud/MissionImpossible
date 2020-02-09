MissionImpossible_headers = [
  'tape.hpp',
  'ad_fwd.hpp',
  'ad.hpp',
  'identity.hpp',
  'tape_vector.hpp',
  'derivatives.hpp',
  'always.hpp',
  'underlying_type.hpp',
  'mission_impossible_tape.hpp',
  'MissionImpossible.hpp',   # main header 
  'functions.hpp',
  'complex_support.hpp',
]

#################################################################
# Uncomment me for if you need some .cpp file
#################
#
# MissionImpossible_sources = ['tape.cpp']

# MissionImpossible_lib = library('MissionImpossible',
# 			  include_directories : inc,
# 			  install : true,
# 			  sources: [MissionImpossible_headers,MissionImpossible_sources])

# MissionImpossible_dep = declare_dependency(include_directories : inc,
# 				     link_with : MissionImpossible_lib)

#################################################################
# Header only lib
#################
#
MissionImpossible_dep = declare_dependency(include_directories : inc)

install_headers(MissionImpossible_headers,
		subdir : 'MissionImpossible')

