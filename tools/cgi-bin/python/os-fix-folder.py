#!/usr/bin/python

import cgi, cgitb, sys, subprocess

subprocess.call(['/opt/weblabmotor/sh/install.sh'])

print "Status: 200 OK"
print "Content-Type: text/html"
print ""
print "..."
