import AidexBleAdapter from '../ble/AidexBleAdapter';

@Entry
@Component
struct Index {
  @State message: string = 'Hello World';

  build() {
    Row() {
      Column() {
        Text(this.message)
          .fontSize(50)
          .fontWeight(FontWeight.Bold)
          .onClick(() => {
            AidexBleAdapter.getInstance().startBleScan()
          })
      }
      .width('100%')
    }
    .height('100%')
  }
}
