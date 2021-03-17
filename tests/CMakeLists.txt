# ##################################################################################################
# Define unit tests.
# ##################################################################################################
macro(unit_test NAME)
   add_executable(${NAME}_tests main.cpp ${NAME}_tests.cpp ${ARGN})
   target_link_libraries(${NAME}_tests PRIVATE bluegrass::cturtle Catch2::Catch2)
   catch_discover_tests(${NAME}_tests)
endmacro()

unit_test(logging)
