#ifndef BLE_H
#define BLE_H

#include "controller/blecontrollerinfo.h"
#include "devcomm/CLibrary/global.h"
#include "constant/globalconstants.h"

#include <string>
#include <vector>
#include <list>
#include <map>
#include <functional>

using namespace std;

class CTimer;
class BleController;
class Ble
{
public:
    static const uint8 DEVICE_TYPE = Comm::PARAM_PAIR_DEVICE_1;

    enum BleState {IDLE, SCANNING, CONNECTING, CONNECTED, DISCONNECTING};

    explicit Ble();
    virtual ~Ble();

protected:
    void setExecuteStartScanCallBack(std::function<void()> callback);
    void setExecuteStopScanCallback(std::function<void()> callback);
    void setExecuteConnectCallback(std::function<void(string)> callback);
    void setExecuteDisconnectCallback(std::function<void()> callback);
    void setIsReadyConnectCallback(std::function<bool(string)> callback);
    void setExecuteWriteCallback(std::function<void(const char *, uint16)> callback);
    void setExecuteWriteCharacteristicCallback(std::function<void(uint16, const char *, uint16)> callback);
    void setExecuteReadCharacteristicCallback(std::function<void(uint16)> callback);
    void setEnableCharacteristicNotifyCallback(std::function<void(uint16)> callback);

    // 服务UUID
    uint16 getServiceUUID();
    
    // 特征UUID
    uint16 getCharacteristicUUID();

    uint16 getCharacteristicUUIDF005();

    // 私有特征UUID
    uint16 getPrivateCharacteristicUUID();
    
    // 第一次连接前的扫描超时设置
    void setDiscoverTimeoutSeconds(uint16 seconds);
    
    BleState getBleState() { return state; };

    // 蓝牙状态发生变化时需调用以下函数
    void onScanRespond(string address, int32 rssi, const char *data, uint16 length);
    void onScanRespondDecoded(string address, string name, int32 rssi, const char *data, uint16 length);
    void onAdvertise(string address, int32 rssi, const char *data, uint16 length);
    void onAdvertiseDecoded(string address, string name, int32 rssi, const char *data, uint16 length);
    void onAdvertiseWithAndroidRawBytes(string address, int32 rssi, const char *data, uint16 length);
    void onConnectSuccess();
    void onConnectFailure();
    void onDisconnected();
    void onReceiveData(const char *data, uint16 length);
    void onReceiveData(uint16 uuid, const char *data, uint16 length);
    void onCharacteristicNotifyEnable(uint16 uuid);
    void onF005NotExist();
    bool isPairing();

private:
    friend class BleController;
    std::function<void()> executeStartScanCallback;
    std::function<void()> executeStopScanCallback;
    std::function<void(string)> executeConnectCallback;
    std::function<void()> executeDisconnectCallback;
    std::function<bool(string)> isReadyConnectCallback;
    std::function<void(const char *, uint16)> executeWriteCallback;
    std::function<void(uint16, const char *, uint16)> executeWriteCharacteristicCallback;
    std::function<void(uint16)> executeReadCharacteristicCallback;
    std::function<void(uint16)> executeEnableCharacteristicNotifyCallback;

    class ReceiveBuffer {
    public:
        int mMaxLength;
        int mPointer;
        int mIndex;
        vector<uint8> dataBuffer;
        vector<uint8> receiveData;

        ReceiveBuffer(int maxLength);
        bool push(const uint8 *data, uint16 length);
        void flush();
    };

    class BleCommand
    {
    public:
        BleCommand(uint8 port, uint8 op, uint8 param);
        BleCommand(uint8 port, uint8 op, uint8 param, const uint8 *data, uint16 length);
        uint8 port;
        uint8 op;
        uint8 param;
        vector<uint8> data;
    };

    uint32 discoverTimeoutSeconds;

    BleState state;
    bool connectWhenDiscovered;
    bool pairWhenConnected;

    BleController *controller;
    std::map<string, BleController*> controllers;

    ReceiveBuffer *buffer;
    CTimer *pSearchTimer;
    CTimer *pAckTimer;
    CTimer *pDisconTimer;
    CTimer *pOnDisconTimer;
    list<BleCommand> commandList;

    function<void(const BleControllerInfo &)> discoverCallback;

    Ble(const Ble&) = delete;
    Ble& operator=(const Ble&) = delete;
    
    void startScan();
    void stopScan();
    bool pair(BleController *controller);
    bool send(BleController *controller, uint8 port, uint8 op, uint8 param, const uint8 *data, uint16 length);

    bool isInConnection();
    bool isFoundCurrent(string address, string name, string sn);
	void connect(BleController *controller);
    void connect();
    void disconnect();
    void sendPairCommand();
    void sendBondCommand();

    void handleEvent(uint8 event);
    void handleCommand(uint8 port, uint8 op, uint8 param, const uint8 *data, uint16 length);
    void write(const uint8 *data, uint16 length);
    void write(uint16 uuid, const uint8 *data, uint16 length);
    void continueSending();
    void pairDenied();
};

#endif // BLE_H
