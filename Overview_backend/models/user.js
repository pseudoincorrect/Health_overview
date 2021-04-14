const Joi = require('joi');
const mongoose = require('mongoose');
const jwt = require('jsonwebtoken');
const bcrypt = require('bcrypt');

const userSchema = new mongoose.Schema({
    name: {
        type: String,
        require: true,
        minlength: 3,
        maxlength: 255
    },
    email: {
        type: String,
        minlength: 3,
        maxlength: 50,
        unique: true,
        required: true
    },
    company: {
        type: String,
        minlength: 2,
        maxlength: 50,
        required: true
    },
    password: {
        type: String,
        minlength: 3,
        maxlength: 1024,
        required: true
    },
    userType: {
        type: String,
        minlength: 3,
        maxlength: 50,
        required: true
    }
});

userSchema.path('email').validate(function (email) {
    var emailRegex = /^([\w-\.]+@([\w-]+\.)+[\w-]{2,4})?$/;
    return emailRegex.test(email);
}, 'incorrect email address format');

userSchema.methods.generateAuthTokenManager = function () {
    const token = jwt.sign(
        {
            _id: this._id,
            company: this.company,
            userType: this.userType
        },
        process.env.JWTPRIVATEKEY);
    return token;
}

userSchema.methods.generateAuthTokenGateway = function () {
    const token = jwt.sign(
        {
            _id: this._id,
            company: this.company,
            userType: "gateway"
        },
        process.env.JWTPRIVATEKEY);
    return token;
}

const generateAuthTokenDemo = function () {
    const token = jwt.sign(
        {
            _id: "0000",
            company: "DemonstrationCorp",
            userType: "demonstration"
        },
        process.env.JWTPRIVATEKEY);
    return token;
}

userSchema.methods.hashPassword = async function () {
    const salt =  Number(process.env.SALTROUNDS);
    this.password = await bcrypt.hash(this.password, salt);
}

const User = mongoose.model('User', userSchema);

function validateUser(user) {
    const schema = {
        name: Joi.string().min(3).max(50).required(),
        password: Joi.string().min(3).max(50).required(),
        company: Joi.string().min(2).max(50).required(),
        userType: Joi.string().min(3).max(50).required(),
        email: Joi
            .string()
            .regex(/^([\w-\.]+@([\w-]+\.)+[\w-]{2,4})?$/)
            .required()
    }
    return Joi.validate(user, schema);
}

exports.User = User;
exports.validate = validateUser;
exports.userSchema = userSchema;
exports.generateAuthTokenDemo = generateAuthTokenDemo;