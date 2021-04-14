const request = require('supertest');
const {User} = require('../../models/user');
const {getToken} = require('./helper/fakeToken');

describe('users', () => {
    let server;
    let savedId;

    const adminToken = getToken("company1", "admin");
    const usrSize = 6;

    const defaultUser = {
        name: "userName",
        email: "email@domain.com",
        company: "company",
        password: "12345",
        userType: "manager"
    }

    const saveSomeUsers= async () => {
        let usrSaved, usrToSave;
        let usr = JSON.parse(JSON.stringify(defaultUser));
        // create and save several employees from the same company
        for (let i = 0; i < usrSize; i++) {
            usr.name = "user_" + i;
            usr.company = "company" + i;
            usr.email = "user_" + i + "@" + usr.company + ".com";
            usrToSave = new User(usr);  
            usrSaved =  await usrToSave.save(); 
            if (i === usrSize/2) savedId = usrSaved._id.toString();
        }
    }

    const execGetRequest = async(userId) => {
        return await request(server)
            .get('/api/users/' + userId)
            .set('authorization', adminToken)
            .send();
    };

    const execPostRequest = async(postUser) => {
        return await request(server)
            .post('/api/users')
            .set('authorization', adminToken)
            .send(postUser);
    }

    const execDeleteRequest = async(deleteId) => {
        return await request(server)
            .delete('/api/users/' + deleteId)
            .set('authorization', adminToken)
            .send();
    }

    beforeEach( async () => {
        server = require('../../index');
    })
    
    afterEach( async () => {
        await User.remove({});
        await server.close(); 
    })

    it('should get all users',
    async () => {
        await saveSomeUsers();

        const res = await execGetRequest("");
        
        expect(res.status).toBe(200);
        expect(res.body.length).toEqual(usrSize);
    })

    it('should get a particular users',
    async () => {
        await saveSomeUsers();

        const res = await execGetRequest(savedId);

        expect(res.status).toBe(200);
        expect(res.body._id.toString()).toEqual(savedId);
    })


    it('should post an user and find it',
    async () => {
        let postUser = JSON.parse(JSON.stringify(defaultUser));

        const res = await execPostRequest(postUser);

        const foundUser = await User.findById(res.body._id);
        
        expect(res.status).toBe(200);
        expect(foundUser._id.toString()).toEqual(res.body._id);
    })


    it('should delete one user',
    async () => {
        await saveSomeUsers();
        const beforeDeleteUser = await User.findById(savedId);

        const res = await execDeleteRequest(savedId);

        const deleteUser = await User.findById(savedId);        

        expect(res.status).toBe(200);
        expect(beforeDeleteUser).toBeInstanceOf(User);
        expect(deleteUser).toBe(null);
    })

})


