const request = require('request-promise');
const CircularBuffer = require("circular-buffer");
const config = require("config");
const EventEmitter = require('events').EventEmitter;
const {getCredentials} = require('../credentials/credentials');
const commands = require('../common/commands');

const serverUrl = config.get("serverUrl");

class NotifGtoN extends EventEmitter {

    constructor (){
        super();
        this.notifBuffer = new CircularBuffer(100);
        this.notifBuffer.buffSizeCheck = 5;
    }

    async getNotifGtoN() {
        let backendUri = `${serverUrl}/notifications/frommanager`;
       
        let options = {
            method: 'GET',
            uri: backendUri,
            headers: { Authorization: getCredentials() },
        };
        
        return request(options)
            .catch(err => {
                console.log(err)
            });
    }

    initNotifGtoN () {
        setInterval( this.requestNotifs.bind(this), 3000);  
    }

    async requestNotifs () {
        let notifRes = await this.getNotifGtoN();
        let notifs = JSON.parse(notifRes);
        
        if(notifs.length) {
            this.bufferizeNotifs(notifs)
            this.emit('notif');
        }
    }
    
    bufferizeNotifs (notifs) {
        for (const notif of notifs) {
            
            console.log({
                nodeAddress: notif.nodeAddress,
                notifMessage: notif.notifMessage 
            });             
            
            this.notifBuffer.enq({
                nodeAddress: notif.nodeAddress,
                notifMessage: notif.notifMessage 
            });
        }
    }

    getOneNotif () {
        return this.notifBuffer.deq();
    }

    bufferCheck() {
        return this.notifBuffer.size();
    }

    proceedNotif (serial) {
        let notifNb = this.bufferCheck();
        if (notifNb) {
            for (let i = 0; i < notifNb; i++) {
                let notif = this.getOneNotif();
                if (notif.notifMessage == 'G_N_NOTIF') {
                    serial.writeCommand(commands.G_N_NOTIF, notif.nodeAddress, 'a')
                    console.log("we got a notif from the frontEnd");
                }
            }
        }
    }
}

exports.NotifGtoN = NotifGtoN;