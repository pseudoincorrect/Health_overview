const request = require('supertest');
const {Notification} = require('../../models/notification');
const {getToken} = require('./helper/fakeToken');

describe('notifications', () => {

    let server;
    let postNotification;
    let notifSaved;
    let notifTimeSaved;
    const managerToken = getToken("company1", "manager");
    const gatewayToken = getToken("company1", "gateway");

    let defaultNotification = {
        nodeAddress: "12345",
        company: "company1",
        fromManager: false,
        notifMessage: {
            notifType: "notifType"
        }
    }

    function getDateMs (strDate){
        let date = new Date(strDate);  
        return date.getTime();
    }

    const saveSomeNotif = async (fromManager) => {
        let getNotif = JSON.parse(JSON.stringify(defaultNotification));
        if (fromManager == "fromManager")
            getNotif.fromManager = true;
        for (let i = 0; i < 12; i++) {
            // node, same company
            getNotif.timeStamp = Date.now();
            if (i == 3) notifTimeSaved = getNotif.timeStamp;
            getNotif.nodeAddress = "address1"
            notifSaved = new Notification(getNotif);  
            await notifSaved.save(); 
            // other node, same company
            getNotif.timeStamp = Date.now();
            getNotif.nodeAddress = "address2"
            notifSaved = new Notification(getNotif);  
            await notifSaved.save(); 
        }
        // change company 
        getNotif.timeStamp = Date.now();
        getNotif.company = "company2"
        notifSaved = new Notification(getNotif);  
        await notifSaved.save(); 
    }

    const execGetFromGatewayRequest = async() => {
        return await request(server)
            .get('/api/notifications/fromGateway')
            .set('authorization', managerToken)
            .send();
    }

    const execGetFromManagerRequest = async() => {
        return await request(server)
            .get('/api/notifications/fromManager')
            .set('authorization', gatewayToken)
            .send();
    }

    const execPostFromGatewayRequest = async() => {
        return await request(server)
            .post('/api/notifications/fromGateway')
            .set('authorization', gatewayToken)
            .send(postNotification);
    }

    const execPostFromManagerRequest = async() => {
        return await request(server)
            .post('/api/notifications/fromManager')
            .set('authorization', managerToken)
            .send(postNotification);
    }

    beforeEach( async () => {
        server = require('../../index');
    })
    
    afterEach( async () => {
        await Notification.remove({});
        await server.close(); 
    })


    
    it('should get the correct notifications to the manager node',
    async () => {
        await saveSomeNotif("fromGateway");
        const res = await execGetFromGatewayRequest();

        const notifArray = res.body;
        let test = true;
        for (let i = 0; i < notifArray.length; i++) {
            let iDate = getDateMs(notifArray[i].timeStamp);
            if (  iDate < notifTimeSaved
                | notifArray[i].company != 'company1'
                | notifArray[i].fromManager != false )
                test = false;
        }
        
        expect(res.status).toBe(200)
        expect(test).toBe(true);
    })

    
    it('should get the correct notifications to the gateway node',
    async () => {
        await saveSomeNotif("fromManager");
        const res = await execGetFromManagerRequest();

        const notifArray = res.body;
        let test = true;
        for (let i = 0; i < notifArray.length; i++) {
            let iDate = getDateMs(notifArray[i].timeStamp);
            if (  iDate < notifTimeSaved
                | notifArray[i].company != 'company1'
                | notifArray[i].fromManager != true )
                test = false;
        }

        expect(res.status).toBe(200)
        expect(test).toBe(true);
    })

    
    it('should let the manager node post a notification and find it',
    async () => {
        postNotification = JSON.parse(JSON.stringify(defaultNotification));
        delete postNotification.company;

        const res = await execPostFromGatewayRequest();
        
        const findNotif = await Notification.findById(res.body._id);   

        expect(res.status).toBe(200)
        expect(findNotif._id.toString()).toEqual(res.body._id);
    })

    
    it('should let the gateway node post a notification and find it',
    async () => {
        postNotification = JSON.parse(JSON.stringify(defaultNotification));
        delete postNotification.company;
        postNotification.fromManager = false;

        const res = await execPostFromGatewayRequest();
        
        const findNotif = await Notification.findById(res.body._id);

        expect(res.status).toBe(200)
        expect(findNotif._id.toString()).toEqual(res.body._id);
    })

})
 

