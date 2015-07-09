#!/usr/bin/python

import cgi, cgitb, sqlite3, sys

dados = cgi.FieldStorage() 

bdOrigem = dados.getvalue('bdOrigem')
bdAlvo = dados.getvalue('bdAlvo');

conn = sqlite3.connect(bdAlvo)
c = conn.cursor()                            

c.execute("ATTACH (?) AS temporaria",[bdOrigem])
c.execute("INSERT INTO sensores SELECT * FROM temporaria.sensores")
c.execute("DETACH DATABASE temporaria")

conn.commit()
conn.close()

print "Status: 200 OK"
print "Content-Type: text/html"
print ""
print "%s" %(dados)