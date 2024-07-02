import { NapiBleAdapter } from 'libblecommon.so/Index';

export default abstract class BleAdapter {
  protected bleAdapter: NapiBleAdapter;

  constructor() {
    this.bleAdapter = new NapiBleAdapter();
    this.bleAdapter.setExecuteStartScanCallBack(() => {
      this.executeStartScan();
    });
    this.bleAdapter.setExecuteStopScanCallback(() => {
      this.executeStopScan();
    });
    this.bleAdapter.setExecuteConnectCallback((mac: string) => {
      this.executeConnect(mac);
    });
    this.bleAdapter.setExecuteDisconnectCallback(() => {
      this.executeDisconnect();
    });
    this.bleAdapter.setIsReadyConnectCallback((mac: string) => {
      return this.isReadyConnect(mac);
    });
    this.bleAdapter.setExecuteWriteCallback((data: Uint8Array, length: number) => {
      this.executeWrite(data, length);
    });
    this.bleAdapter.setExecuteWriteCharacteristicCallback((uuid: number, data: Uint8Array, length: number) => {
      this.executeWriteCharacteristic(uuid, data, length);
    });
    this.bleAdapter.setExecuteReadCharacteristicCallback((uuid: number) => {
      this.executeReadCharacteristic(uuid);
    });
  }

  protected getServiceUUID(): number {
    return this.bleAdapter.getServiceUUID();
  }

  protected getCharacteristicUUID(): number {
    return this.bleAdapter.getCharacteristicUUID();
  }

  protected getPrivateCharacteristicUUID(): number {
    return this.bleAdapter.getPrivateCharacteristicUUID();
  }

  protected setDiscoverTimeoutSeconds(seconds: number) {
    this.bleAdapter.setDiscoverTimeoutSeconds(seconds);
  }

  protected onAdvertiseWithAndroidRawBytes(address: String, rssi: number, data) {
    this.bleAdapter.onAdvertiseWithAndroidRawBytes(address, rssi, data)
  }

  protected onConnectSuccess() {
    this.bleAdapter.onConnectSuccess();
  }

  protected onConnectFailure() {
    this.bleAdapter.onConnectFailure();
  }

  protected onDisconnected() {
    this.bleAdapter.onDisconnected();
  }

  protected onReceiveData(data: Uint8Array) {
    this.bleAdapter.onReceiveData(data);
  }

  protected onReceiveDataWithUuid(uuid: number, data: Uint8Array) {
    this.bleAdapter.onReceiveDataWithUuid(uuid, data);
  }

  protected abstract executeStartScan();

  protected abstract executeStopScan();

  protected abstract executeConnect(mac: string);

  protected abstract executeDisconnect();

  protected abstract isReadyConnect(mac: string): boolean;

  protected abstract executeWrite(data: Uint8Array, length: number);

  protected abstract executeWriteCharacteristic(uuid: number, data: Uint8Array, length: number);

  protected abstract executeReadCharacteristic(uuid: number);
}