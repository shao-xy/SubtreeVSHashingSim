add_subdirectory(network)
add_subdirectory(process)
add_subdirectory(services)

add_library(cluster STATIC
	Cluster.cc
	Host.cc
	Process.cc
	Service.cc
)
target_link_libraries(cluster process services common time)
