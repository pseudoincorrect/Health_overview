const jwt = require('jsonwebtoken');

module.exports = function authorization(req, res, next){
    const token = req.header('authorization');
    if(!token) return res.status(401).send(
                            'Access denied. No token')

    try {
        const decoded = jwt.verify(
                    token, process.env.JWTPRIVATEKEY);
        req.userId = decoded._id;
        req.company = decoded.company;
        req.userType = decoded.userType;
        
        next();
    } catch (error) {
        return res.status(400).send('invalid token');
    }
}