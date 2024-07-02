const baseUuid = '00000000-0000-1000-8000-00805F9B34FB';

export function toUuid(value: number) {
  let hexString = value.toString(16).toUpperCase();
  while (hexString.length < 4) {
    hexString = '0' + hexString;
  }
  const finalUuid = baseUuid.slice(0, 9) + hexString + baseUuid.slice(13);
  return finalUuid;
}

export function toIntBigEndian(uuid: string) {
  const hexString = uuid.toString().substring(4, 8);
  const intValue = parseInt(hexString, 16);
  return intValue;
}