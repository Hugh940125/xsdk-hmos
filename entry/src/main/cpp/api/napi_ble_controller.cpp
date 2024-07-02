//
// Created on 2024/6/26.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#include <js_native_api_types.h>

class NapiBleController {
 public:
  static napi_value Init(napi_env env, napi_value exports);

  explicit NapiBleController();
  ~NapiBleController();
    
  static napi_value constructor(napi_env env, napi_callback_info info);
    
  static napi_value setOnDiscoverCallback(napi_env env, napi_callback_info info);

  static napi_value setMessageCallback(napi_env env, napi_callback_info info);

  static napi_value setBleAdapter(napi_env env, napi_callback_info info);

  static napi_value getMac(napi_env env, napi_callback_info info);

  static napi_value setMac(napi_env env, napi_callback_info info);

  static napi_value getName(napi_env env, napi_callback_info info);

  static napi_value setName(napi_env env, napi_callback_info info);

  static napi_value getSn(napi_env env, napi_callback_info info);

  static napi_value setSn(napi_env env, napi_callback_info info);

  static napi_value getHostAddress(napi_env env, napi_callback_info info);

  static napi_value setHostAddress(napi_env env, napi_callback_info info);

  static napi_value getId(napi_env env, napi_callback_info info);

  static napi_value setId(napi_env env, napi_callback_info info);

  static napi_value getKey(napi_env env, napi_callback_info info);

  static napi_value setKey(napi_env env, napi_callback_info info);

  static napi_value getRssi(napi_env env, napi_callback_info info);

  static napi_value setSupportKeepConnect(napi_env env, napi_callback_info info);

  static napi_value register(napi_env env, napi_callback_info info);

  static napi_value unregister(napi_env env, napi_callback_info info);

  static napi_value disconnect(napi_env env, napi_callback_info info);

  static napi_value pair(napi_env env, napi_callback_info info);

  static napi_value unpair(napi_env env, napi_callback_info info);
};

napi_value NapiBleController::constructor(napi_env env, napi_callback_info info){
    
}
