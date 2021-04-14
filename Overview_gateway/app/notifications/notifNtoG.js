const request = require('request-promise');
const config = require("config");
const {getCredentials} = require('../credentials/credentials');
const tsFormat = require('../common/timeserieFormat');

const serverUrl = config.get("serverUrl");

class NotifNtoG {
    
    constructor() { }
    
    postNotif(notif) {
        let backendUri = `${serverUrl}/notifications/fromGateway`;
       
        let options = {
            method: 'POST',
            uri: backendUri,
            body: { notif },
            headers: { Authorization: getCredentials() },
            json: true // Automatically stringifies the body to JSON
        };
        
        request(options)
            .then(() => {})
            .catch(err => {
                console.log(err)
            });
    }

    addNotif(data, command) {
        let notif = this.parseNotifData(data, command);
        
        this.postNotif(notif);
        console.log(notif);
    }

    parseNotifData(data, command) {
        let notif = {
            notifMessage: command,
            nodeAddress : data.slice(
                tsFormat.nodeAddress,
                tsFormat.skinTemperature)
        };
        return notif;
    }
}

exports.NotifNtoG = NotifNtoG;
