#!/usr/bin/python

import cgi, cgitb, sqlite3, sys

dados = cgi.FieldStorage() 

bd = dados.getvalue('bd')
a0 = dados.getvalue('a0');
a1 = dados.getvalue('a1');
a2 = dados.getvalue('a2');
a3 = dados.getvalue('a3');
a4 = dados.getvalue('a4');
a5 = dados.getvalue('a5');

conn = sqlite3.connect(bd)
c = conn.cursor()                            

c.execute("INSERT INTO sensores VALUES (datetime('now','localtime'),?,?,?,?,?,?,'1','0')",(a0,a1,a2,a3,a4,a5))

conn.commit()
conn.close()

print "Status: 200 OK"
print "Content-Type: text/html"
print ""
print "%s" %(dados)