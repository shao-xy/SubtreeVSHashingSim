add_subdirectory(services)

add_library(cluster STATIC
	Cluster.cc
	Host.cc
	Service.cc
)
target_link_libraries(cluster services common)
