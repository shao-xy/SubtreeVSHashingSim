add_library(process STATIC
	ClientProcess.cc
)
target_link_libraries(process network)
