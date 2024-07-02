import BleAdapter from "../../../ets/ble/BleAdapter";

type ExecuteScanCallback = () => void;
type ExecuteConnectCallback = (mac: string) => void;
type ExecuteDisconnectCallback = () => void;
type IsReadyConnectCallback = (mac: string) => void;
type ExecuteWriteCallback = (data: Uint8Array, length: number) => void;
type ExecuteWriteCharacteristicCallback = (uuid: number, data: Uint8Array, length: number) => void;
type ExecuteReadCharacteristicCallback = (uuid: number) => void;

export class NapiBleAdapter {
  constructor()

  setExecuteStartScanCallBack: (callback: ExecuteScanCallback) => void;

  setExecuteStopScanCallback: (callback: ExecuteScanCallback) => void;

  setExecuteConnectCallback: (callback: ExecuteConnectCallback) => void;

  setExecuteDisconnectCallback: (callback: ExecuteDisconnectCallback) => void;

  setIsReadyConnectCallback: (callback: IsReadyConnectCallback) => void;

  setExecuteWriteCallback: (callback: ExecuteWriteCallback) => void;

  setExecuteWriteCharacteristicCallback: (callback: ExecuteWriteCharacteristicCallback) => void;

  setExecuteReadCharacteristicCallback: (callback: ExecuteReadCharacteristicCallback) => void;

  getServiceUUID: () => number;

  getCharacteristicUUID: () => number;

  getPrivateCharacteristicUUID: () => number;

  setDiscoverTimeoutSeconds: (seconds: number) => void;

  onAdvertiseWithAndroidRawBytes: (address: String, rssi: number, data: Uint8Array) => void;

  onConnectSuccess: () => void;

  onConnectFailure: () => void;

  onDisconnected: () => void;

  onReceiveData: (data: Uint8Array) => void;

  onReceiveDataWithUuid: (uuid: number, data: Uint8Array) => void;
}

declare class BleControllerInfo {
  public type: number;
  public address: string;
  public name: string;
  public sn: string;
  public rssi: number;
  public params: Uint8Array;
}

type OnDiscoverCallback = (info: BleControllerInfo) => void;
type OnMessageCallback = (operation: number, success: boolean, data: Uint8Array) => void;

export class NapiBleController {
  constructor()

  setOnDiscoverCallback: (callback: OnDiscoverCallback) => void;

  setMessageCallback: (callback: OnMessageCallback) => void;

  setBleAdapter: (adapter: BleAdapter) => void;

  getMac: () => string;

  setMac: (mac: string) => void;

  getName: () => string;

  setName: (name: string) => void;

  getSn: () => string;

  setSn: (sn: string) => void;

  getHostAddress: () => string;

  setHostAddress: (address: Uint8Array) => void;

  getId: () => Uint8Array;

  setId: (id: Uint8Array) => void;

  getKey: () => Uint8Array;

  setKey: (key: Uint8Array) => void;

  getRssi: () => number;

  setSupportKeepConnect: (isSupport: boolean) => void;

  register: () => void;

  unregister: () => void;

  disconnect: () => void;

  pair: () => number;

  unpair: () => number;
}
