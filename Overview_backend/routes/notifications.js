const {Notification, validate} = require('../models/notification');
const authorization = require('../middleware/authorization');
const hasRole = require('../middleware/role');
const express = require('express');
const router = express.Router();

router.get('/fromGateway', 
        [authorization, hasRole("manager")], async (req, res) => {
    const company = req.company;

    const notifications = await Notification.find( 
        { $and: [
            { company: company },
            { fromManager: false },
            { read : false} // for future use
        ] }, 
        null,
        { sort: { timeStamp : -1 } })
        .limit(5)
    
    if (!notifications) return res.status(404).send(
            'No notifications for the given company was found.');
    
    res.send(notifications);

    for (const notification of notifications) {
        await Notification.remove({fromManager: false })
        // await Notification.findByIdAndRemove(notification._id);
    }
});

router.get('/fromManager', 
        [authorization, hasRole("gateway")], async (req, res) => {
    const company = req.company;

    const notifications = await Notification.find( 
        { $and: [
            { company: company },
            { fromManager: true },
            { read : false} // for future use
        ] }, 
        null,
        { sort: { timeStamp : -1 } })
        .limit(5)
    
    res.send(notifications);

    for (const notification of notifications) {
        await Notification.remove({fromManager: true })
        // await Notification.findByIdAndRemove(notification._id);
    }
});

router.post('/fromManager', [authorization, hasRole("manager")],async (req, res) => {
    const { error } = validate(req.body);
    if(error) return res.status(400).send(error.details[0].message)

    const notif = {
        nodeAddress :req.body.nodeAddress,
        company : req.company,
        timeStamp :  Math.floor(Date.now()),
        notifMessage : req.body.notifMessage,
        fromManager : true
    }
    const notification = new Notification(notif);

    try {
        err = await notification.save(); 
    } catch (error) {
        console.log(error);
    }

    res.send(notification);
})

router.post('/fromGateway', [authorization, hasRole("gateway")], async (req, res) => {
    const { error } = validate(req.body.notif);
    if(error) {
        console.log(error.details[0].message);
        return res.status(400).send(error.details[0].message)
    }

    // console.log("got a valid notif");
    const notif = {
        nodeAddress :req.body.notif.nodeAddress,
        company : req.company,
        timeStamp :  Math.floor(Date.now()),
        notifMessage : req.body.notif.notifMessage,
        fromManager : false
    }

    const notification = new Notification(notif);

    await notification.save();

    // console.log(notification);

    res.send(notification);
})

module.exports = router; 

/*
POST, GET at http://localhost:3000/api/employess
PUT, GET, DELETE at http://localhost:3000/api/employess/:id
{
  "name": "Thomas Fernadez",
  "nodeAddress": "12345",
}
dont forget the token
*/
