#include "aidexxcontroller.h"
#include "../../ble.h"
#include "../../constant/cgm/aidexxoperation.h"
#include "../../util/byteutils.h"
#include "../../util/md5.h"
#include "../../util/encryptor/aesencryptor.h"
#include "../../devcomm/CLibrary/lib_checksum.h"

AidexXController::AidexXController() : BleController() {
    initialize();
}

AidexXController::AidexXController(const BleControllerInfo &info) : BleController() {
    initialize();
    setInfo(info);
}

AidexXController::~AidexXController() {
    delete defaultParam;
}

void AidexXController::initialize() {
    type = DEV_TYPE_CGM_X;
    authenticated = true;
#if ENABLE_ENCRYPTION
    authenticated = false;
#endif
    frameEnable = false;
    acknowledgement = false;
    autoDisconnect = false;

    defaultParam = new DefaultParam(this);
}

void AidexXController::setSn(const string &sn) {
    BleController::setSn(sn);
    if (!sn.size())
        return;

    int snLen = (int) sn.length();
    uint8 snChar[snLen];
    for (int i = 0; i < snLen; i++)
        snChar[i] = *(sn.data() + i);
    ByteUtils::snToBytes((char *) snChar, snLen);

    uint8 snChar1[snLen];
    uint8 snChar2[snLen];
    for (int i = 0; i < snLen; i++) {
        snChar1[i] = snChar[i] * 13 + 61;
        snChar2[i] = snChar[i] * 17 + 19;
    }
    MD5::digest(snChar1, snLen, (unsigned char *) snSecret1);
    MD5::digest(snChar2, snLen, (unsigned char *) snSecret2);
}

bool AidexXController::startEncryption(const uint8 *key) {
    if (sn.empty())
        return false;
    if (accessKey.empty())
        return false;
    DevComm::select(getDevCommType());
    DevComm::getInstance()->getEncryptor()->setIv(snSecret2);
    DevComm::getInstance()->getEncryptor()->setKey(accessKey.data());
    uint8 newKey[KEY_LENGTH + 1];
    memcpy(newKey, key, KEY_LENGTH + 1);
    DevComm::getInstance()->getEncryptor()->decrypt(newKey, KEY_LENGTH + 1);
    if (newKey[KEY_LENGTH] != LibChecksum_GetChecksum8Bit(newKey, KEY_LENGTH))
        return false;
    DevComm::getInstance()->getEncryptor()->setKey(newKey);

#if ENABLE_ENCRYPTION
    authenticated = true;
#endif
    return true;
}

uint16 AidexXController::pair() {
    if (getSecret() == NULL) {
        return BleOperation::BUSY;
    }

    return BleController::pair();
}

