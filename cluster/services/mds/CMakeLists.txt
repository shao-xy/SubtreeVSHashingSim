add_library(mds STATIC
	MetadataDispatcher.cc
)
target_link_libraries(mds common services)
