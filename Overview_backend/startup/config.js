const config = require('config');

module.exports = function (){
    if ( !process.env.JWTPRIVATEKEY ||
         !process.env.FIRSTUSERPASSWORD ) {
       throw new Error('FATAL ERROR: jwtPrivateKey or firstUserPassword not defined');
    }
}