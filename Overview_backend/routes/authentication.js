const Joi = require('joi');
const _ = require('lodash');
const bcrypt = require('bcrypt');
const {User, generateAuthTokenDemo} = require('../models/user');
const express = require('express');
const router = express.Router();
const authorization = require('../middleware/authorization');
const hasRole = require('../middleware/role');

router.post('/', async (req, res) => {
	const { error } = validate(req.body);
	if (error) 
		return res.status(400).send(error.details[0].message);

	let user = await User.findOne({email: req.body.email});
	if (!user) 
		return res.status(400).send('Invalid email or password');

	const validPassword = await bcrypt.compare(
		req.body.password, user.password);
	if (!validPassword) 
		return res.status(400).send('Invalid email or password');

	const token = user.generateAuthTokenManager();
	
	res.send({token});
});

router.get('/gatewaytoken', [authorization, hasRole("manager")], async (req, res) => {
	let user = await User.findById(req.userId);
	if (!user) 
		return res.status(400).send('Invalid email or password');
	let gatewayToken = user.generateAuthTokenGateway();

	res.send({gatewayToken});
});

router.get('/demonstration', async (req, res) => {
    const demoToken = generateAuthTokenDemo();
	res.send({demoToken});
});

function validate(userLogin) {
	const schema = {
		email: Joi
			.string()
			.regex(/^([\w-\.]+@([\w-]+\.)+[\w-]{2,4})?$/)
			.required(),
		password: Joi.string().min(3).max(50).required()
	};
	return Joi.validate(userLogin, schema);
}

module.exports = router;

/*
POST http://localhost:3500/api/authentication
{
	"email": "email@domain.com",
	"password": "ThisAClearPassword"
}
*/
