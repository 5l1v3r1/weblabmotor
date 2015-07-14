var http = require('http');
 
 
var server = http.createServer(function(req, res) {
 
  res.writeHead(200);
 
  var str = '{ "name": "John Doe", "age": 42 }';
  var json = JSON.parse(str);

  res.end('Hello Http');
      res.end(json);

 
});
 
server.listen(8080);