const {Employee} = require('../models/employee');
const randomName = require('node-random-name');

const emplSize = 20;
const defaultEmployee = {
    name: "",
    nodeAddress: "",
    company: ""
};

const fillEmployees = async () => {
    let empl = JSON.parse(JSON.stringify(defaultEmployee));

    // create and save several employee from the same company
    empl.company = "DemonstrationCorp"
    for (let i = 1; i < emplSize; i++) {
        empl.nodeAddress = "address_" + i;
        empl.name        = randomName();
        emplToSave       = new Employee(empl);  
        await emplToSave.save(); 
    }

    // // add employees for an other company
    // empl.company = "HBKU"
    // for (let i = 1; i < emplSize; i++) {
    //     empl.nodeAddress = "address_" + i;
    //     empl.name        = randomName();
    //     emplToSave       = new Employee(empl);  
    //     await emplToSave.save(); 
    // }
}

exports.fillEmployees = fillEmployees; 