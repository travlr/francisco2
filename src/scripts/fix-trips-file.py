#!/usr/bin/env python

import xml.etree.ElementTree as et
from lxml import etree
import sys, os
import subprocess as sp
import shlex as sh
import re
from copy import deepcopy


def usage() :
    ret = 'Usage: ' + sys.argv[0] + ' <full-path-to-randomTrips-script> <name-of-city>'
    #print
    #print ret
    #print
    sys.exit(1)


if len(sys.argv) != 3:
    usage()


def str_only_impl(fix_id):
    f = open(sys.argv[2] + '.trips.xml')
    lines = f.readlines()
    f.close()

    clean_lines = []

    for line in lines:
        if line.rstrip():
            clean_lines.append(line)

    lines = clean_lines
    print "[DEBUG] len(lines):", len(lines)

    # <trip id="3598" depart="3598.00" from="5011424" to="9272727#1" />

    new_lines = []

    for i in xrange(len(lines)):
        line = lines[i]

        #print i,
        if str('id="' + fix_id + '"') in line:
            if (fix_id != "0"):
                new_line = lines[i-1]
            else:
                new_line = lines[len(lines)-10]

            new_line = new_line.replace(str(int(fix_id)-1), fix_id, 2)
#            print '[DEBUG] new_line:', new_line
#            if (fix_id != "0"):
#                print "[DEBUG] lines[i-1]:", lines[i-1]
#            else:
#                print "[DEBUG] lines[(len(lines)-10)]", lines[len(lines)-10]
#            print "        line      :", line
#            print "        new_line  :", new_line
            #sys.exit(0)
            new_lines.append(new_line)
        else:
            new_lines.append(line)

    print

    if len(new_lines) == len(lines):
        print "[DEBUG] len(new_lines:)", len(new_lines)
        #print new_lines[:50]
        os.remove(sys.argv[2] + ".trips.xml")
        f = open(sys.argv[2] + ".trips.xml", 'w')
        f.write('\n'.join(new_lines))
        new_lines = []
        f.close()
        #print
        #sys.exit(0)
    else:
        print "[ERROR] the len of new_lines:", len(new_lines), "does not equal len lines:", len(lines)
        print "[ERROR-INFO] the last 5 lines of both:"
        print ""
        idx_offset = range(1,6)
        idx_offset.reverse()
        for i in idx_offset:
            print len(lines)-i, ":", lines[len(lines)-i]
        print ""
        for i in idx_offset:
            print len(lines)-i, ":", new_lines[len(new_lines)-i]
        sys.exit(1)


debug = False
first_run = True
num_run = 0

while not debug:

    num_run += 1
    print "[DEBUG] RUN:", num_run
    has_repair = False

    if first_run:
        print "[DEBUG] running randomTrips.py now.."
        p1 = sp.Popen(sh.split(sys.argv[1] + ' -n ' + sys.argv[2] + '.net.xml -o ' + sys.argv[2] + '.trips.xml'), stdout = sp.PIPE, stderr = sp.PIPE)
        first_run = False

    p2 = sp.Popen(sh.split('duarouter --repair true --trip-files ' + sys.argv[2] + '.trips.xml --net-file ' + sys.argv[2] + '.net.xml --output-file ' + sys.argv[2] + '.rou.xml'), stdout=sp.PIPE, stderr=sp.PIPE)

    ret = p2.communicate()
    #print
    #print "[DEBUG] ret from duarouter:", ret[1]
    #print
    #print
    ##print
    ##print ret[0]
    ##print
    ##print
    ##print ret[1]


    #txt = "Error: The vehicle \\'(\d+)\\' has no valid route"
    txt = r"vehicle \'(\d+)\' "
    pat = re.compile(txt)
    m = pat.search(ret[1])
    fix_id = ""

    if m:
        ##print "and the group is:", m.group(0)
        ##print "and the number is:", m.group(1)
        fix_id = m.group(1)
        print "[DEBUG] fix_id: ", fix_id
    else:
        print "[FINISHED?] the regex did not produce an id to fix!"
        print "            Here's the output:"
        print ret[1]
        sys.exit(1)

    str_only_impl(fix_id)

    #debug = True
