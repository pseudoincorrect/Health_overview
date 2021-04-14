const {User} = require('../../models/user');
const request = require('supertest');
const bcrypt = require('bcrypt');
const jwt = require('jsonwebtoken');

let server; 

describe('authorization middleware', () => {

    let user;
    let postEmail;
    let postPass;
    let defaultUserInfo = {
        name: "userName",
        email: "email@domain.com",
        company: "company",
        password: "12345",
        userType: "userType"
    }

    async function exec () {
        return await request(server)
            .post('/api/authentication')
            .send({ 
                email: postEmail,
                password: postPass                    
            });
    };
    
    beforeEach( async () => { 
        server = require('../../index'); 
        user = new User(defaultUserInfo);

        const salt =  Number(process.env.SALTROUNDS);
        user.password = await bcrypt.hash(user.password, salt);
        await user.save();

        token = user.generateAuthToken();
        postEmail =  defaultUserInfo.email;
        postPass = defaultUserInfo.password;
    });

    afterEach( async () => { 
        await User.remove({});
        await server.close(); 
    });

    
    it('should return a 400 error if email is invalid', async () => {
        postEmail = 'falseEmain@domain.com';
        const res = await exec();

        expect(res.status).toBe(400);
    });

    
    it('should return a 400 error if password is invalid', async () => {
        postPass = 'wrongPass';
        const res = await exec();

        expect(res.status).toBe(400);
    });

    
    it('should return a 200 error if token is valid', async () => {
        const res = await exec();
        
        expect(res.status).toBe(200);
    });

    
    it('should return a valid token in response', async () => {
        const res = await exec();
        
        const token = JSON.parse(res.text);
        const decoded = jwt.verify(token.token, process.env.JWTPRIVATEKEY);
        
        expect(decoded.company).toBe(defaultUserInfo.company);
    });
});