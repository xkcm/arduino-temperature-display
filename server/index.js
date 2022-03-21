
const { promisify } = require('util');
const { createArduinoObservable, createSerialPortInstance, createGPUTemperatureObserver } = require('./utils');

async function main() {
  const arduinoSerialPort = await createSerialPortInstance()
  await promisify(arduinoSerialPort.open.bind(arduinoSerialPort))() // Wait for open port

  console.log(`Opened serial port at ${arduinoSerialPort.path}`)

  const arduinoData = createArduinoObservable(arduinoSerialPort)
  arduinoData.subscribe(console.log) // Read serial data
  
  const tempObserver = createGPUTemperatureObserver(5000)
  tempObserver.subscribe(temperature => {
    arduinoSerialPort.write(temperature)
    arduinoSerialPort.write('\n')
  })
}

main()
