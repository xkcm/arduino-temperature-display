
const { promisify } = require('util');
const { createSerialPortObserver, createSerialPortInstance, createHardwareSensorObserver } = require('./utils');

async function main() {
  const arduinoSerialPort = await createSerialPortInstance()
  await promisify(arduinoSerialPort.open.bind(arduinoSerialPort))() // Wait for open port

  console.log(`Opened serial port at ${arduinoSerialPort.path}`)

  const arduinoData = createSerialPortObserver(arduinoSerialPort)
  arduinoData.subscribe(console.log) // Read serial data
  
  const tempObserver = createHardwareSensorObserver({
    intervalMs: 1000,
    sensorConfig: {
      device: 'nouveau-pci-0100',
      sensor: 'temp1'
    }
  })
  tempObserver.subscribe(console.log)
  tempObserver.subscribe(({ value }) => {
    arduinoSerialPort.write(`${value.toFixed(1)}C`)
    arduinoSerialPort.write('\n')
  })
}

main()
