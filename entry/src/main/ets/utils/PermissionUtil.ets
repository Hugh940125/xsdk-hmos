import { abilityAccessCtrl, bundleManager, common, Want } from '@kit.AbilityKit'
import { Permissions } from '@ohos.abilityAccessCtrl';
import { BusinessError } from '@kit.BasicServicesKit';

async function checkAccessToken(permission: Permissions): Promise<abilityAccessCtrl.GrantStatus> {
  let atManager = abilityAccessCtrl.createAtManager()
  let grantStatus = abilityAccessCtrl.GrantStatus.PERMISSION_DENIED
  let tokenId = 0
  try {
    let bundleInfo: bundleManager.BundleInfo =
      await bundleManager.getBundleInfoForSelf(bundleManager.BundleFlag.GET_BUNDLE_INFO_WITH_APPLICATION);
    let appInfo: bundleManager.ApplicationInfo = bundleInfo.appInfo;
    tokenId = appInfo.accessTokenId;
  } catch (error) {
    const err: BusinessError = error as BusinessError;
    console.error(`Failed to get bundle info for self. Code is ${err.code}, message is ${err.message}`);
  }
  try {
    grantStatus = await atManager.checkAccessToken(tokenId, permission);
  } catch (error) {
    const err: BusinessError = error as BusinessError;
    console.error(`Failed to check access token. Code is ${err.code}, message is ${err.message}`);
  }
  return grantStatus;
}

export interface PermissionCheckCallback {
  onSuccess: () => void;
  onFail: (permissions: Array<Permissions>) => void;
}
export async function checkPermissions(permissions: Array<Permissions>, callback: PermissionCheckCallback): Promise<void> {
  let isGrant = true;
  let permissionsDenied: Array<Permissions> = [];
  for (let index = 0; index < permissions.length; index++) {
    const permission = permissions[index];
    let grantStatus: abilityAccessCtrl.GrantStatus = await checkAccessToken(permission);
    if (grantStatus === abilityAccessCtrl.GrantStatus.PERMISSION_DENIED) {
      isGrant = false
      permissionsDenied.push(permission);
    }
  }
  if (isGrant) {
    callback.onSuccess();
  } else {
    callback.onFail(permissionsDenied)
  }
}

export type PermissionRequestCallback = () => void
export function requestPermissions(context: common.UIAbilityContext, permissions: Array<Permissions>,
  callback: PermissionRequestCallback): void {
  let atManager: abilityAccessCtrl.AtManager = abilityAccessCtrl.createAtManager();
  atManager.requestPermissionsFromUser(context, permissions).then((data) => {
    let grantStatus: Array<number> = data.authResults;
    let permissionsDenied: Array<Permissions> = [];
    let length: number = grantStatus.length;
    let isGrant = true;
    for (let i = 0; i < length; i++) {
      if (grantStatus[i] != 0) {
        isGrant = false
        permissionsDenied.push(permissions[i])
      }
    }
    if (isGrant) {
      callback();
    }else{
      //show dialog
    }
  }).catch((err: BusinessError) => {
    console.error(`Failed to request permissions from user. Code is ${err.code}, message is ${err.message}`);
  })
}

function openPermissionsInSystemSettings(context: common.UIAbilityContext): void {
  let wantInfo: Want = {
    bundleName: 'com.huawei.hmos.settings',
    abilityName: 'com.huawei.hmos.settings.MainAbility',
    uri: 'application_info_entry',
    parameters: {
      pushParams: 'com.example.myapplication' // 打开指定应用的详情页面
    }
  }
  context.startAbility(wantInfo).then(() => {
    // ...
  }).catch((err: BusinessError) => {
    // ...
  })
}

