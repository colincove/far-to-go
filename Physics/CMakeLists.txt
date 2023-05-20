add_subdirectory(bullet3)

add_library(
	Physics
	physics_test.cpp)

target_include_directories(Physics
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )

install(TARGETS Physics CONFIGURATIONS Debug DESTINATION Debug/lib)

target_link_libraries(
	Physics
	PRIVATE ${BULLET_LIBRARIES})