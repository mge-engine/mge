SET(extract_functions "${CMAKE_SOURCE_DIR}/src/modules/vulkan/extract_functions.py")
SET(vulkan_generated_source "${CMAKE_BINARY_DIR}/src/modules/vulkan/vulkan_generated.cpp")
ADD_CUSTOM_COMMAND(
    OUTPUT ${vulkan_generated_source}
    COMMAND "${PYTHON_EXECUTABLE}" ${extract_functions} ${vulkan_generated_source}
    DEPENDS ${extract_functions}
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/src/modules/vulkan/vk")

ADD_LIBRARY(vkutils STATIC
            library.cpp
            ${vulkan_generated_source})

TARGET_LINK_LIBRARIES(vkutils mgecore vulkan)

#EXECUTE_PROCESS(COMMAND "${PYTHON_EXECUTABLE}" ${CMAKE_SOURCE_DIR}/src/modules/vulkan/extract_functions.py "${Vulkan_INCLUDE_DIRS}/vulkan/vulkan.h" "${CMAKE_BINARY_DIR}/src/modules/vulkan/vulkan_function_members.hpp" "${CMAKE_BINARY_DIR}/src/modules/vulkan/vk/vulkan_function_resolve.hpp" "${CMAKE_BINARY_DIR}/src/modules/vulkan/vk/vulkan_generated.cpp")

#ADD_CUSTOM_COMMAND(OUTPUT "${CMAKE_BINARY_DIR}/src/modules/vulkan/vk/vulkan_generated.cpp"
#                   COMMAND "${PYTHON_EXECUTABLE}" ${CMAKE_SOURCE_DIR}/src/modules/vulkan/extract_functions.py "${Vulkan_INCLUDE_DIRS}/vulkan/vulkan.h" "${CMAKE_BINARY_DIR}/src/modules/vulkan/vulkan_function_members.hpp" "${CMAKE_BINARY_DIR}/src/modules/vulkan/vk/vulkan_function_resolve.hpp" "${CMAKE_BINARY_DIR}/src/modules/vulkan/vk/vulkan_generated.cpp"
#                   DEPENDS ${CMAKE_SOURCE_DIR}/src/modules/vulkan/extract_functions.py)

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
