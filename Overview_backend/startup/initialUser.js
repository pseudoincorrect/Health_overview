const config = require('config');
const {User} = require('../models/user');

module.exports = async function (){
    let userData = config.get("firstUser");
    userData.password = process.env.FIRSTUSERPASSWORD;

    let user = await User.findOne({email: userData.email});
    if (!user) {
        user = new User(userData);
        await user.hashPassword();
        await user.save();
    }
}