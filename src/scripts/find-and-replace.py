#!/usr/bin/env python

import os
import sys
import re






def print_usage():
    msg = """
    
    USAGE: 
        %s <dir-path> <find-word> <replace-word>
    
    """ % os.path.basename(sys.argv[0])
    
    print msg
    

def replace_target_word(line):
    return re.sub(sys.argv[2], sys.argv[3], line)




if __name__ == '__main__':
    
    if (len(sys.argv) != 4):
        print_usage()
        sys.exit(1)
    
    files = os.listdir(sys.argv[1])
    
    if (len(files) == 0):
        print "[ERROR] the dir-path given is empty"
        sys.exit(1)
    
    for fname in files:
        f = open(sys.argv[1] + '/' + fname, 'r+')
        lines = f.readlines()
        f.close()
        if not lines:
            print "[ERROR] The file " + fname + " is empty. Try again :-)"
            sys.exit(1)
            
        new_lines = []
        
        for i in range(len(lines)):
            line = lines[i]
            new_line = replace_target_word(line)
            count = 0;
            if new_line != line:
                count += 1
                txt = '[INFO] # ' + str(count) + ' word replacement made: FILE: ' \
                        + sys.argv[1] + '/' + fname + ' LINE: ' + str(i) + '.'
                print ''
                print txt
                print ''
                new_lines.append(new_line)
            else:
                new_lines.append(lines[i])
        f = open(sys.argv[1] + '/' + fname, 'w')
        f.write('\n'.join(new_lines))
        f.close()
            












































