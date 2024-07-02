//
// Created on 2024/1/15.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#include "napi_ble_adapter.h"
#include "nativebleadapter.h"
#include <node_api.h>

napi_value NapiBleAdapter::constructor(napi_env env, napi_callback_info info) {
    napi_value targetObj = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, 0, nullptr, &targetObj, &data);
    NativeBleAdapter *bleAdapter = new NativeBleAdapter();
    napi_wrap(
        env, targetObj, bleAdapter,
        [](napi_env /*env*/, void *data, void * /*hint*/) {
            NativeBleAdapter *bind = (NativeBleAdapter *)data;
            delete bind;
            bind = nullptr;
        },
        nullptr, nullptr);
    return targetObj;
}

napi_value NapiBleAdapter::getServiceUUID(napi_env env, napi_callback_info info) {
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, 0, nullptr, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    napi_value serviceUuid;
    napi_create_int32(env, bleAdapter->getServiceUUID(), &serviceUuid);
    return serviceUuid;
}

napi_value NapiBleAdapter::getCharacteristicUUID(napi_env env, napi_callback_info info) {
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, 0, nullptr, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    napi_value uuid;
    napi_create_int32(env, bleAdapter->getCharacteristicUUID(), &uuid);
    return uuid;
}

napi_value NapiBleAdapter::getPrivateCharacteristicUUID(napi_env env, napi_callback_info info) {
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, 0, nullptr, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    napi_value uuid;
    napi_create_int32(env, bleAdapter->getPrivateCharacteristicUUID(), &uuid);
    return uuid;
}

