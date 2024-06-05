# CMakeList.txt : CMake project for AzureIoTHubClient, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)



project ("AzureIoTHubClient" VERSION 1.0)

# Add source to this project's executable.
add_executable (AzureIoTHubClient "AzureIoTHubClient.cpp" )

# Set path to the Azure IOT SDK root
set (AZURE_IOT_SDK_C_DIR "C:/Users/jleix/source/repos/Library/azure-iot-sdk-c")

# Set path to the rapidjson root
set(RAPID_JSON_ROOT "C:/Users/jleix/source/repos/Library/rapidjson")

# define azure dependency libaries
set(AZURE_IOT_SDK_C_LINK_LIBRARIES winhttp crypt32 ws2_32 secur32 advapi32 ncrypt tbs)

# Include directories for Azure IoT SDK
target_include_directories(
    AzureIoTHubClient PUBLIC
    ${RAPID_JSON_ROOT}/include
    ${AZURE_IOT_SDK_C_DIR}/iothub_client/inc
    ${AZURE_IOT_SDK_C_DIR}/deps/parson
    ${AZURE_IOT_SDK_C_DIR}/provisioning_client/inc
    ${AZURE_IOT_SDK_C_DIR}/provisioning_client/deps/utpm/inc
    ${AZURE_IOT_SDK_C_DIR}/deps/umqtt/inc    
    ${AZURE_IOT_SDK_C_DIR}/c-utility/inc
    ${AZURE_IOT_SDK_C_DIR}/deps/azure-macro-utils-c/inc
    ${AZURE_IOT_SDK_C_DIR}/deps/umock-c/inc
    
)

# Link the necessary Azure IoT Hub libraries
target_link_libraries(
    AzureIoTHubClient 
    ${AZURE_IOT_SDK_C_LINK_LIBRARIES}
    ${AZURE_IOT_SDK_C_DIR}/cmake/iothub_client/Debug/iothub_client.lib
    ${AZURE_IOT_SDK_C_DIR}/cmake/provisioning_client/Debug/prov_device_client.lib
    ${AZURE_IOT_SDK_C_DIR}/cmake/iothub_client/Debug/iothub_client_mqtt_transport.lib
    ${AZURE_IOT_SDK_C_DIR}/cmake/provisioning_client/Debug/prov_auth_client.lib
    ${AZURE_IOT_SDK_C_DIR}/cmake/umqtt/Debug/umqtt.lib
    ${AZURE_IOT_SDK_C_DIR}/cmake/provisioning_client/Debug/hsm_security_client.lib
    ${AZURE_IOT_SDK_C_DIR}/cmake/provisioning_client/deps/utpm/Debug/utpm.lib
    ${AZURE_IOT_SDK_C_DIR}/cmake/c-utility/Debug/aziotsharedutil.lib
    ${AZURE_IOT_SDK_C_DIR}/cmake/deps/parson/Debug/parson.lib
)




