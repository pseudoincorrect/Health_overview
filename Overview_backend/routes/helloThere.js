const express = require('express');
const router = express.Router();

router.get('/', async (req, res) => {
    res.send({hello: "Hello from the backend !"});
});


module.exports = router; 

/*
POST, GET at http://localhost:3000/api/employess
PUT, GET, DELETE at http://localhost:3000/api/employess/:id
{
  "nodeAddress": "12345",
  "timeSeries": {
      "temperature": "23"
      "humidity": "55"
  }
}
dont forget the token
*/
