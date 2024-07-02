#ifndef BleAdapter_H
#define BleAdapter_H

#include "ble.h"

class NativeBleAdapter : public Ble {
public:
    NativeBleAdapter() : Ble(){};

    using Ble::getBleState;
    using Ble::getCharacteristicUUID;
    using Ble::getPrivateCharacteristicUUID;
    using Ble::getServiceUUID;
    using Ble::onAdvertise;
    using Ble::onAdvertiseWithAndroidRawBytes;
    using Ble::onConnectFailure;
    using Ble::onConnectSuccess;
    using Ble::onDisconnected;
    using Ble::onReceiveData;
    using Ble::onScanRespond;
    using Ble::onCharacteristicNotifyEnable;
    using Ble::onF005NotExist;
    using Ble::isPairing;
    using Ble::setDiscoverTimeoutSeconds;
    using Ble::setExecuteConnectCallback;
    using Ble::setExecuteDisconnectCallback;
    using Ble::setExecuteReadCharacteristicCallback;
    using Ble::setExecuteStartScanCallBack;
    using Ble::setExecuteStopScanCallback;
    using Ble::setExecuteWriteCallback;
    using Ble::setExecuteWriteCharacteristicCallback;
    using Ble::setIsReadyConnectCallback;
    using Ble::setEnableCharacteristicNotifyCallback;

protected:
    void executeStartScan();

    void executeStopScan();

    bool isReadyToConnect(string mac);

    void executeConnect(string mac);

    void executeDisconnect();

    void executeWrite(const char *data, uint16 length);

    void executeWriteCharacteristic(uint16 uuid, const char *data, uint16 length);

    void executeReadCharacteristic(uint16 uuid);

    void executeEnableCharacteristicNotify(uint16 uuid);
};

#endif // BleAdapter_H
