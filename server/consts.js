module.exports = {
  ARDUINO_MANUFACTURER_NAME: 'arduino',
  SENSORS_CMD: 'sensors -j',
  SENSORS_OUTPUT_PARSING_FAILED_ERROR_MESSAGE: 'Error occurred while trying to parse sensors output.',
  SENSORS_DEVICE_NOT_DETECTED_ERROR_MESSAGE_CREATOR: {
    construct: ({ device }) => `No device "${device || "<device>"}" was detected.`
  }
}
