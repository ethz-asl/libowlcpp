#!/usr/bin/env python

'''

'''

import os, subprocess, sys, shutil, re, zipfile, tarfile

root_path = sys.argv[1]
out_path = os.path.join(root_path, 'out', 'release')
version = subprocess.check_output(args=['git', 'describe', '--always', '--dirty=*'])
version = version.strip()
v = re.match('v(\d+)\.(\d+).(\d+)(-\d+-g[\dabcdef]+)?', version)
rel_name = 'owlcpp-%s.%s.%s' % v.group(1,2,3)
if v.lastindex==4: rel_name += v.group(4)
print 'creating ' + rel_name
if os.path.exists(out_path): shutil.rmtree(out_path)
rel_path = os.path.join(out_path, rel_name)
os.makedirs(rel_path)

dir_list = [
            'apps', 'binding', 'include', 'lib', 'build', 
            'doc', 'external', 'sample_data'
            ]
for dir in dir_list:
    src = os.path.join(root_path, dir)
    dst = os.path.join(rel_path, dir)
    shutil.copytree(src, dst)

shutil.copy(os.path.join(root_path, 'readme.txt'), rel_path)
jr_fn = 'jamroot.jam'
jr_str = open(os.path.join(root_path, jr_fn), 'r').read()
patt = re.compile(r'(constant\s+OWLCPP_VERSION\s+:\s+)(\[[^[]+\])')
repl = r'\1"' + version + '"'
t = re.subn(patt, repl, jr_str)
open(os.path.join(rel_path, jr_fn), 'w').write(t[0])


z = zipfile.ZipFile(rel_path + '.zip', 'w')
for dir, sub, files in os.walk(rel_path):
    reldir = os.path.relpath(dir, out_path)
    for file in files: z.write(os.path.join(dir, file), os.path.join(reldir, file))
z.close()

b = tarfile.open(rel_path + '.tar.bz2', 'w|bz2')
for dir, sub, files in os.walk(rel_path):
    reldir = os.path.relpath(dir, out_path)
    for file in files: b.add(os.path.join(dir, file), os.path.join(reldir, file))
b.close()
