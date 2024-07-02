export default abstract class DeviceModel {
  isSensorExpired: Boolean = false;
  targetBriefIndex = 0;
  targetCalIndex = 0;
  nextBriefIndex = 0;
  nextRawIndex = 0;
  nextCalIndex = 0;
  latestAdvertTime = 0;
  lastRapidIncreaseAlertTime = 0;
  lastRapidDecreaseAlertTime = 0;
  glucose: number = -1;
  lastHistoryTime: Date = null
  controller: BleController = null
  var isHistoryValid: Boolean = false
  var malFunctionList = mutableListOf<Int>()
  var glucoseLevel: GlucoseLevel? = null
  var glucoseTrend: GlucoseTrend? = null
  var latestHistory: AidexXHistoryEntity? = null
  var isGettingTransmitterData = false
  var alert: ((time: String, type: Int) -> Unit)? = null
  var onCalibrationPermitChange: ((state:CalibrationState) -> Unit)? = null
  var minutesAgo: Int? = null

  private set get() {
            field = if (lastHistoryTime == null) {
                null
            }

  else {
  (TimeUtils.currentTimeMillis - lastHistoryTime!!.time).millisToMinutes()
}

return field
}

}