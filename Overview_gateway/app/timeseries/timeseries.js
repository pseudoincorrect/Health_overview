const request = require('request-promise');
const CircularBuffer = require("circular-buffer");
const config = require("config");
const {getCredentials} = require('../credentials/credentials');
const tsFormat = require('../common/timeserieFormat');

const serverUrl = config.get("serverUrl");

class Timeseries {
    
    constructor() {
        this.circBuff = new CircularBuffer(100);
        this.circBuff.buffSizeCheck = 2;
    }
    
    postTimeseries(timeSeries) {
        let backendUri = `${serverUrl}/timeseries`;
       
        let options = {
            method: 'POST',
            uri: backendUri,
            body: { timeSeries },
            headers: { Authorization: getCredentials() },
            json: true // Automatically stringifies the body to JSON
        };
        
        request(options)
            .then(() => {})
            .catch(err => {
                console.log(err)
            });
    }

    bufferCheck() {
        if (this.circBuff.size() >= this.circBuff.buffSizeCheck) {
            let timeSeries = [];
            for (let i = 0; i < this.circBuff.buffSizeCheck; i++) {
                let timeSerie = this.circBuff.deq();
                timeSeries.push(timeSerie);
            }
            this.postTimeseries(timeSeries);
        }
    }

    addSerie(data) {
        let timeSeries = this.parseTimeserieData(data);
        // console.log(timeSeries);
        this.circBuff.enq(timeSeries);
        this.bufferCheck();
    }

    parseTimeserieData(data) {
        let toInt16 = (a) => {
            a = parseInt(a, 16)
            a = ((a & 0x8000) > 0) ? a - 0x10000 : a;
            return a;
        }

        let temperature = toInt16(data.slice(
            tsFormat.temperature,
            tsFormat.heartRate))
        temperature = fixed_8_8_to_float(
            (temperature >> 8) & 0x000000FF, 
            temperature & 0x000000FF ) 
        
        let timeSerie = {
            nodeAddress : data.slice(
                tsFormat.nodeAddress,
                tsFormat.temperature),
            timeSerie : {
                temperature : temperature,
                heartRate : toInt16(data.slice(
                    tsFormat.heartRate,
                    tsFormat.oximetry)),
                oximetry : toInt16(data.slice(
                    tsFormat.oximetry,
                    tsFormat.humidity)),
                humidity : toInt16(data.slice(
                    tsFormat.humidity,
                    tsFormat.dehydration)),
                dehydration : toInt16(data.slice(
                    tsFormat.dehydration,
                    tsFormat.reserved1))
            }
        };
        return timeSerie;
    }
}

function fixed_8_8_to_float(integer, fract) {
    sum_fract = 0.0
    if (fract & 128) {sum_fract += 0.5}
    if (fract & 64)  {sum_fract += 0.25}
    if (fract & 32)  {sum_fract += 0.125}
    if (fract & 16)  {sum_fract += 0.062}
    if (fract & 8)   {sum_fract += 0.031}
    if (fract & 4)   {sum_fract += 0.015}
    return integer + sum_fract;
}

exports.Timeseries = Timeseries;
