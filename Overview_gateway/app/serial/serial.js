const SerialPort = require('serialport');
const Delimiter = require('@serialport/parser-delimiter');
const fs = require('fs');
const EventEmitter = require('events');
const {sleep} = require('../common/sleep')

const delimiter = '\n'

class Serial extends EventEmitter {

    constructor() {
        super();
        this.portName = '';
        this.serial;
    }

    async getPort () {
        let files;
        let rxbuffer = '';
        this.portName = '';
        const ports = await SerialPort.list();

        for (const iterator of ports) {
            if(iterator.manufacturer == "SEGGER") {
                let path = iterator.comName
                this.serial = new SerialPort(path, { baudRate: 115200 });
                const parser = this.serial.pipe(new Delimiter({delimiter}))
                
                parser.on('error', this.serialError);
    
                parser.on('data', data => {
                    rxbuffer = data.toString('utf-8').trim();
                    if (rxbuffer === 'yes') {
                        this.portName = path;
                    }
                });
    
                await this.serial.write('z\r', this.serialError);
                await sleep(1000);
                this.serial.close();
                if (this.portName) { return true }
            }
        }
        return false;
    }

    async initSerialProcess () {
        this.serial = new SerialPort(this.portName, { baudRate: 115200 });
        const parser = this.serial.pipe(new Delimiter({delimiter}))
        
        parser.on('error', this.serialError);

        parser.on('data', rxData => {
            this.emit('rx', rxData.toString());
        });

        this.serial.on('close', async () => this.tryOpen());
    }

    writeCommand (command, address='', content) {
        this.serial.write(`${command}${address}${content}\r`, this.serialError);
    }

    serialError (error) {
        if (error) { return console.log('Error on write: ', error.message); }
    }

    async tryOpen () {
        let opened = false;
        console.log("NO gateway detected"); 
        while (! opened) {
            try {
                opened = await this.getPort();
            } catch (UnhandledPromiseRejectionWarning) {
                opened = false;
            }
        }
        await this.initSerialProcess();
        console.log("Nrf gateway detected on " + this.portName); 
        console.log("retrying to connect to it"); 
        
    }
        
}

exports.Serial = Serial;
