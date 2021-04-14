const request = require('supertest');
const {TimeSerie} = require('../../models/timeSerie');
const {getToken} = require('./helper/fakeToken');

describe('timeSeries', () => {

    let server;
    let postRes;
    let timeSerieSaved;
    let postTimeSerie;
    let deleteId;
    let date;
    let getTimeStamp;
    let getNodeAddress;
    let queryGet;
    const adminToken = getToken("company1", "admin");
    const managerToken = getToken("company1", "manager");
    const gatewayToken = getToken("company1", "gateway");



    const empSize = 6;

    const defaultTimeSerie = {
        company: "company1",
        nodeAddress: "address1",
        timeStamp: date,
        timeSerie: {
            dataType1: 1,
            dataType2: 2
        }
    }

    const defaultPostTimeSerie = {
        nodeAddress: "address1",
        timeSerie: {
            dataType1: 1,
            dataType2: 2
        }
    }

    const execGetRequest = async() => {
        return await request(server)
            .get('/api/timeseries/')
            .set('authorization', managerToken)
            .query(queryGet)
            .timeout(50000)
            .send();
    }

    const execPostRequest = async() => {
        return await request(server)
            .post('/api/timeseries')
            .set('authorization', gatewayToken)
            .send(defaultPostTimeSerie);
    }

    const execDeleteRequest = async() => {
        return await request(server)
            .delete('/api/timeseries/' + deleteId)
            .set('authorization', adminToken)
            .send();
    }

    const saveSomeSeries = async () => {
        let getSerie = JSON.parse(JSON.stringify(defaultTimeSerie));
        for (let i = 0; i < empSize; i++) {
            // node, same company
            getSerie.nodeAddress = "address1"
            getSerie.timeStamp = Date.now();
            if (i === empSize/2) {
                getCompany = getSerie.company;
                getTimeStamp = getSerie.timeStamp;
                getNodeAddress = getSerie.nodeAddress;
            }
            timeSerieSaved = new TimeSerie(getSerie);  
            await timeSerieSaved.save(); 
            // other node, same company
            getSerie.timeStamp = Date.now();
            getSerie.nodeAddress = "address2"
            timeSerieSaved = new TimeSerie(getSerie);  
            await timeSerieSaved.save(); 
        }
        // change company 
        getSerie.timeStamp = Date.now();
        getSerie.company = "company2"
        getSerie.nodeAddress = "address3"
        timeSerieSaved = new TimeSerie(getSerie);  
        await timeSerieSaved.save(); 
    }

    beforeEach( async () => {
        server = await require('../../index');
        date = Date.now();
        timeSerieSaved = new TimeSerie(defaultTimeSerie);        
        await timeSerieSaved.save();
    })
    
    afterEach( async () => {
        await TimeSerie.remove({});
        await server.close(); 
    })

    
    it('should GET the latest posted timeserie for each node of a company',
    async () => {
        await saveSomeSeries();

        const res = await execGetRequest();
        
        // ensure only we don't get unwanted series
        let successTest = true;

        if (res.body.length != 2) successTest = false;  
        function isIn(serie) { 
            if (serie.nodeAddress == "address1") return false;
            else if (serie.nodeAddress == "address2") return false;
            else return true;
        }
        if (res.body.find(isIn)) successTest = false;

        expect(successTest).toBe(true);
    })

    
    it('should GET the timeseries for a time interval for each node of a company',
    async () => {
        await saveSomeSeries();
        queryGet = {
            timeLow : getTimeStamp
        };
        const res = await execGetRequest();
        
        // ensure only we don't get unwanted series
        let successTest = true;

        
        if (res.body.length == 2) successTest = false;  
        function isIn(serie) { 
            if (serie.nodeAddress == "address1") return false;
            else if (serie.nodeAddress == "address2") return false;
            else return true;
        }
        if (res.body.find(isIn)) successTest = false;

        expect(successTest).toBe(true);
    })

    
    it('should GET the timeseries for a time interval for a single node of a company',
    async () => {
        await saveSomeSeries();
        queryGet = {
            nodeAddress : getNodeAddress,
            timeLow : getTimeStamp
        };

        const res = await execGetRequest();
        
        // ensure we don't get unwanted series
        let successTest = true;
        function isIn(serie) { 
            let date = new Date(serie.timeStamp);  
            let dateMs = date.getTime();
            if (serie.nodeAddress != getNodeAddress) { return true; }
            else if (dateMs < getTimeStamp) { return true; }           
            else { return false; }
        }
        if (res.body.find(isIn)) successTest = false;

        expect(successTest).toBe(true);
    })

    
    it('should return 200 if everything is OK with POST',
    async () => {    
        postRes = await execPostRequest();
        
        expect(postRes.status).toBe(200);
    })

    
    it('should POST a timeserie and find it',
    async () => {
        postRes = await execPostRequest();
        postTimeSerie = await TimeSerie.find({_id: postRes.body._id})

        expect(postTimeSerie[0].str).toEqual(postRes.body.str);
    })

    
    it('should delete one timeSerie',
    async () => {
        deleteId = timeSerieSaved._id;

        const res = await execDeleteRequest();

        const deleteTimeSerie = await TimeSerie.find({});
        
        expect(deleteTimeSerie).toEqual([]);
    })
})


