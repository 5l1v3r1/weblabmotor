#!/usr/bin/python
# -*- coding: utf-8 -*-

import cgi, cgitb, sys
import MySQLdb as mdb

#habilita debug no browser
cgitb.enable();

dados = cgi.FieldStorage()

a0 = dados.getvalue('a0');
a1 = dados.getvalue('a1');
a2 = dados.getvalue('a2');
a3 = dados.getvalue('a3');
a4 = dados.getvalue('a4');
a5 = dados.getvalue('a5');


con = mdb.connect('localhost', 'root', 'root', 'sensores');

with con:
    
    cur = con.cursor()
    cur.execute("INSERT INTO `sensores`.`dados`(`data`,`a0`,`a1`,`a2`,`a3`,`a4`,`a5`)VALUES(current_timestamp(),%s,%s,%s,%s,%s,%s);",(a0,a1,a2,a3,a4,a5))

print "Status: 200 OK"
print "Content-Type: text/html"
print ""
print "%s" %(dados)
print "<br>"