add_subdirectory(mds)

add_library(services STATIC
	MONService.cc
	MDSService.cc
)
target_link_libraries(services mds global)
