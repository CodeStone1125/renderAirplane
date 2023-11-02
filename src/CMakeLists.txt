project(HW1 C CXX)

set(HW1_SOURCE
  ${HW1_SOURCE_DIR}/camera.cpp
  ${HW1_SOURCE_DIR}/opengl_context.cpp
  ${HW1_SOURCE_DIR}/main.cpp
)

set(HW1_HEADER
  ${HW1_SOURCE_DIR}/../include/camera.h
  ${HW1_SOURCE_DIR}/../include/opengl_context.h
  ${HW1_SOURCE_DIR}/../include/utils.h
)
add_executable(HW1 ${HW1_SOURCE} ${HW1_HEADER})
target_include_directories(HW1 PRIVATE ${HW1_SOURCE_DIR}/../include)

add_dependencies(HW1 glad glfw glm)
# Can include glfw and glad in arbitrary order
target_compile_definitions(HW1 PRIVATE GLFW_INCLUDE_NONE)
# More warnings
if (NOT MSVC)
  target_compile_options(HW1
    PRIVATE "-Wall"
    PRIVATE "-Wextra"
    PRIVATE "-Wpedantic"
  )
endif()
# Prefer std c++20, at least need c++17 to compile
set_target_properties(HW1 PROPERTIES
  CXX_STANDARD 20
  CXX_EXTENSIONS OFF
)

target_link_libraries(HW1
  PRIVATE glad
  PRIVATE glfw
)

if (TARGET glm::glm_shared)
  target_link_libraries(HW1 PRIVATE glm::glm_shared)
elseif(TARGET glm::glm_static)
  target_link_libraries(HW1 PRIVATE glm::glm_static)
else()
  target_link_libraries(HW1 PRIVATE glm::glm)
endif()
