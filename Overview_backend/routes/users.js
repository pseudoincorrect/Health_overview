const {User, validate} = require('../models/user');
const hasRole = require('../middleware/role');
const authorization = require('../middleware/authorization');
const express = require('express');
const router = express.Router();

router.get('/', [authorization, hasRole("admin")], async (req, res) => {
    const users = await User.find().sort('company');
    res.send(users);
});

router.get('/:id', [authorization, hasRole("admin")], async (req, res) => {
    const user = await User.findById(req.params.id);
  
    if (!user) return res.status(404).send(
            'The user with the given ID was not found.');
  
    res.send(user);
});

router.post('/', [authorization, hasRole("admin")], async (req, res) => {
    const { error } = validate(req.body);
    if(error) return res.status(400).send(error.details[0].message)
    
    let user = await User.findOne({email: req.body.email});
    if (user) return res.status(400).send('User already registered');
    
    user = new User(req.body);

    await user.hashPassword();

    await user.save();

    res.send(user);
})

router.delete('/:id', [authorization, hasRole("admin")], async (req, res) => {
    const user = await User.findByIdAndRemove(req.params.id);
    
    if (!user) return res.status(404).send(
            'The user with the given ID was not found.');
    
    res.send(user);
});

module.exports = router; 

/*
POST, GET at http://localhost:3000/api/users
PUT, GET, DELETE at http://localhost:3000/api/users/:id
{
  "name": "Thomas Fernadez",
  "email": "email@domain.com",
  "password": "ThisAClearPassword",
  "userType": "manager"
}
dont forget the token
*/
