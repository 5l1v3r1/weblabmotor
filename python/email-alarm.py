#!/usr/bin/python
#python alarmeEmail.py ./database.db a0 send_from, send_to, subject, text, files, server, port, username, password, isTls

import sqlite3, sys
from enviarEmail import send_mail

bancoDados	=sys.argv[1];
sensor 		=sys.argv[2];
send_from	=sys.argv[3];
send_to		=sys.argv[4];
subject		=sys.argv[5];
text		=sys.argv[6];
files		=sys.argv[7];
server		=sys.argv[8];
port		=sys.argv[9];
username	=sys.argv[10];
password	=sys.argv[11];
isTls		=sys.argv[12]; 


conn = sqlite3.connect(bancoDados)
c = conn.cursor()

c.execute('SELECT * FROM sensores ORDER BY date DESC')

dado=c.fetchone();

if int(dado[1]) >= int(sensor):
   print "Enviar email"
   send_mail( send_from, send_to, subject, text, files, server, port, username, password, isTls)
else:
   print "Sensor sem problemas"