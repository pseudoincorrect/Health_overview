const jwt = require('jsonwebtoken');
var mongoose = require('mongoose');

function getToken(company, userType) {
    
    let id = mongoose.Types.ObjectId();

    const token  = jwt.sign(
        {
            _id: id,
            company: company,
            userType: userType
        },
        process.env.JWTPRIVATEKEY
    );
    return token;
}

exports.getToken = getToken;