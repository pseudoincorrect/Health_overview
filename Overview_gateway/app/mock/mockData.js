
const defaultTimeSerie = {
    nodeAddress: "",
    timeSerie:   { }
}

// mockDataTimer = setInterval( 
//     function() { createFakeData(circularBuffer, dataMocker); }, 
//     1000);

class DataMocker {
    constructor () {
        this.address = 0;
        this.temperature = 0;
        this.humidity = 0;
        this.heartRate = 0;
        this.date = Date.now();

        this.initMockData();
    }

    initMockData () {
        this.address = 0;
        this.temperature = 30;  
        this.temperatureSign = 1;
        this.humidity    = 50;
        this.heartRate   = 90; 
    }

    createMockData (){
        const data = {
            temperature: this.temperature,  
            humidity:    this.humidity,
            heartRate:   this.heartRate
        }
        if (this.address == 0) {
            if (this.temperatureSign > 0) {
                this.temperature += 1;
                if (this.temperature > 50)
                    this.temperatureSign = -1;
            } else {
                this.temperature -= 1;
                if (this.temperature < 0)
                    this.temperatureSign = 1;            
            }
        }

        this.temperature += getRandomInt(-1, 1);
        this.temperature = (this.temperature < 10) ? 10 : this.temperature
        this.temperature = (this.temperature > 50) ? 50 : this.temperature
    
        this.humidity += getRandomInt(-2, 2);
        this.humidity = (this.humidity < 0) ? 0 : this.humidity
        this.humidity = (this.humidity > 100) ? 100 : this.humidity
        
        this.heartRate += getRandomInt(-4, 4);
        this.heartRate = (this.heartRate < 60) ? 60 : this.heartRate
        this.heartRate = (this.heartRate > 140) ? 140 : this.heartRate

        return data;
    }

    getOneMock () {
        let getSerie = JSON.parse(JSON.stringify(defaultTimeSerie));
        // create Mock time series randmly increasing

        getSerie.nodeAddress = "address_" + this.pad(this.address, 3);
        this.address = this.address + 1
        if (this.address > 5)
            this.address = 0;

        getSerie.timeSerie   = this.createMockData();
        return getSerie;
    }

    pad(num, size) {
        var s = "000000000" + num;
        return s.substr(s.length-size);
    }

}

function getRandomInt(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
}

exports.DataMocker = DataMocker; 
