import BleAdapter from './BleAdapter';
import bluetoothManager from '@ohos.bluetoothManager';
import BluetoothDeviceStore from './BleDeviceStore';
import { toIntBigEndian, toUuid } from './UuidUtil';
import HashMap from '@ohos.util.HashMap';
import systemDateTime from '@ohos.systemDateTime';
import constant from '@ohos.bluetooth.constant';
import ble from '@ohos.bluetooth.ble';

const DATA_PACKAGE_SIZE = 20;
const SEND_DATA_INTERVAL = 20;

export default class AidexBleAdapter extends BleAdapter {
  private lastSendTime = 0;
  private connectDevice: ble.GattClientDevice = null;
  private characteristicMap: HashMap<number, ble.BLECharacteristic> = new HashMap();

  private static instance: AidexBleAdapter;

  private constructor() {
    super();
  }

  public static getInstance(): AidexBleAdapter {
    if (!AidexBleAdapter.instance) {
      AidexBleAdapter.instance = new AidexBleAdapter();
    }
    return AidexBleAdapter.instance;
  }

  private onBLEDeviceFind(data: Array<ble.ScanResult>) {
    if (typeof (data) === 'undefined' || data.length <= 0) {
      return;
    }
    for (let result of data) {
      console.error("onBLEDeviceFind --> " + result.deviceName);
      this.onAdvertiseWithAndroidRawBytes(result.deviceId, result.rssi, result.data);
      BluetoothDeviceStore.getInstance().addDevice(result);
    }
  }

  public startBleScan() {
    this.executeStartScan();
  }

  protected executeStartScan() {
    try {
      ble.on("BLEDeviceFind", this.onBLEDeviceFind);
      console.error("BLE --> executeStartScan");
      ble.startBLEScan(
        [{
          serviceUuid: "0000181F-0000-1000-8000-00805F9B34FB"
        }],
        {
          interval: 500,
          dutyMode: ble.ScanDuty.SCAN_MODE_LOW_POWER,
          matchMode: ble.MatchMode.MATCH_MODE_AGGRESSIVE,
        }
      );
    } catch (err) {
      console.error("BLE --> executeStartScan errCode:" + err.code + ",errMessage:" + err.message);
    }
  }

  protected executeStopScan() {
    try {
      console.error("BLE --> executeStopScan");
      ble.stopBLEScan();
    } catch (err) {
      console.error("BLE --> executeStopScan errCode:" + err.code + ",errMessage:" + err.message);
    }
  }

  private connectStateChanged(connectState: ble.BLEConnectionChangeState) {
    switch (connectState.state) {
      case constant.ProfileConnectionState.STATE_DISCONNECTED:
        console.error("BLE --> STATE_DISCONNECTED");
        this.connectDevice.off('BLEConnectionStateChange');
        this.connectDevice.off('BLECharacteristicChange');
        this.connectDevice.close();
        this.onDisconnected();
        break;
      case constant.ProfileConnectionState.STATE_CONNECTING:
        console.error("BLE --> STATE_CONNECTING");
        break;
      case constant.ProfileConnectionState.STATE_CONNECTED:
        console.error("BLE --> STATE_CONNECTED");
        try {
          this.connectDevice.getServices().then(result => {
            this.characteristicMap.clear()
            for (let service of result) {
              let serviceUuid = service.serviceUuid
              if (serviceUuid == toUuid(this.getServiceUUID())) {
                for (let characteristic of service.characteristics) {
                  let characteristicUuid = characteristic.characteristicUuid
                  if (characteristicUuid == toUuid(this.getPrivateCharacteristicUUID())) {
                    this.enableNotify(characteristic);
                    this.characteristicMap.set(this.getPrivateCharacteristicUUID(), characteristic)
                  }
                  if (characteristicUuid == toUuid(this.getCharacteristicUUID())) {
                    this.enableNotify(characteristic);
                    this.characteristicMap.set(this.getCharacteristicUUID(), characteristic)
                  }
                }
                this.onConnectSuccess();
                break;
              }
            }
          });
        } catch (err) {
          this.onConnectFailure();
          console.error("BLE --> discoverService errCode:" + err.code + ",errMessage:" + err.message);
        }
        break;
      case bluetoothManager.ProfileConnectionState.STATE_DISCONNECTING:
        console.error("BLE --> STATE_DISCONNECTING");
        break;
      default:
        console.error("BLE --> Connect state change " + connectState.state);
        break;
    }
  }