napi_value NapiBleAdapter::setDiscoverTimeoutSeconds(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    uint32_t seconds;
    napi_get_value_uint32(env, args[0], &seconds);
    bleAdapter->setDiscoverTimeoutSeconds(seconds);
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value NapiBleAdapter::onAdvertiseWithAndroidRawBytes(napi_env env, napi_callback_info info) {
    size_t argc = 3;
    napi_value args[3] = {nullptr};
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    size_t bufferSize = 0;
    napi_get_value_string_utf8(env, args[0], nullptr, 0, &bufferSize);
    char *buffer = (char *)malloc(bufferSize + 1);
    size_t address;
    napi_get_value_string_utf8(env, args[0], buffer, bufferSize + 1, &address);
    int32 rssi;
    napi_get_value_int32(env, args[1], &rssi);
    uint32_t arraySize;
    napi_get_array_length(env, args[2], &arraySize);
    void *data;
    size_t length = static_cast<size_t>(arraySize);
    napi_get_arraybuffer_info(env, args[2], &data, &length);
    const char *convertedData = reinterpret_cast<const char *>(data);
    bleAdapter->onAdvertiseWithAndroidRawBytes(buffer, rssi, convertedData, arraySize);
    free(buffer);
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value NapiBleAdapter::onConnectSuccess(napi_env env, napi_callback_info info) {
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, 0, nullptr, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    bleAdapter->onConnectSuccess();
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value NapiBleAdapter::onConnectFailure(napi_env env, napi_callback_info info) {
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, 0, nullptr, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    bleAdapter->onConnectFailure();
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value NapiBleAdapter::onDisconnected(napi_env env, napi_callback_info info) {
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, 0, nullptr, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    bleAdapter->onDisconnected();
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value NapiBleAdapter::onReceiveData(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    uint32_t arraySize;
    napi_get_array_length(env, args[0], &arraySize);
    void *data;
    size_t length = static_cast<size_t>(arraySize);
    napi_get_arraybuffer_info(env, args[0], &data, &length);
    const char *convertedData = reinterpret_cast<const char *>(data);
    bleAdapter->onReceiveData(convertedData, arraySize);
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value NapiBleAdapter::onReceiveDataWithUuid(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value args[2] = {nullptr};
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    int32 uuid;
    napi_get_value_int32(env, args[0], &uuid);
    uint32_t arraySize;
    napi_get_array_length(env, args[1], &arraySize);
    void *data;
    size_t length = static_cast<size_t>(arraySize);
    napi_get_arraybuffer_info(env, args[1], &data, &length);
    const char *convertedData = reinterpret_cast<const char *>(data);
    bleAdapter->onReceiveData(uuid, convertedData, arraySize);
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value NapiBleAdapter::setExecuteDisconnectCallback(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    napi_value jsFunction = args[0];
    napi_ref callbackRef;
    napi_create_reference(env, jsFunction, 1, &callbackRef);
    bleAdapter->setExecuteDisconnectCallback([env, callbackRef]() {
        napi_value callback;
        napi_get_reference_value(env, callbackRef, &callback);
        napi_call_function(env, nullptr, callback, 0, nullptr, nullptr);
    });
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value NapiBleAdapter::setExecuteReadCharacteristicCallback(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    napi_value jsFunction = args[0];
    napi_ref callbackRef;
    napi_create_reference(env, jsFunction, 1, &callbackRef);
    bleAdapter->setExecuteReadCharacteristicCallback([env, callbackRef](int uuid) {
        napi_value callback;
        napi_get_reference_value(env, callbackRef, &callback);
        size_t argSize = 1;
        napi_value argv[1] = {nullptr};
        napi_create_int32(env, uuid, &argv[0]);
        napi_call_function(env, nullptr, callback, argSize, argv, nullptr);
    });
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value NapiBleAdapter::setEnableCharacteristicNotifyCallback(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    napi_value jsFunction = args[0];
    napi_ref callbackRef;
    napi_create_reference(env, jsFunction, 1, &callbackRef);
    bleAdapter->setEnableCharacteristicNotifyCallback([env, callbackRef](int uuid) {
        napi_value callback;
        napi_get_reference_value(env, callbackRef, &callback);
        size_t argSize = 1;
        napi_value argv[1] = {nullptr};
        napi_create_int32(env, uuid, &argv[0]);
        napi_call_function(env, nullptr, callback, argSize, argv, nullptr);
    });
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value NapiBleAdapter::setIsReadyConnectCallback(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    napi_value jsFunction = args[0];
    napi_ref callbackRef;
    napi_create_reference(env, jsFunction, 1, &callbackRef);
    bleAdapter->setIsReadyConnectCallback([env, callbackRef](string mac) {
        napi_value callback;
        napi_get_reference_value(env, callbackRef, &callback);
        size_t argSize = 1;
        napi_value argv[1] = {nullptr};
        napi_create_string_utf8(env, mac.data(), mac.size(), &argv[0]);
        napi_value result;
        napi_status status = napi_call_function(env, nullptr, callback, argSize, argv, &result);
        if (status == napi_ok) {
            bool isReady;
            napi_get_value_bool(env, result, &isReady);
            return isReady;
        }
        return false;
    });
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value NapiBleAdapter::setExecuteConnectCallback(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    napi_value jsFunction = args[0];
    napi_ref callbackRef;
    napi_create_reference(env, jsFunction, 1, &callbackRef);
    bleAdapter->setExecuteConnectCallback([env, callbackRef](string mac) {
        napi_value callback;
        napi_get_reference_value(env, callbackRef, &callback);
        size_t argSize = 1;
        napi_value argv[1] = {nullptr};
        napi_create_string_utf8(env, mac.data(), mac.size(), &argv[0]);
        napi_call_function(env, nullptr, callback, argSize, argv, nullptr);
    });
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value NapiBleAdapter::setExecuteStartScanCallBack(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    napi_value jsFunction = args[0];
    napi_ref callbackRef;
    napi_create_reference(env, jsFunction, 1, &callbackRef);
    bleAdapter->setExecuteStartScanCallBack([env, callbackRef]() {
        napi_value callback;
        napi_get_reference_value(env, callbackRef, &callback);
        napi_call_function(env, nullptr, callback, 0, nullptr, nullptr);
    });
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value NapiBleAdapter::setExecuteStopScanCallback(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    napi_value jsFunction = args[0];
    napi_ref callbackRef;
    napi_create_reference(env, jsFunction, 1, &callbackRef);
    bleAdapter->setExecuteStopScanCallback([env, callbackRef]() {
        napi_value callback;
        napi_get_reference_value(env, callbackRef, &callback);
        napi_call_function(env, nullptr, callback, 0, nullptr, nullptr);
    });
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value NapiBleAdapter::setExecuteWriteCallback(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    napi_value jsFunction = args[0];
    napi_ref callbackRef;
    napi_create_reference(env, jsFunction, 1, &callbackRef);
    bleAdapter->setExecuteWriteCallback([env, callbackRef](const char *data, uint16 length) {
        napi_value callback;
        napi_get_reference_value(env, callbackRef, &callback);
        size_t argSize = 2;
        napi_value argv[argSize];
        napi_create_buffer_copy(env, length, data, nullptr, &argv[0]);
        napi_create_int32(env, length, &argv[1]);
        napi_call_function(env, nullptr, callback, argSize, argv, nullptr);
    });
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value NapiBleAdapter::setExecuteWriteCharacteristicCallback(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value args[1] = {nullptr};
    napi_value thisArg = nullptr;
    napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    NativeBleAdapter *bleAdapter = nullptr;
    napi_unwrap(env, thisArg, (void **)&bleAdapter);
    napi_value jsFunction = args[0];
    napi_ref callbackRef;
    napi_create_reference(env, jsFunction, 1, &callbackRef);
    bleAdapter->setExecuteWriteCharacteristicCallback([env, callbackRef](uint16 uuid, const char *data, uint16 length) {
        napi_value callback;
        napi_get_reference_value(env, callbackRef, &callback);
        size_t argSize = 3;
        napi_value argv[argSize];
        napi_create_int32(env, uuid, &argv[0]);
        napi_create_buffer_copy(env, length, data, nullptr, &argv[1]);
        napi_create_int32(env, length, &argv[2]);
        napi_call_function(env, nullptr, callback, argSize, argv, nullptr);
    });
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value NapiBleAdapter::Init(napi_env env, napi_value exports) {
    napi_property_descriptor classProp[] = {
        {"getServiceUUID", nullptr, NapiBleAdapter::getServiceUUID, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"getCharacteristicUUID", nullptr, NapiBleAdapter::getCharacteristicUUID, nullptr, nullptr, nullptr,
         napi_default, nullptr},
        {"getPrivateCharacteristicUUID", nullptr, NapiBleAdapter::getPrivateCharacteristicUUID, nullptr, nullptr,
         nullptr, napi_default, nullptr},
        {"setDiscoverTimeoutSeconds", nullptr, NapiBleAdapter::setDiscoverTimeoutSeconds, nullptr, nullptr, nullptr,
         napi_default, nullptr},
        {"onAdvertiseWithAndroidRawBytes", nullptr, NapiBleAdapter::onAdvertiseWithAndroidRawBytes, nullptr, nullptr,
         nullptr, napi_default, nullptr},
        {"onConnectSuccess", nullptr, NapiBleAdapter::onConnectSuccess, nullptr, nullptr, nullptr, napi_default,
         nullptr},
        {"onConnectFailure", nullptr, NapiBleAdapter::onConnectFailure, nullptr, nullptr, nullptr, napi_default,
         nullptr},
        {"onDisconnected", nullptr, NapiBleAdapter::onDisconnected, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"onReceiveData", nullptr, NapiBleAdapter::onReceiveData, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"onReceiveDataWithUuid", nullptr, NapiBleAdapter::onReceiveDataWithUuid, nullptr, nullptr, nullptr,
         napi_default, nullptr},
        {"setExecuteConnectCallback", nullptr, NapiBleAdapter::setExecuteConnectCallback, nullptr, nullptr, nullptr,
         napi_default, nullptr},
        {"setExecuteDisconnectCallback", nullptr, NapiBleAdapter::setExecuteDisconnectCallback, nullptr, nullptr,
         nullptr, napi_default, nullptr},
        {"setExecuteStartScanCallBack", nullptr, NapiBleAdapter::setExecuteStartScanCallBack, nullptr, nullptr, nullptr,
         napi_default, nullptr},
        {"setExecuteStopScanCallback", nullptr, NapiBleAdapter::setExecuteStopScanCallback, nullptr, nullptr, nullptr,
         napi_default, nullptr},
        {"setExecuteReadCharacteristicCallback", nullptr, NapiBleAdapter::setExecuteReadCharacteristicCallback, nullptr,
         nullptr, nullptr, napi_default, nullptr},
        {"setEnableCharacteristicNotifyCallback", nullptr, NapiBleAdapter::setEnableCharacteristicNotifyCallback,
         nullptr, nullptr, nullptr, napi_default, nullptr},
        {"setExecuteWriteCallback", nullptr, NapiBleAdapter::setExecuteWriteCallback, nullptr, nullptr, nullptr,
         napi_default, nullptr},
        {"setExecuteWriteCharacteristicCallback", nullptr, NapiBleAdapter::setExecuteWriteCharacteristicCallback,
         nullptr, nullptr, nullptr, napi_default, nullptr},
        {"setIsReadyConnectCallback", nullptr, NapiBleAdapter::setIsReadyConnectCallback, nullptr, nullptr, nullptr,
         napi_default, nullptr}};
    napi_value classBind = nullptr;
    const char *className = "NapiBleAdapter";
    const size_t numProperties = sizeof(classProp) / sizeof(classProp[0]);
    napi_define_class(env, className, NAPI_AUTO_LENGTH, NapiBleAdapter::constructor, nullptr,
                      numProperties, classProp, &classBind);
    napi_set_named_property(env, exports, className, classBind);
    return exports;
}
