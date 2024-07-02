#ifndef AIDEXXCONTROLLER_H
#define AIDEXXCONTROLLER_H

#define OP_GET_DEVICE_INFO                  0x10
#define OP_GET_BRAODCAST_DATA               0x11

#define OP_SET_START_TIME                   0x20
#define OP_GET_START_TIME                   0x21
#define OP_GET_CGM_RECORD_RANGE             0x22
#define OP_GET_SG_RECORD                    0x23
#define OP_GET_RAW_RECORD                   0x24
#define OP_CALIBRATION                      0x25
#define OP_GET_CALIBRATION_RECORD_RANGE     0x26
#define OP_GET_CALIBRATION_RECORD           0x27

#define OP_SET_DEFAULT_PARAM                0x30
#define OP_GET_DEFAULT_PARAM                0x31
#define OP_GET_SENSOR_CHECK                 0x32

#define OP_GET_AUTO_UPDATE_STATUS           0x33
#define OP_SET_AUTO_UPDATE_STATUS           0x34
#define OP_SET_DYNAMIC_ADV_MODE             0x35
#define OP_KEEP_LONG_CONNECT                0x41

#define OP_GET_LOG_RANGE                    0xE0
#define OP_GET_LOG                          0xE1
#define OP_GET_ERROR_LOG                    0xE2

#define OP_RESET                            0xF0
#define OP_SHELF_MODE                       0xF1
#define OP_DELETE_BOND                      0xF2
#define OP_CLEAR_STORAGE                    0xF3
#define OP_SET_GC_BIAS_TRIMMING             0xF4
#define OP_SET_GC_IMEAS_TRIMMING            0xF5

#define AIDEXX_DEFAULT_PARAM_COUNT  0xFF

#include "../../devcomm/CLibrary/global.h"
#include "../../devcomm/devcommclass.h"
#include "../blecontroller.h"
#include "../../constant/cgm/aidexxconstants.h"
#include "../../parser/cgm/aidexxentities.h"

class Ble;

class AidexXController : public BleController {
public:
    static const uint16 SERVICE_UUID = 0x181F;
    static const uint16 CHARACTERISTIC_UUID = 0xF002;
    static const uint16 PRIVATE_CHARACTERISTIC_UUID = 0xF001;
    static const uint16 PRIVATE_CHARACTERISTIC_UUID_2 = 0xF003;
    static const uint16 CHARACTERISTIC_UUID_F005 = 0xF005;
    static const uint HOST_ADDRESS_LENGTH = 6;
    static const uint SECRET_LENGTH = 16;
    static const uint KEY_LENGTH = 16;

    explicit AidexXController();

    explicit AidexXController(const BleControllerInfo &info);

    ~AidexXController();

    bool isBleNativePaired() { return bleNativePaired; }

    bool isProductExpired() {
        return isProductionExpired && (status & 0x01) == 1 && (calTemp & 0x01) == 1;
    }

    bool isSensorExpired() { return isExpired; }

    bool getRssi() { return rssi; }

    bool isAesInitialized() { return aesInitialized; }

    uint8 getStatus() {
        return status;
    }

    uint8 getCalTemp() {
        return calTemp;
    }

    // getter
    const uint8 *getSecret() const override { return sn.size() ? snSecret1 : NULL; }

    bool isPaired() const override { return accessKey.size(); }

    // setter
    void setSn(const string &sn) override;

    bool startEncryption(const uint8 *key);

    uint16 pair() override;

    uint16 unpair() override;

    // CGM命令
    uint16 getDeviceInfo();

    uint16 getBroadcastData();

    uint16 newSensor(AidexXDatetimeEntity &datetime);

    uint16 getStartTime();

    uint16 getHistoryRange();

    uint16 getHistories(uint16 timeOffset);

    uint16 getRawHistories(uint16 timeOffset);

    uint16 calibration(uint16 glucose, uint16 timeOffset);

    uint16 getCalibrationRange();

    uint16 getCalibration(uint16 index);

