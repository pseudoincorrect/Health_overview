const Joi = require('joi');
const mongoose = require('mongoose');

const notificationSchema = new mongoose.Schema({
    nodeAddress: {
        type: String,
        minlength: 5,
        maxlength: 50,
        required: true
    },
    company: {
        type: String,
        required: true,
        minlength: 2,
        maxlength: 255
    },
    timeStamp : { 
        type : Number, 
        required: true,
    },
    date : { 
        type : Date, 
        default: Date.now
    },
    fromManager: {
        type: Boolean,
        required: true
    },
    read: {
        type: Boolean,
        default: false
    },
    notifMessage: {
        type: String,
        minlength: 3,
        maxlength: 50,
        required: true
    }
});

const Notification = mongoose.model('Notification', notificationSchema);

function validateNotification(notification) {
    const schema = {
        nodeAddress: Joi.string().min(3).max(50).required(),
        notifMessage: Joi.string().min(3).max(50).required()
    }
    return Joi.validate(notification, schema);
}

exports.Notification = Notification; 
exports.validate = validateNotification;
exports.notificationSchema = notificationSchema;
