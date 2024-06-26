﻿
#include <iostream>
#include <fstream>   
#include <string>
#include <ctime>

// include headers from rapidjson library
// do handle json documents
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/ostreamwrapper.h>

// include headers from azure-iot-sdk-c library 
// to creating client to connect to Azure IoTHub
#include "iothub.h"
#include "iothub_client_ll.h"
#include "iothub_device_client_ll.h"
#include "iothub_message.h"
#include "iothubtransportmqtt.h"
#include "azure_c_shared_utility/threadapi.h"


// Azure IotHub connection String
static const char* connectionString = "HostName=IotHub01.azure-devices.net;DeviceId=Device001;SharedAccessKey=PxORMzHDzAtbQFE0+YkJ4LuHW2CgDTuq0AIoTNXX5C8=";

// Input file name
static std::string file = "C:/Users/jleix/source/repos/IoT_Edge_SW/Data/Input.json";

// Flags to check callback methods met 
static bool message_send_confirmation = false;
static bool direct_methode_confirmation = false;


// method to convert rapidjson document to std::string
std::string doc2Str(rapidjson::Document& doc) {

    rapidjson::StringBuffer strbuf;
    strbuf.Clear();
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    doc.Accept(writer);

    return strbuf.GetString(); 
}

// method to create rapidjson document (input) from json file
rapidjson::Document readInput(std::string file) {

    std::ifstream ifs(file, std::ifstream::binary);
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    return doc;
}

// method to create rapidjson document (output) from input
rapidjson::Document createOutput(rapidjson::Document& docIn) {

    rapidjson::Document doc;
    doc.SetObject();

    // cast current unix time to use in rapidjson document
    rapidjson::Value val(rapidjson::kObjectType);
    std::string time = std::to_string(std::time(0));
    val.SetString(std::to_string(std::time(0)).c_str(), static_cast<rapidjson::SizeType>(time.length()), doc.GetAllocator());

    doc.AddMember("tag", "device001", doc.GetAllocator());
    doc.AddMember("type", "measures", doc.GetAllocator());
    doc.AddMember("pId", "1234567890", doc.GetAllocator());
    doc.AddMember("interval", "15", doc.GetAllocator());
    doc.AddMember("ts", val, doc.GetAllocator());
    doc.AddMember("ms", docIn, doc.GetAllocator());

    return doc;

}

// callback for messages sent
static void send_confirm_callback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* userContextCallback)
{
    (void)userContextCallback;
    // When a message is sent this callback will get invoked
    message_send_confirmation = true;
    std::cout << "Confirmation callback received for message with result " << MU_ENUM_TO_STRING(IOTHUB_CLIENT_CONFIRMATION_RESULT, result) << std::endl;
}

// callback to get client to Azure IoTHub connection status
void connection_status_callback(IOTHUB_CLIENT_CONNECTION_STATUS result, IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason, void* user_context)
{
    (void)reason;
    (void)user_context;

    if (result == IOTHUB_CLIENT_CONNECTION_AUTHENTICATED)
    {
        std::cout << "The device client is connected to iothub" << std::endl;
    }
    else
    {
        std::cout << "The device client has been disconnected" << std::endl;
    }
}

// catch direct methode call
static int DeviceMethodCallback(const char* method_name, const unsigned char* payload, size_t size, unsigned char** response, size_t* resp_size, void* userContextCallback)
{
    std::string device_id((const char*)userContextCallback);

    // check methode name set
    if (method_name == NULL)
    {
        std::cout << "invalid method name" << std::endl;
        return -1;
    }
    // check response set
    else if ((response == NULL) || (resp_size == NULL))
    {
        std::cout << "invalid response parameters" << std::endl;
        return -1;
    }
    // handle direct method 'Test' 
    else if (strcmp(method_name, "Test") == 0)
    {
        // show message from direct method
        std::cout << std::endl
            << "Device  : " << device_id << std::endl
            << "Method  : " << method_name << std::endl
            << "Payload : " << (const char*)payload << std::endl;

        // create response to direct method
        int status = 200;
        char* RESPONSE_STRING = "{ \"Response\": \"Received payload from methode test \" }";

        *resp_size = strlen(RESPONSE_STRING);
        *response = (unsigned char*)malloc(*resp_size);
        if (*response == NULL)
        {
            status = -1;
        }
        else
        {
            memcpy(*response, RESPONSE_STRING, *resp_size);
        }

        // direct method hit (called from IoTHub)
        direct_methode_confirmation= true;

        return status;
    }
}




int main(void)
{
    // Used to initialize IoTHub SDK subsystem
    (void)IoTHub_Init();

    // create and connect client to Azure IoTHub
    std::cout << "Creating IoTHub Device handle" << std::endl;
    IOTHUB_DEVICE_CLIENT_LL_HANDLE device_ll_handle = IoTHubDeviceClient_LL_CreateFromConnectionString(connectionString, MQTT_Protocol);
    if (device_ll_handle == NULL)
    {
        std::cout << "Failure creating IotHub device. Hint: Check your connection string." << std::endl;
    }
    else
    {
        // Setting connection status callback to get indication of connection to iothub
        if (IoTHubDeviceClient_LL_SetConnectionStatusCallback(device_ll_handle, connection_status_callback, NULL) != IOTHUB_CLIENT_OK)
        {
            std::cout << "ERROR: SetConnectionStatusCallback ..........FAILED!" << std::endl;
        }

        // Hook azure IoTHub direct methode callback 
        static const char* deviceId1 = "Device001";
        if (IoTHubDeviceClient_LL_SetDeviceMethodCallback(device_ll_handle, DeviceMethodCallback, (void*)deviceId1) != IOTHUB_CLIENT_OK)
        {
            std::cout << "ERROR: SetDeviceMethodCallback ..........FAILED!" << std::endl;
        }

        // read json input from file (input)
        rapidjson::Document docIn = readInput(file);
        std::cout << "Input: " << std::endl << doc2Str(docIn) << std::endl;

        // create json document to send to IoTHub (output)
        rapidjson::Document docOut = createOutput(docIn);
        std::cout << "Output: " << std::endl << doc2Str(docOut).c_str() << std::endl;

        // Construct the iothub message from the json object
        IOTHUB_MESSAGE_HANDLE message_handle = IoTHubMessage_CreateFromString(doc2Str(docOut).c_str());

        // Set Message property
        (void)IoTHubMessage_SetMessageId(message_handle, "MSG_ID");
        (void)IoTHubMessage_SetContentTypeSystemProperty(message_handle, "application%2fjson");

        // Add custom properties to message
        (void)IoTHubMessage_SetProperty(message_handle, "property_key", "property_value");

        // send mesage to IoTHub
        std::cout << "Sending message to IoTHub" << std::endl;
        IoTHubDeviceClient_LL_SendEventAsync(device_ll_handle, message_handle, send_confirm_callback, NULL);

        // The message is copied to the sdk so the we can destroy it
        IoTHubMessage_Destroy(message_handle);

        // wait for message confirmation callback
        while (!message_send_confirmation) {
            IoTHubDeviceClient_LL_DoWork(device_ll_handle);
            ThreadAPI_Sleep(10);
        }

        // wait for direct method confirmation callback
        while (!direct_methode_confirmation) {
            IoTHubDeviceClient_LL_DoWork(device_ll_handle);
            ThreadAPI_Sleep(10);
        }

        // Clean up the iothub sdk handle
        IoTHubDeviceClient_LL_Destroy(device_ll_handle);
    }
    // Free all the sdk subsystem
    IoTHub_Deinit();

    return 0;
}


