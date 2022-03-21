const { fromEvent, map, buffer, filter, Subject, interval, from, mergeWith, take, mergeMap } = require('rxjs');
const { SerialPort } = require('serialport');
const { exec } = require('child_process')

async function createSerialPortInstance() {
  const ports = await SerialPort.list()
  const { path } = ports.find(({ manufacturer }) => manufacturer.toLowerCase().includes('arduino'))
  return new SerialPort({
    path,
    baudRate: 9600,
    autoOpen: false,
    endOnClose: true
  })
}

function createSerialPortObserver(serialPort) {
  const sub = fromEvent(serialPort, 'data')
  return sub.pipe(
    map(b => b.toString()),
    buffer(
      sub.pipe(
        filter(v => v.includes('\n'))
      )
    ),
    map(chunks => chunks.join('').slice(0, -2))
  )
}

async function cmd(cmd) {
  const cp = exec(cmd)
  const output = []
  cp.stdout.on('data', c => output.push(c))
  cp.stderr.on('data', c => output.push(c))
  return new Promise(res => {
    cp.on('exit', () => {
      res(output.join(''))
    })
  })
}

async function readSensor({ device, sensor, property }) {
  const rawOutput = await cmd('sensors -j')
  const resolvedProperty = property || `${sensor}_input`
  let parsed
  try {
    parsed = JSON.parse(rawOutput)
  } catch {
    throw new Error('Error occurred while trying to parse sensors output.')
  }
  if (!Reflect.has(parsed, device)) {
    throw new Error(`No device "${device}" was detected.`)
  }
  const value = parsed[device][sensor]?.[resolvedProperty]
  return {
    device,
    sensor,
    property: resolvedProperty,
    value
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