module.exports = function HasRole(roles) {
    return function(req, res, next) {
        let hasRole = roles.includes(req.userType);
        if (!hasRole) {
            return res.status(403).send('Access Denied');
        }
        else {
            next();
        }
    }
  }