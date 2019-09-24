add_library(global STATIC
	global_conf.cc
	global_disp.cc
)
target_link_libraries(global mds)
