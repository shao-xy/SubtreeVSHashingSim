add_executable(test_file_system file_system.cc)
target_link_libraries(test_file_system common fs)

add_executable(test_hashing hashing.cc)
target_link_libraries(test_hashing common)

add_executable(test_lru testlru.cc)
target_link_libraries(test_lru common)

add_executable(test_time time.cc)
target_link_libraries(test_time common time)

add_executable(test_exlog exlog.cc)
target_link_libraries(test_exlog common)

add_executable(test_random random.cc)
target_link_libraries(test_random common)
