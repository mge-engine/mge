#EXECUTE_PROCESS(COMMAND "${PYTHON_EXECUTABLE}" ${CMAKE_SOURCE_DIR}/src/modules/vulkan/extract_functions.py "${Vulkan_INCLUDE_DIRS}/vulkan/vulkan.h" "${CMAKE_BINARY_DIR}/src/modules/vulkan/vulkan_function_members.hpp" "${CMAKE_BINARY_DIR}/src/modules/vulkan/vk/vulkan_function_resolve.hpp" "${CMAKE_BINARY_DIR}/src/modules/vulkan/vk/vulkan_generated.cpp")

ADD_CUSTOM_COMMAND(OUTPUT "${CMAKE_BINARY_DIR}/src/modules/vulkan/vk/vulkan_generated.cpp"
                   COMMAND "${PYTHON_EXECUTABLE}" ${CMAKE_SOURCE_DIR}/src/modules/vulkan/extract_functions.py "${Vulkan_INCLUDE_DIRS}/vulkan/vulkan.h" "${CMAKE_BINARY_DIR}/src/modules/vulkan/vulkan_function_members.hpp" "${CMAKE_BINARY_DIR}/src/modules/vulkan/vk/vulkan_function_resolve.hpp" "${CMAKE_BINARY_DIR}/src/modules/vulkan/vk/vulkan_generated.cpp"
                   DEPENDS ${CMAKE_SOURCE_DIR}/src/modules/vulkan/extract_functions.py)

#ADD_LIBRARY(vkutils STATIC
#    instance.cpp
#    library.cpp
#    surface.cpp
#    physical_device.cpp
#    queue_family.cpp
#    swap_chain.cpp
#    device.cpp
#    queue.cpp
#    image_view.cpp
#    render_pass.cpp
#    framebuffer.cpp
#    error.cpp
#    ${CMAKE_BINARY_DIR}/src/modules/vulkan/vk/vulkan_generated.cpp)
#TARGET_INCLUDE_DIRECTORIES(vkutils PRIVATE "${CMAKE_BINARY_DIR}/src/modules/vulkan/vk" "${CMAKE_BINARY_DIR}/src/modules/vulkan")
#TARGET_LINK_LIBRARIES(vkutils vulkan)
#ADD_SUBDIRECTORY(test)
