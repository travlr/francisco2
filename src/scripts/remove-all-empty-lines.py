# -*- coding: utf-8 -*-
"""
Created on Sun Apr 20 21:00:21 2014

@author: prodatalab
"""

import os
import sys










def print_usage(file_name):
    print ""
    print file_name + " <dir_path>"
    print ""


if __name__ == '__main__':

    if len(sys.argv) != 2:
        print_usage(os.path.basename(sys.argv[0]))
        sys.exit(1)
        
    file_names = os.listdir(sys.argv[1]);

    if len(file_names) == 0:
        print "[ERROR] THE DIR HAS NO FILES";
        sys.exit(1)

    print ""
    print file_names
    print ""

    for file_name in file_names:
        f = open(sys.argv[1] + "/" + file_name, 'r')
        lines = f.readlines()
        f.close()
        
        new_lines = []                
        
        print ""        
        
        for line in lines:
            if line == "\n" or line == "\r\n":
                print ".",
                continue
            else:
                print '+',
                new_lines.append(line)
        
        print ""        
        
        f = open(sys.argv[1] + "/" + file_name, 'w')
        f.write('\n'.join(new_lines))
        f.close()

        print ""
        print "All done"
        print ""









































