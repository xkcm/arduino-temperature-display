const { fromEvent, map, buffer, filter, interval, mergeWith, mergeMap, firstValueFrom } = require('rxjs');
const { SerialPort } = require('serialport');
const { exec } = require('child_process');
const {
  SENSORS_CMD, SENSORS_OUTPUT_PARSING_FAILED_ERROR_MESSAGE,
  SENSORS_DEVICE_NOT_DETECTED_ERROR_MESSAGE_CREATOR, ARDUINO_MANUFACTURER_NAME
} = require('./consts');

async function createSerialPortInstance() {
  const ports = await SerialPort.list()
  const { path } = ports.find(
    ({ manufacturer }) => manufacturer.toLowerCase().includes(ARDUINO_MANUFACTURER_NAME)
  )
  return new SerialPort({
    path,
    baudRate: 9600,
    autoOpen: false
  })
}

function createSerialPortObserver(serialPort) {
  const serialData = fromEvent(serialPort, 'data')
  return serialData.pipe(
    map(bufferData => bufferData.toString()),
    buffer(serialData.pipe(
      filter(chunk => chunk.includes('\n'))
    )),
    map(chunks => chunks.join('').trimEnd())
  )
}

async function cmd(command) {
  const childProcess = exec(command)
  const stdoutStream = fromEvent(childProcess.stdout, 'data')
  const stderrStream = fromEvent(childProcess.stderr, 'data')
  const outputStream = stdoutStream.pipe(
    mergeWith(stderrStream),
    buffer(fromEvent(childProcess, 'exit')),
    map(chunks => chunks.join(''))
  )
  return firstValueFrom(outputStream)
}

async function readSensor({ device, sensor, property }) {
  const rawOutput = await cmd(SENSORS_CMD)
  const resolvedProperty = property || `${sensor}_input`
  let parsedOutput
  try {
    parsedOutput = JSON.parse(rawOutput)
  } catch {
    throw new Error(SENSORS_OUTPUT_PARSING_FAILED_ERROR_MESSAGE)
  }
  if (!Reflect.has(parsedOutput, device)) {
    throw new Error(SENSORS_DEVICE_NOT_DETECTED_ERROR_MESSAGE_CREATOR.construct({ device }))
  }
  const value = parsedOutput[device][sensor]?.[resolvedProperty]
  return {
    device,
    sensor,
    property: resolvedProperty,
    value,
    timestamp: Date.now()
  }
}

function createHardwareSensorObserver({ sensorConfig, intervalMs = 1000 }) {
  return interval(intervalMs).pipe(
    mergeMap(() => readSensor(sensorConfig))
  )
}

module.exports = {
  createSerialPortObserver,
  createHardwareSensorObserver,
  createSerialPortInstance
}