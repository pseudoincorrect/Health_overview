const {Employee} = require('../models/employee');
const {TimeSerie} = require('../models/timeSerie');
const {fillEmployees} = require('./fillEmployees');
const {fillTimeSeries} = require('./fillTimeSeries');
const {cleanDb} = require('./cleanDb');

require('../startup/db')();
require('../startup/logging')();

const fillDb = async () => {
    await fillEmployees();
    await fillTimeSeries();
}

const runProcess = async () => {
    console.log("cleaning Db");
    
    await cleanDb();
    await fillDb();
    winston.info(`database filled with good stuffs`);
    await process.exit();
}

runProcess();

// JWT
// {
//     "user": "1234567890",
//     "company": "company_1",
//     "userType": "manager"
// }
// eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VyIjoiMTIzNDU2Nzg5MCIsImNvbXBhbnkiOiJjb21wYW55XzEiLCJ1c2VyVHlwZSI6Im1hbmFnZXIifQ.HGqjInR_it51-bm5413ybIJE73zq-Gp6pRW-0IDjICs