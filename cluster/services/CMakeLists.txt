add_library(services STATIC
	MONService.cc
	MDSService.cc
)
target_link_libraries(services fs)
