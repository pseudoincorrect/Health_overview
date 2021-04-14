const {TimeSerie, validate} = require('../models/timeSerie');
const authorization = require('../middleware/authorization');
const hasRole = require('../middleware/role')
const express = require('express');
const router = express.Router();

router.get('/', 
    [authorization, hasRole(["manager","demonstration"])], async (req, res) => {
    const company = req.company;
    const nodeAddress = req.query.nodeAddress;
    const timeLow = Number(req.query.timeLow);
    let timeSerie;

    if (!nodeAddress) {
        if (! timeLow) {
            // get last time serie for each employee
            timeSerie = await TimeSerie.aggregate([
                { $match: { company :  company }}, 
                { $group : {
                    _id : "$nodeAddress", 
                    timeSerie : { $last : "$timeSerie" },
                    timeStamp : { $last : "$timeStamp" }
                }},
            ]);
            for (let i = 0; i < timeSerie.length; i++) {
                if (timeSerie[i]._id) {
                    timeSerie[i].nodeAddress = timeSerie[i]._id
                    delete timeSerie[i]._id; 
                }
            }
        }
        else {
            // get last 1000 timeseries
            timeSerie = await TimeSerie

            .find( 
                { $and: [
                    { company: { "$eq":  company }},
                    { timeStamp: { "$gte": timeLow } } ] 
                },
                null,
                { sort: { timeStamp : -1 } })
            .limit(1000)
        }
    }
    else {
        // get last 250 timeseries for an employee
        timeSerie = await TimeSerie
        .find( 
            { $and: [
                { company: company },
                { nodeAddress: nodeAddress },
                { timeStamp: { "$gte": timeLow } } ] 
            }, 
            null, 
            { sort: { timeStamp : -1 } } )
        .limit(250)
    }
    
    if (!timeSerie) return res.status(404).send(
            'No timeSerie for the given company was found.');

    res.send(timeSerie);
});

router.post('/', [authorization, hasRole("gateway")], async (req, res) => {
    let validation;
    
    req.body.timeSeries.every(element => {
            let {error} = validate(element);
            validation = error;
    });
    
    if(validation) { 
        return res.status(400).send(validation.details[0].message);
    }

    req.body.timeSeries.every(element => {
        element.company = req.company;
    });
    
    for (const timeserie of req.body.timeSeries) {
        timeserie.company = req.company;
        timeserie.timeStamp =  Math.floor(Date.now()); 
        timeSerie = new TimeSerie(timeserie);
        await timeSerie.save();
    }
    
    res.send(timeSerie);
})

router.delete('/:id', [authorization, hasRole("admin")], async (req, res) => {
    const timeSerie = await TimeSerie.findByIdAndRemove(req.params.id);
    
    if (!timeSerie) return res.status(404).send(
            'The timeSerie with the given ID was not found.');
    
    res.send(timeSerie);
});

module.exports = router; 

/*
POST, GET at http://localhost:3000/api/employess
PUT, GET, DELETE at http://localhost:3000/api/employess/:id
{
  "nodeAddress": "12345",
  "timeSeries": {
      "temperature": "23"
      "humidity": "55"
  }
}
dont forget the token
*/
