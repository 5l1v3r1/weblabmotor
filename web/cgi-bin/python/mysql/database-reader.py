#!/usr/bin/python
# -*- coding: utf-8 -*-

import cgi, cgitb, sys, json
import MySQLdb as mdb
cgitb.enable();

connection = mdb.connect('localhost', 'root', 'root', 'sensores');
 
cursor = connection.cursor()

cursor.execute("SELECT * FROM dados ORDER BY data ASC")
 
dados = cursor.fetchall()
dados = [i for sub in dados for i in sub]  # Convert to list from tuple

connection.close() 

print "Status: 200 OK"
print "Content-Type: text/html"
print ""
print "%s" % (dados)