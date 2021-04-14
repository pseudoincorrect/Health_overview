const {TimeSerie} = require('../models/timeSerie');

let temperature;
let humidity;
let heartRate;
let date;
const empSize = 600;
const defaultTimeSerie = {
    company:     "",
    nodeAddress: "",
    timeStamp:   date,
    timeSerie:   { }
}

function getRandomInt(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
}

function initFakeData (){
    temperature = 30;  
    humidity    = 50;
    heartRate   = 90; 
}

function createFakeData (){
    const data = 
    {
        temperature: temperature,  
        humidity:    humidity,
        heartRate:   heartRate
    }
    temperature += getRandomInt(-1, 1);
    temperature = (temperature < 10) ? 10 : temperature
    temperature = (temperature > 50) ? 50 : temperature

    humidity += getRandomInt(-2, 2);
    humidity = (humidity < 0) ? 0 : humidity
    humidity = (humidity > 100) ? 100 : humidity
    
    heartRate += getRandomInt(-4, 4);
    heartRate = (heartRate < 60) ? 60 : heartRate
    heartRate = (heartRate > 140) ? 140 : heartRate
    return data;
}

const fillTimeSeries = async () => {
    let getSerie = JSON.parse(JSON.stringify(defaultTimeSerie));
    initFakeData();
    setInterval(
        function() { updateTimeseriesDemo(getSerie); }
        , 100
    );
}

async function updateTimeseriesDemo (getSerie) {
    // create fake time series randmly increasing
    getSerie.company = "DemonstrationCorp";
    getSerie.nodeAddress = "address_" + getRandomInt(1, 19);
    getSerie.timeStamp   = Date.now();
    getSerie.timeSerie   = createFakeData();
    timeSerieSaved       = new TimeSerie(getSerie);  
    await timeSerieSaved.save(); 
    
    // // save same serie with different company
    // getSerie.company = "HBKU";
    // timeSerieSaved       = new TimeSerie(getSerie);  
    // await timeSerieSaved.save(); 
}
exports.fillTimeSeries = fillTimeSeries; 
