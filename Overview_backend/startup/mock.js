const {Employee} = require('../models/employee');
const {TimeSerie} = require('../models/timeSerie');
const {Notification} = require('../models/notification');
const {fillEmployees} = require('../mock/fillEmployees');
const {fillTimeSeries} = require('../mock/fillTimeSeries');

module.exports = async function (){
    await TimeSerie.remove({});
    await Employee.remove({});
    await Notification.remove({});
    await fillEmployees();
    await fillTimeSeries();
}