uint16 AidexXController::unpair() {
    if (sendCommand(OP_DELETE_BOND, 0, 0)) {
        return AidexXOperation::DELETE_BOND;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::keepConnect() {
    if (sendCommand(OP_KEEP_LONG_CONNECT, 0, 0)) {
        return AidexXOperation::KEEP_LONG_CONNECT;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::getDeviceInfo() {
    if (sendCommand(OP_GET_DEVICE_INFO, 0, 0)) {
        return AidexXOperation::GET_DEVICE_INFO;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::getBroadcastData() {
    if (sendCommand(OP_GET_BRAODCAST_DATA, 0, 0)) {
        return AidexXOperation::GET_BROADCAST_DATA;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::newSensor(AidexXDatetimeEntity &datetime) {
    uint8 data[9];
    LittleEndianByteUtils::unsignedShortToBytes(datetime.year, data);
    data[2] = datetime.month;
    data[3] = datetime.day;
    data[4] = datetime.hour;
    data[5] = datetime.minute;
    data[6] = datetime.second;
    data[7] = datetime.timeZone;
    data[8] = datetime.dstOffset;

    if (sendCommand(OP_SET_START_TIME, data, 9)) {
        return AidexXOperation::SET_NEW_SENSOR;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::getStartTime() {
    if (sendCommand(OP_GET_START_TIME, 0, 0)) {
        return AidexXOperation::GET_START_TIME;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::getHistoryRange() {
    if (sendCommand(OP_GET_CGM_RECORD_RANGE, 0, 0)) {
        return AidexXOperation::GET_HISTORY_RANGE;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::getHistories(uint16 timeOffset) {
    uint8 data[2];
    LittleEndianByteUtils::unsignedShortToBytes(timeOffset, data);
    if (sendCommand(OP_GET_SG_RECORD, data, 2)) {
        return AidexXOperation::GET_HISTORIES;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::getRawHistories(uint16 timeOffset) {
    uint8 data[2];
    LittleEndianByteUtils::unsignedShortToBytes(timeOffset, data);
    if (sendCommand(OP_GET_RAW_RECORD, data, 2)) {
        return AidexXOperation::GET_HISTORIES_RAW;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::calibration(uint16 glucose, uint16 timeOffset) {
    uint8 data[4];
    LittleEndianByteUtils::unsignedShortToBytes(timeOffset, data);
    LittleEndianByteUtils::unsignedShortToBytes(glucose, data + 2);
    if (sendCommand(OP_CALIBRATION, data, 4)) {
        return AidexXOperation::SET_CALIBRATION;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::getCalibrationRange() {
    if (sendCommand(OP_GET_CALIBRATION_RECORD_RANGE, 0, 0)) {
        return AidexXOperation::GET_CALIBRATION_RANGE;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::getCalibration(uint16 index) {
    uint8 data[2];
    LittleEndianByteUtils::unsignedShortToBytes(index, data);
    if (sendCommand(OP_GET_CALIBRATION_RECORD, data, 2)) {
        return AidexXOperation::GET_CALIBRATION;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::getDefaultParamData() {
    return defaultParam->get();
}

uint16 AidexXController::setDefaultParamData(float32 value[]) {
    return defaultParam->set(42, value);
}

uint16 AidexXController::setDefaultParamData(const uint8 *data, uint16 length) {
//    uint8 crc = (uint8) data[length - 1];
//    if (crc != LibChecksum_GetChecksum8Bit((const uint8 *)data, length-1)) {
//        return BleOperation::BUSY;
//    }
//
//    length = length - 4;
    float32 value[length / 2];
    for (int i = 0; i < length / 2; i++) {
        int16 v = LittleEndianByteUtils::byteToShort((const uint8 *) data + 2 * i);
        value[i] = v / 100.0;
    }

    return defaultParam->set(length / 2, value);
}

uint16 AidexXController::getSensorCheck(uint8 index) {
    uint8 data[1];
    data[0] = index;
    if (sendCommand(OP_GET_SENSOR_CHECK, data, 1)) {
        return AidexXOperation::GET_SENSOR_CHECK;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::getAutoUpdateStatus() {
    if (sendCommand(OP_GET_AUTO_UPDATE_STATUS, 0, 0)) {
        return AidexXOperation::GET_AUTO_UPDATE_STATUS;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::setAutoUpdateStatus() {
    uint8 data[1];
    data[0] = 1;
    if (sendCommand(OP_SET_AUTO_UPDATE_STATUS, data, 1)) {
        return AidexXOperation::SET_AUTO_UPDATE_STATUS;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::setDynamicAdvMode(uint8 mode) {
    uint8 data[1];
    data[0] = mode;
    if (sendCommand(OP_SET_DYNAMIC_ADV_MODE, data, 1)) {
        return AidexXOperation::SET_DYNAMIC_ADV_MODE;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::reset() {
    if (sendCommand(OP_RESET, 0, 0)) {
        return AidexXOperation::RESET;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::shelfMode() {
    if (sendCommand(OP_SHELF_MODE, 0, 0)) {
        return AidexXOperation::SHELF_MODE;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::DeleteBond() {
    if (sendCommand(OP_DELETE_BOND, 0, 0)) {
        return AidexXOperation::DELETE_BOND;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::ClearStorage() {
    if (sendCommand(OP_CLEAR_STORAGE, 0, 0)) {
        return AidexXOperation::CLEAR_STORAGE;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::setGcBiasTrimming(uint16 value) {
    uint8 data[2];
    LittleEndianByteUtils::unsignedShortToBytes(value, data);
    if (sendCommand(OP_SET_GC_BIAS_TRIMMING, data, 2)) {
        return AidexXOperation::SET_GC_BIAS_TRIMMING;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::setGcImeasTrimming(int16 zero, uint16 scale) {
    uint8 data[4];
    LittleEndianByteUtils::shortToBytes(zero, data);
    LittleEndianByteUtils::unsignedShortToBytes(scale, data + 2);
    if (sendCommand(OP_SET_GC_IMEAS_TRIMMING, data, 4)) {
        return AidexXOperation::SET_GC_IMEAS_TRIMMING;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::getLogRange() {
    if (sendCommand(OP_GET_LOG_RANGE, 0, 0)) {
        return AidexXOperation::GET_LOG_RANGE;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::getLogs(uint16 index) {
    uint8 data[2];
    LittleEndianByteUtils::unsignedShortToBytes(index, data);
    if (sendCommand(OP_GET_LOG, data, 2)) {
        return AidexXOperation::GET_LOG;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::getErrorLogs() {
    if (sendCommand(OP_GET_ERROR_LOG, 0, 0)) {
        return AidexXOperation::GET_ERROR_LOG;
    } else {
        return BleOperation::BUSY;
    }
}

void AidexXController::setInfo(const BleControllerInfo &info) {
    BleController::setInfo(info);
    int size = (int) info.params.size();
    if (size > 0) {
        isProductionExpired = false;
        auto flag = (uint8) info.params[0];
        bleNativePaired = (flag & 0x01) > 0;
        aesInitialized = (flag & 0x02) > 0;
        status = info.status;
        calTemp = info.calTemp;
        isExpired = (status & 0x01) == 1 && (calTemp & 0x01) == 0;
        rssi = info.rssi;
        if (size >= 5) {
            uint16_t value = info.params[3] | (info.params[4] << 8);
            if (value != 0) {
                uint8 expireYear = (value >> 9) & 0x7F;
                uint8 expireMonth = (value >> 5) & 0x0F;
                uint8 expireDay = value & 0x1F;
                std::tm givenTime = {};
                givenTime.tm_year = (2000 + expireYear) - 1900;
                givenTime.tm_mon = expireMonth - 1;
                givenTime.tm_mday = expireDay;
                auto expireTime = std::chrono::system_clock::from_time_t(std::mktime(&givenTime));
                auto now = std::chrono::system_clock::now();
                isProductionExpired = expireTime < now;
            }
        }
    }
}

void AidexXController::setSendTimeout(int msec) {
    DevComm::getInstance()->setSendTimeout(msec);
}

void AidexXController::setRetryCount(int count) {
    DevComm::getInstance()->setRetryCount(count);
}

bool AidexXController::sendCommand(uint8 op, uint8 *data, uint16 length, bool instantly) {
    if (instantly)
        return DevComm::getInstance()->send(0xFF, 0, op, 0xFF, data, length);
    else
        return send(0xFF, op, 0xFF, data, length);
}

bool AidexXController::handleCommand(uint8 port, uint8 op, uint8 param, const uint8 *data, uint16 length) {
    if (port == 0xFF && op == 0xFF) {
        if (length == 1) {
            autoSending = true;
            onReceive(AidexXOperation::SET_SN, true, data, length);
            return true;
        }
        if (length == 2) {
            onReceive(AidexXOperation::PAIR, false, data, length);
            return false;
        }
        if (length == SECRET_LENGTH) {
            //TODO 如果全0则配对失败？？？
            autoSending = true;
            setKey((const char *) data);
            onReceive(BleOperation::PAIR, true, data, length);
            return true;
        }
        if (length == SECRET_LENGTH + 1) {
            autoSending = true;
            bool success = startEncryption(data);
            onReceive(BleOperation::BOND, success, data, length);
            return success;
        }
        if (length == 0) {
            onReceive(BleOperation::CONNECT, true, data, length);
            onReceive(BleOperation::PAIR, false, data, length);
            return false;
        }
    } else if (port == 0xFF && op == 0xFE) {
        onReceive(AidexXOperation::AUTO_UPDATE_FULL_HISTORY - INSTANT_MESSAGE_TYPE_HISTORY + data[0], true, data,
                  length);
        return true;
    }

    int aidexXOp = BleOperation::UNKNOWN;
    autoSending = false;

    if (op == defaultParam->setCode) return defaultParam->sendResp(data, length);
    else if (op == defaultParam->getCode) return defaultParam->queryResp(data, length);
    else if (op == OP_GET_DEVICE_INFO) aidexXOp = AidexXOperation::GET_DEVICE_INFO;
    else if (op == OP_GET_BRAODCAST_DATA) aidexXOp = AidexXOperation::GET_BROADCAST_DATA;
    else if (op == OP_SET_START_TIME) aidexXOp = AidexXOperation::SET_NEW_SENSOR;
    else if (op == OP_GET_START_TIME) aidexXOp = AidexXOperation::GET_START_TIME;
    else if (op == OP_GET_CGM_RECORD_RANGE) aidexXOp = AidexXOperation::GET_HISTORY_RANGE;
    else if (op == OP_GET_SG_RECORD) aidexXOp = AidexXOperation::GET_HISTORIES;
    else if (op == OP_GET_RAW_RECORD) aidexXOp = AidexXOperation::GET_HISTORIES_RAW;
    else if (op == OP_CALIBRATION) aidexXOp = AidexXOperation::SET_CALIBRATION;
    else if (op == OP_GET_CALIBRATION_RECORD_RANGE) aidexXOp = AidexXOperation::GET_CALIBRATION_RANGE;
    else if (op == OP_GET_CALIBRATION_RECORD) aidexXOp = AidexXOperation::GET_CALIBRATION;
    else if (op == OP_GET_SENSOR_CHECK) aidexXOp = AidexXOperation::GET_SENSOR_CHECK;
    else if (op == OP_GET_AUTO_UPDATE_STATUS) aidexXOp = AidexXOperation::GET_AUTO_UPDATE_STATUS;
    else if (op == OP_SET_AUTO_UPDATE_STATUS) aidexXOp = AidexXOperation::SET_AUTO_UPDATE_STATUS;
    else if (op == OP_SET_DYNAMIC_ADV_MODE) aidexXOp = AidexXOperation::SET_DYNAMIC_ADV_MODE;
    else if (op == OP_RESET) aidexXOp = AidexXOperation::RESET;
    else if (op == OP_SHELF_MODE) aidexXOp = AidexXOperation::SHELF_MODE;
    else if (op == OP_DELETE_BOND) aidexXOp = AidexXOperation::DELETE_BOND;
    else if (op == OP_CLEAR_STORAGE) aidexXOp = AidexXOperation::CLEAR_STORAGE;
    else if (op == OP_SET_GC_BIAS_TRIMMING) aidexXOp = AidexXOperation::SET_GC_BIAS_TRIMMING;
    else if (op == OP_SET_GC_IMEAS_TRIMMING) aidexXOp = AidexXOperation::SET_GC_IMEAS_TRIMMING;
    else if (op == OP_KEEP_LONG_CONNECT) aidexXOp = AidexXOperation::KEEP_LONG_CONNECT;
    else if (op == OP_GET_LOG_RANGE) aidexXOp = AidexXOperation::GET_LOG_RANGE;
    else if (op == OP_GET_LOG) aidexXOp = AidexXOperation::GET_LOG;
    else if (op == OP_GET_ERROR_LOG) aidexXOp = AidexXOperation::GET_ERROR_LOG;

    if (aidexXOp == AidexXOperation::DELETE_BOND) {
        std::vector<uint8_t> byteArray;
        for (int i = 0; data[i] != '\0'; ++i) {
            byteArray.push_back(data[i]);
        }
        if (autoDeleteBond) {
            //data中设置标志
            byteArray.push_back(0x01);
            autoDeleteBond = false;
        } else {
            byteArray.push_back(0x02);
        }
        const uint8 *newData = byteArray.data();
        setKey(NULL);
        autoSending = false;
        onReceive(aidexXOp, true, newData, byteArray.size());
        return true;
    }
    autoSending = true;
    onReceive(aidexXOp, true, data, length);
    return true;
}

void AidexXController::onReceive(uint16 op, bool success, const uint8 *data, uint16 length) {
    switch (op) {
        case BleOperation::CONNECT:
        case BleOperation::DISCONNECT: {
            authenticated = false;
        }
            break;
    }
    BleController::onReceive(op, success, data, length);
}

AidexXController::LongAttribute::LongAttribute(AidexXController *controller, uint8 maxNumber, uint8 setCode,
                                               uint8 getCode, uint16 setOp, uint16 getOp) {
    this->controller = controller;
    this->maxNumber = maxNumber;
    this->setCode = setCode;
    this->getCode = getCode;
    this->setOp = setOp;
    this->getOp = getOp;
    sendValue = (float32 *) malloc(maxNumber * sizeof(float32));
    sendBuffer = (uint8 *) malloc(maxNumber * sizeof(int16) + 2);
    queryBuffer = (uint8 *) malloc(maxNumber * sizeof(int16) + 1);
}

AidexXController::LongAttribute::~LongAttribute() {
    free(sendValue);
    free(sendBuffer);
    free(queryBuffer);
}

uint16 AidexXController::LongAttribute::set(uint8 number, float32 value[]) {
    if (number > maxNumber)
        return BleOperation::UNKNOWN;
    setNumber = number;
    sendIndex = 1;
    sendRespErrorCount = 0;
    for (uint8 i = 0; i < number; i++)
        sendValue[i] = value[i];
    if (send(false)) {
        return setOp;
    } else {
        return BleOperation::BUSY;
    }
}

uint16 AidexXController::LongAttribute::get() {
    queryIndex = 1;
    queryRespErrorCount = 0;
    if (query(false)) {
        return getOp;
    } else {
        return BleOperation::BUSY;
    }
}

bool AidexXController::LongAttribute::send(bool instantly) {
    sendBuffer[0] = setNumber;
    sendBuffer[1] = sendIndex;
    uint8 count = (controller->mtu - 5) / 2;
    uint8 i;
    for (i = 0; i < count; i++) {
        uint8 index = i + sendIndex - 1;
        if (index >= setNumber)
            break;
        float32 value = sendValue[index];
        if (value >= 0)
            value += 0.005;
        else
            value -= 0.005;
        LittleEndianByteUtils::shortToBytes((int16) (value * 100), sendBuffer + 2 + i * 2);
    }
    sendIndex += i - 1;
    return controller->sendCommand(setCode, sendBuffer, i * 2 + 2, instantly);
}

bool AidexXController::LongAttribute::sendResp(const uint8 *data, uint16 length) {
    if (length < 3)
        return false;
    uint8 resp = data[0];
    uint8 number = data[1];
    uint8 index = data[2];
    if (number != setNumber) {
        return false;
    }
    if (index != sendIndex) {
        sendRespErrorCount++;
        return sendRespErrorCount < MAX_RESP_ERROR_COUNT;
    }
    sendRespErrorCount = 0;
    if (resp == AidexXResponseCode::REFUSED || resp == AidexXResponseCode::TIME_OUT || index >= setNumber) {
        controller->autoSending = true;
        controller->onReceive(setOp, true, data, 1);
    } else {
        sendIndex++;
        send(true);
    }
    return true;
}

bool AidexXController::LongAttribute::query(bool instantly) {
    uint8 data = queryIndex;
    return controller->sendCommand(getCode, &data, 1, instantly);
}

bool AidexXController::LongAttribute::queryResp(const uint8 *data, uint16 length) {
    if (length < 5)
        return false;
    uint8 resp = data[0];
    uint8 number = data[1];
    uint8 index = data[2];
    uint8 count = (length - 3) / 2;
    if (resp == AidexXResponseCode::REFUSED || resp == AidexXResponseCode::TIME_OUT || number > maxNumber ||
        index == 0) {
        return false;
    }
    if (index != queryIndex) {
        queryRespErrorCount++;
        return queryRespErrorCount < MAX_RESP_ERROR_COUNT;
    }
    queryIndex = index + count;
    if (queryIndex > number + 1) {
        return false;
    }
    queryRespErrorCount = 0;
    if (index == 1)
        queryBuffer[0] = resp;
    ByteUtils::copy((char *) queryBuffer + (index - 1) * 2 + 1, (char *) data + 3, length - 3);
    if (queryIndex == number + 1) {
        controller->autoSending = true;
        controller->onReceive(getOp, true, queryBuffer, number * 2 + 1);
    } else {
        query(true);
    }
    return true;
}

AidexXController::DefaultParam::DefaultParam(AidexXController *controller) : LongAttribute(
        controller,
        AIDEXX_DEFAULT_PARAM_COUNT,
        OP_SET_DEFAULT_PARAM,
        OP_GET_DEFAULT_PARAM,
        AidexXOperation::SET_DEFAULT_PARAM,
        AidexXOperation::GET_DEFAULT_PARAM
) {
}

uint16 AidexXController::DefaultParam::set(uint8 number, float32 value[]) {
    return LongAttribute::set(number, value);
}
