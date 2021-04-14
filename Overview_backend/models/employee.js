const Joi = require('joi');
const mongoose = require('mongoose');

const employeeSchema = new mongoose.Schema({
    name: {
        type: String,
        require: true,
        minlength: 2,
        maxlength: 255
    },
    nodeAddress: {
        type: String,
        minlength: 5,
        maxlength: 50,
        required: true
    },
    company: {
        type: String,
        minlength: 2,
        maxlength: 50,
        required: true
    }
});


const Employee = mongoose.model('Employee', employeeSchema);

function validateEmployee(employee) {
    const schema = {
        name: Joi.string().min(2).max(50).required(),
        nodeAddress: Joi.string().min(2).max(50).required(),
    }
    return Joi.validate(employee, schema);
}


exports.Employee = Employee; 
exports.validate = validateEmployee;
exports.employeeSchema = employeeSchema;