    uint16 getDefaultParamData();

    uint16 setDefaultParamData(float32 value[]);

    uint16 setDefaultParamData(const uint8 *data, uint16 length);

    uint16 getSensorCheck(uint8 index);

    uint16 getAutoUpdateStatus();

    uint16 setAutoUpdateStatus();

    uint16 setDynamicAdvMode(uint8 mode);

    uint16 reset();

    uint16 shelfMode();

    uint16 DeleteBond();

    uint16 keepConnect() override;

    uint16 ClearStorage();

    uint16 setGcBiasTrimming(uint16 value);

    uint16 setGcImeasTrimming(int16 zero, uint16 scale);

    uint16 getLogRange();

    uint16 getLogs(uint16 index);

    uint16 getErrorLogs();

protected:
    uint8 snSecret1[SECRET_LENGTH] = {0};
    uint8 snSecret2[SECRET_LENGTH] = {0};

    uint16 getServiceUUID() const override { return SERVICE_UUID; }

    uint16 getCharacteristicUUID() const override { return CHARACTERISTIC_UUID; }

    uint16 getCharacteristicUUIDF005() const override { return CHARACTERISTIC_UUID_F005; }

    uint16 getPrivateCharacteristicUUID() const override {
        return isPairing ? PRIVATE_CHARACTERISTIC_UUID : PRIVATE_CHARACTERISTIC_UUID_2;
    };

    uint8 getPacketLength() const override { return 0; }

    uint getCommPort() const override { return 0xFF; }

    uint getHostAddressLength() const override { return HOST_ADDRESS_LENGTH; }

    uint getIdLength() const override { return 0; }

    uint getKeyLength() const override { return KEY_LENGTH; }

    uint getDevCommType() const override { return DEV_COMM_TYPE_1; }

    bool isEncryptionEnabled() const override { return false; }

    bool isBufferEnabled() const override { return false; }

    bool isAuthenticated() const { return authenticated; }

    bool isFrameEnabled() const { return frameEnable; }

    bool isAcknowledgement() const { return acknowledgement; }

    void setInfo(const BleControllerInfo &info) override;

    void setSendTimeout(int msec);

    void setRetryCount(int count);

    bool sendCommand(uint8 op, uint8 *data, uint16 length, bool instantly = false) override;

    bool handleCommand(uint8 port, uint8 op, uint8 param, const uint8 *data, uint16 length) override;

    void onReceive(uint16 op, bool success, const uint8 *data = 0, uint16 length = 0) override;

private:
    void initialize();

    class LongAttribute {
    public:
        LongAttribute(AidexXController *controller, uint8 maxNumber, uint8 setCode, uint8 getCode, uint16 setOp,
                      uint16 getOp);

        ~LongAttribute();

        uint16 set(uint8 number, float32 value[]);

        uint16 get();

    private:
        friend class AidexXController;

        AidexXController *controller;
        uint8 maxNumber;
        uint8 setCode;
        uint8 getCode;
        uint16 setOp;
        uint16 getOp;

        uint8 setNumber;
        uint8 sendIndex;
        float32 *sendValue;
        uint8 *sendBuffer;

        uint8 queryIndex;
        uint8 *queryBuffer;

        const uint8 MAX_RESP_ERROR_COUNT = 3;
        uint8 sendRespErrorCount;
        uint8 queryRespErrorCount;

        bool send(bool instantly);

        bool sendResp(const uint8 *data, uint16 length);

        bool query(bool instantly);

        bool queryResp(const uint8 *data, uint16 length);
    };

    class DefaultParam : public LongAttribute {
    public:
        DefaultParam(AidexXController *controller);

        uint16 set(uint8 number, float32 value[]);
    };

    DefaultParam *defaultParam;

    bool bleNativePaired;
    bool aesInitialized;
    bool isProductionExpired;
    bool isExpired;
    uint8 status;
    uint8 calTemp;
    int32 rssi;
};

#endif // AIDEXXCONTROLLER_H
