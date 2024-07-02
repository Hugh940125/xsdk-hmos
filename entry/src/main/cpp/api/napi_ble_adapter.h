//
// Created on 2024/1/15.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#ifndef AiDEX_HarmonyOS_napi_ble_adapter_H
#define AiDEX_HarmonyOS_napi_ble_adapter_H

#include <js_native_api.h>
#include <js_native_api_types.h>

class NapiBleAdapter {
 public:
  static napi_value Init(napi_env env, napi_value exports);

 private:
  explicit NapiBleAdapter();
  ~NapiBleAdapter();
    
  static napi_value constructor(napi_env env, napi_callback_info info);

  static napi_value getServiceUUID(napi_env env, napi_callback_info info);

  static napi_value getCharacteristicUUID(napi_env env, napi_callback_info info);

  static napi_value getPrivateCharacteristicUUID(napi_env env, napi_callback_info info);

  static napi_value setDiscoverTimeoutSeconds(napi_env env, napi_callback_info info);

  static napi_value onAdvertiseWithAndroidRawBytes(napi_env env, napi_callback_info info);

  static napi_value onConnectSuccess(napi_env env, napi_callback_info info);

  static napi_value onConnectFailure(napi_env env, napi_callback_info info);

  static napi_value onDisconnected(napi_env env, napi_callback_info info);

  static napi_value onReceiveData(napi_env env, napi_callback_info info);

  static napi_value onReceiveDataWithUuid(napi_env env, napi_callback_info info);

  static napi_value setExecuteDisconnectCallback(napi_env env, napi_callback_info info);

  static napi_value setExecuteReadCharacteristicCallback(napi_env env, napi_callback_info info);

  static napi_value setEnableCharacteristicNotifyCallback(napi_env env, napi_callback_info info);

  static napi_value setIsReadyConnectCallback(napi_env env, napi_callback_info info);

  static napi_value setExecuteConnectCallback(napi_env env, napi_callback_info info);

  static napi_value setExecuteStartScanCallBack(napi_env env, napi_callback_info info);

  static napi_value setExecuteStopScanCallback(napi_env env, napi_callback_info info);

  static napi_value setExecuteWriteCallback(napi_env env, napi_callback_info info);

  static napi_value setExecuteWriteCharacteristicCallback(napi_env env, napi_callback_info info);
};

#endif // AiDEX_HarmonyOS_napi_ble_adapter_H
