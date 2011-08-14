#!/usr/bin/env python

print "Generates up to 3 bytes for the id when sending a packet."
id=input("id=")


ids=[id%256];
id/=256;

MORE_ID=0b10000000

if id>0:
	ids.insert(-1,MORE_ID|(id%256))
	id/=128;
	if id>0:
		ids.insert(-1,MORE_ID|(id%256))
		id/=128;

print ids
print "["+', '.join(map(bin,ids))+"]"