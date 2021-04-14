const Joi = require('joi');
const mongoose = require('mongoose');

const timeSerieSchema = new mongoose.Schema({
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
    },
    timeStamp : { 
        type : Number, 
        required: true
    },
    date : { 
        type : Date, 
        default: Date.now
    },
    timeSerie: {
        type: Object,
        required: true
    }
});


const TimeSerie = mongoose.model('TimeSerie', timeSerieSchema);

function validateTimeSerie(timeSerie) {
    const schema = {
        nodeAddress: Joi.string().min(3).max(50).required(),
        timeSerie: Joi.object().required()
    }
    return Joi.validate(timeSerie, schema);
}


exports.TimeSerie = TimeSerie; 
exports.validate = validateTimeSerie;
exports.timeSerieSchema = timeSerieSchema;