const {Serial} = require('./app/serial/serial');
const {readCredentials, getCredentials} = require('./app/credentials/credentials');
const {NotifGtoN} = require('./app/notifications/notifGtoN');
const {NotifNtoG} = require('./app/notifications/notifNtoG');
const {Timeseries} = require('./app/timeseries/timeseries');
const {DataMocker} = require('./app/mock/mockData');
const {NotifMocker} = require('./app/mock/mockNotif');
const commands = require('./app/common/commands');
const config = require("config");
const {sleep} = require('./app/common/sleep')

let serial;
let notifGtoN;
let timeseries;
let dataMocker;

async function main () {
	console.log(config.get("serverUrl"));
	
    serial = new Serial();
    notifGtoN = new NotifGtoN();
    notifNtoG = new NotifNtoG();
    timeseries = new Timeseries();
    dataMocker = new DataMocker();
    notifMocker = new NotifMocker();

    console.log(config.get("serverUrl"));
    // console.log(config.get("cred"));

    // need to modify getCredentials()
    // while (getCredentials() == undefined) {
    //     await readCredentials();
    // }

    await notifGtoN.initNotifGtoN();

    while (! await startSerial(serial)) ;

    // setInterval(function() { mockTimeserie(dataMocker); }, 100);
    // setInterval(function() { mockNotif(notifMocker); }, 8000); 
}


async function startSerial(serial) {
    if (await serial.getPort()) {
        console.log("Nrf gateway detected on " + serial.portName); 
        serial.initSerialProcess();
        serial.on('rx', (rxData) => processRxData(rxData));
        notifGtoN.on('notif', () => notifGtoN.proceedNotif(serial)); 
        return true;
    }
    else {
        await sleep(1000);
        console.log("NO gateway detected"); 
        console.log("retrying to connect"); 
    }
    return false;
}

function processRxData (rxData) {
    console.log(new Date(), rxData);
    
    if( getCommand(rxData) === commands.N_G_SENSORS.charCodeAt(0)) {
        timeseries.addSerie(rxData.slice(4, rxData.length));
    }
    else if (getCommand(rxData) === commands.N_G_NOTIF.charCodeAt(0)) {
        notifNtoG.addNotif(rxData.slice(4, rxData.length), "N_G_NOTIF");
    }
}

function getCommand (rxData) {
    return parseInt(rxData.slice(0, commands.commandSize), 16);
}

function mockTimeserie (dataMocker) {
    timeseries.postTimeseries([dataMocker.getOneMock()]);
}

function mockNotif (notifMocker) {
    notifNtoG.postNotif(notifMocker.getOneMock());
}

main();
