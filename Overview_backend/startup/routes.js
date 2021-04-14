const express = require('express');
const users = require('../routes/users');
const employees = require('../routes/employees');
const timeSeries = require('../routes/timeSeries');
const notifications = require('../routes/notifications');
const authentication = require('../routes/authentication');
const angular = require('../routes/angular');
const helloThere = require('../routes/helloThere');
const error = require('../middleware/error');

module.exports = function(app){
    app.use(express.json());
    app.use('/api/users', users);
    app.use('/api/employees', employees);
    app.use('/api/timeseries', timeSeries);
    app.use('/api/notifications', notifications);
    app.use('/api/authentication', authentication);
    app.use('/api/hellothere', helloThere);

    app.use(express.static(__dirname + '/../distAngular/dist'));
    app.use('/angular', angular);
    app.get('*',function (req, res) { res.redirect('/'); });
    app.use(error);
}

