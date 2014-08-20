#!/usr/bin/env python
# -*- coding: utf-8 -*-

from subprocess import check_call
import argparse
import os
import fnmatch

# cli arg parsing
parser = argparse.ArgumentParser(description="Returns delimited string of all the sub-directories of a provided root directory, that contain header files.")
parser.add_argument('--delimeter', nargs=1, default='\n', help="Character to use as delimiter of returned string. default='\n'")
parser.add_argument("root_dir_path", help='The top directory to recurse through.')
args = parser.parse_args()


# recurse the dirs, find the .h files, and include the directory.

header_dirs = []

for root, dnames, fnames in os.walk(args.root_dir_path):
    headers = fnmatch.filter(fnames, '*.h')
    if len(headers):
        header_dirs.append(root)


ret_str = args.delimeter[0].join(header_dirs)

print ret_str

