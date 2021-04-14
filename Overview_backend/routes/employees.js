const {Employee, validate} = require('../models/employee');
const authorization = require('../middleware/authorization');
const hasRole = require('../middleware/role');
const express = require('express');
const router = express.Router();


router.get('/',
    [authorization, hasRole(["manager","demonstration"])], async (req, res) => {

    const company = req.company;
    const employees = await Employee.find({company : company}).sort('name');
    
    res.send(employees);
});


router.get('/:nodeAddress',
    [authorization, hasRole(["manager","demonstration"])], async (req, res) => {

    const company = req.company;
    
    const employee = await Employee.find( { 
        $and: [
            { company : company },
            { nodeAddress : req.params.nodeAddress }
        ] 
    } );
  
    if (!employee) return res.status(404).send(
            'The employee with the given ID was not found.');
  
    res.send(employee);
});


router.put('/',
    [authorization, hasRole("manager")], async (req, res) => {

    console.log("employee", req.body);
    
    const { error } = validate(req.body);
    if(error) return res.status(400).send(error.details[0].message)
    
    req.body.company = req.company;

    let queriedEmployee = await Employee.findOneAndUpdate(
        { 
            nodeAddress: req.body.nodeAddress 
        },
        req.body,
        { 
            upsert : true,
            new : true 
        }      
    );

    res.send(queriedEmployee);
});


router.delete('/:nodeAddress',
    [authorization, hasRole("manager")], async (req, res) => {

    const employee = await Employee.findOneAndRemove( { 
        $and: [
            { company : req.company },
            { nodeAddress : req.params.nodeAddress }
        ] 
    } );
    
    if (!employee) return res.status(404).send(
            'The employee with the given ID was not found.');
    
    res.send(employee);
});

router.put('/:nodeAddress',
    [authorization, hasRole("manager")], async (req, res) => {

    req.body.nodeAddress = req.params.nodeAddress;
    const { error } = validate(req.body);

    if(error) return res.status(400).send(error.details[0].message)

    const employee = await Employee.findOneAndUpdate( 
        {nodeAddress: req.params.nodeAddress}, 
        {name:req.body.name}, 
        {new: true}
    );
    
    if (!employee) return res.status(404).send(
            'The employee with the given ID was not found.');

    res.send(employee);
});


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
