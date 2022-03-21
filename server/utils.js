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

function createArduinoObservable(serialPort) {
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

async function readGPUTemperature() {
  let output = await cmd('sensors')
  output = output.split('\n')
  const temp = output.splice(
    output.findIndex(line => line.toLowerCase().includes('pci adapter')),
    3
  ).slice(-1)[0].match(/temp1:\s+\+(\d+\.\d)°C/)[1]
  return temp+'C'
}

function createGPUTemperatureObserver(intervalMs = 1000) {
  return interval(intervalMs).pipe(
    mergeMap(() => readGPUTemperature())
  )
}

module.exports = {
  createArduinoObservable,
  createGPUTemperatureObserver,
  createSerialPortInstance
}