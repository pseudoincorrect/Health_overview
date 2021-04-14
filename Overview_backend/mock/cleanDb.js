const {Employee} = require('../models/employee');
const {TimeSerie} = require('../models/timeSerie');

const cleanDb = async () => {
    await TimeSerie.remove({company: "company1"});
    await TimeSerie.remove({company: "company2"});
    await TimeSerie.remove({company: "HBKU"});
    await Employee.remove({company: "company1"});
    await Employee.remove({company: "company2"});
    await Employee.remove({company: "HBKU"});
}

exports.cleanDb = cleanDb;