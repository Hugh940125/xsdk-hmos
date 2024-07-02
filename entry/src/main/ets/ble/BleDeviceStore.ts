import HashMap from '@ohos.util.HashMap';
import ble from '@ohos.bluetooth.ble';

export default class BluetoothDeviceStore {
  private static instance: BluetoothDeviceStore;
  private deviceMap: HashMap<string, ble.ScanResult>;

  private constructor() {
    this.deviceMap = new HashMap();
  }

  public static getInstance(): BluetoothDeviceStore {
    if (!BluetoothDeviceStore.instance) {
      BluetoothDeviceStore.instance = new BluetoothDeviceStore();
    }
    return BluetoothDeviceStore.instance;
  }

  public addDevice(result: ble.ScanResult) {
    let deviceId = result.deviceId;
    this.deviceMap.set(deviceId, result);
  }

  public getDeviceMap() {
    return this.deviceMap;
  }
}