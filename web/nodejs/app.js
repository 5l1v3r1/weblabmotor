var http = require('http');

var sqlite3 = require('sqlite3').verbose();
 
var db = new sqlite3.Database('mydb.db');
 
var check;
 
var dados;

db.serialize(function() {
  
 
  db.run("CREATE TABLE if not exists user_info (info TEXT)");
 
  var stmt = db.prepare("INSERT INTO user_info VALUES (?)");
 
  for (var i = 0; i < 10; i++) {
      stmt.run("Ipsum " + i);
  }
  stmt.finalize();
 
  db.each("SELECT rowid AS id, info FROM user_info", function(err, row) {
      //console.log(row.id + ": " + row.info)
      //console.log(JSON.stringify(row));
      dados = JSON.stringify(row);
  });
 
});

db.close();

var server = http.createServer(function(req, res) {

  res.writeHead(200);

  res.end('Hello Http');
  res.end('Hello Http');
  res.end('Hello Http');

  res.end(dados);
 
});
 
server.listen(8080);
