#!/usr/bin/env python

'''

'''

import os, subprocess, sys

rel_path = sys.argv[1]
print 'creating ' + rel_path
if os.path.exists(rel_path): os.rmdir(rel_path)
os.makedirs(rel_path)

print subprocess.check_output(args=['git', 'describe', '--always', '--dirty=*'])