  private enableNotify(characteristic: ble.BLECharacteristic) {
    this.connectDevice.setCharacteristicChangeNotification(characteristic, true);
    for (let desc of characteristic.descriptors) {
      desc.descriptorValue = new Uint8Array([0x01, 0x00]);
      this.connectDevice.writeDescriptorValue(desc);
    }
  }

  protected executeConnect(mac: string) {
    try {
      this.connectDevice = ble.createGattClientDevice(mac);
      this.connectDevice.on('BLEConnectionStateChange', this.connectStateChanged);
      this.connectDevice.on('BLECharacteristicChange', this.characteristicChange);
      this.connectDevice.connect();
    } catch (err) {
      this.onConnectFailure();
      console.error("BLE --> executeConnect errCode:" + err.code + ",errMessage:" + err.message);
    }
  }

  private characteristicChange(characteristic: ble.BLECharacteristic) {
    let characteristicUuid = characteristic.characteristicUuid;
    let value = new Uint8Array(characteristic.characteristicValue);
    this.onReceiveDataWithUuid(toIntBigEndian(characteristicUuid), value);
  }

  protected executeDisconnect() {
    try {
      this.connectDevice.disconnect();
    } catch (err) {
      console.error("BLE --> executeDisconnect errCode:" + err.code + ",errMessage:" + err.message);
    }
  }

  protected isReadyConnect(mac: string) {
    let isReady = BluetoothDeviceStore.getInstance().getDeviceMap().get(mac) != null;
    console.error("BLE --> isReadyConnect " + isReady);
    return isReady;
  }

  protected executeWrite(data: Uint8Array, length: number) {
    this.sendData(this.getCharacteristicUUID(), data, length);
  }

  protected executeWriteCharacteristic(uuid: number, data: Uint8Array, length: number) {
    this.sendData(uuid, data, length);
  }

  private sleep(ms: number): Promise<void> {
    return new Promise(resolve => setTimeout(resolve, ms));
  }

  private checkSendInterval() {
    try {
      systemDateTime.getRealTime().then(async (time) => {
        let interval = time - this.lastSendTime;
        if (interval < SEND_DATA_INTERVAL) {
          await this.sleep(SEND_DATA_INTERVAL - interval)
        }
      }).catch((error) => {
        console.info(`Failed to get real time. message:${error.message}, code:${error.code}`);
      });
    } catch (e) {
      console.info(`Failed to get real time. message:${e.message}, code:${e.code}`);
    }
  }

  private sendData(uuid: number, data: Uint8Array, length: number) {
    try {
      let characteristic = this.characteristicMap.get(uuid)
      if (characteristic == null || this.connectDevice == null) {
        console.error("BLE --> sendData fail, connectDevice or characteristic is null");
        return;
      }
      this.checkSendInterval()
      if (length <= DATA_PACKAGE_SIZE) {
        characteristic.characteristicValue = data;
        this.connectDevice.writeCharacteristicValue(characteristic, ble.GattWriteType.WRITE_NO_RESPONSE);
      } else {
        let pieces = length / DATA_PACKAGE_SIZE
        for (let index = 0; index <= pieces; index++) {
          var subArray;
          if (index < pieces) {
            subArray = data.subarray(index * DATA_PACKAGE_SIZE, index * DATA_PACKAGE_SIZE + DATA_PACKAGE_SIZE);
          } else {
            subArray = data.subarray(index * DATA_PACKAGE_SIZE, length);
          }
          this.sendData(uuid, subArray, subArray.length);
        }
      }
    } catch (err) {
      console.error("BLE --> sendData errCode:" + err.code + ",errMessage:" + err.message);
    }
  }

  protected executeReadCharacteristic(uuid: number) {
    if (this.connectDevice == null) {
      console.error("BLE --> executeReadCharacteristic this.connectDevice is null:");
      return;
    }
    if (this.characteristicMap.get(uuid) == null) {
      console.error("BLE --> executeReadCharacteristic characteristic is null:");
      return;
    }
    this.connectDevice.readCharacteristicValue(this.characteristicMap.get(uuid)).then(characteristic => {
      let value = new Uint8Array(characteristic.characteristicValue);
      this.onReceiveDataWithUuid(toIntBigEndian(characteristic.characteristicUuid), value);
    })
  }
}