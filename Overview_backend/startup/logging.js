winston = require('winston');
require('winston-mongodb');
require('express-async-errors');

module.exports = function (){
    winston.exceptions.handle( 
    new winston.transports.Console({colorize: true, prettyPrint: true}),
    new winston.transports.File({filename: "exception.log"})
    );

    process.on('unhandledRejection', (ex) => {
    // since winston does not catch unhandled Rejections,
    // we catch them with the OS event (process.on())
    // and throw it as an exception that will be caught
    // by winston
    throw ex;
    });

    // winston.add(winston.transports.File,{filename:'logfile.log'});

    if (process.env.NODE_ENV !== 'production') {
        winston.add(new winston.transports.Console({
          format: winston.format.simple()
        }));
    }

    // winston.add(winston.transports.MongoDB, {
    //     db: 'mongodb://localhost/vidly',
    //     level: "info"
    // });
}
