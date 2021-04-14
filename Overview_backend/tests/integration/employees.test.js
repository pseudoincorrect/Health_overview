const request = require('supertest');
const {Employee} = require('../../models/employee');
const {getToken} = require('./helper/fakeToken');

describe('employees', () => {

    let server;
    let savedAddress;

    const managerToken = getToken("company1", "manager");
    
    const emplSize = 6;
    const defaultEmployee = {
        name: "employee1",
        nodeAddress: "address1",
        company: "company1"
    };

    const saveSomeEmployees = async () => {
        let empl = JSON.parse(JSON.stringify(defaultEmployee));
        // create and save several employee from the same company
        for (let i = 0; i < emplSize; i++) {
            empl.nodeAddress = "address_" + i;
            empl.name = "employee_" + i;
            emplToSave = new Employee(empl);  
            await emplToSave.save(); 
            if (i === emplSize/2) savedAddress = empl.nodeAddress;
        }
        // change company 
        empl.name = "employee_0";
        empl.nodeAddress = "address_0"
        empl.company = "company2"
        emplToSave = new Employee(empl);  
        await emplToSave.save(); 
    }

    const execGetRequest = async(emplNodeAddress) => {
        return await request(server)
            .get('/api/employees/' + emplNodeAddress)
            .set('authorization', managerToken)
            .send();
    };

    const execPostRequest = async(postEmployee) => {
        return await request(server)
            .post('/api/employees/')
            .set('authorization', managerToken)
            .send(postEmployee);
    };

    const execDeleteRequest = async(deleteId) => {
        return await request(server)
            .delete('/api/employees/' + deleteId)
            .set('authorization', managerToken)
            .send();
    }

    beforeEach( async () => {
        server = require('../../index');
    });
    
    afterEach( async () => {
        await Employee.remove({});
        await server.close(); 
    });


    it('should return all employees of the company',
    async () => {
        await saveSomeEmployees();
        emplNodeAddress = "";

        const res = await execGetRequest("");

        const emplArray = res.body;
        let test = true;
        if (emplArray.length != emplSize) test = false;
        for (let i = 0; i < emplArray.length; i++) {
            if (emplArray[i].company != 'company1')
                test = false;
        }
        
        expect(res.status).toBe(200)
        expect(test).toBe(true);
    });


    it('should return the correct employees of the company',
    async () => {
        await saveSomeEmployees();

        const res = await execGetRequest(savedAddress);

        const emplArray = res.body;
        let test = true;
        if (  emplArray.length != 1 
            | emplArray[0].company != 'company1') 
            test = false;

        expect(res.status).toBe(200)
        expect(test).toBe(true);
    });


    it('should post a new employee it',
    async () => {
        let postEmployee = JSON.parse(JSON.stringify(defaultEmployee));
        delete postEmployee.company; 

        const res = await execPostRequest(postEmployee);

        const employee = await Employee.find({_id: res.body._id});

        expect(res.status).toBe(200)
        expect(employee[0]._id.toString()).toEqual(res.body._id);
    });


    it('should delete one employee',
    async () => {
        await saveSomeEmployees();

        const res = await execDeleteRequest(savedAddress);

        deleteEmployee = await Employee.find( { 
            $and: [
                { company : "company1" },
                { nodeAddress : savedAddress }
            ] 
        } );

        expect(res.status).toBe(200)
        expect(deleteEmployee).toEqual([]);
    });

});


