#!/usr/bin/env python

'''
owlcpp/build/release.py is part of owlcpp project
Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
Copyright Mikhail K Levin 2013
'''

import os, subprocess, sys, shutil, re, zipfile, tarfile

def convert_crlf(src_file, dst, target_os):
    s = open(src_file, 'rb').read()
    if '\0' not in s:
        if target_os == 'unix':
            s = s.replace('\r\n', '\n')
            s = s.replace('\r', '\n')
        if target_os == 'dos':
            s = s.replace('\r\n', '\n')
            s = s.replace('\r', '\n')
            s = s.replace('\n', '\r\n')
    else: print src_file + ' binary'
    if os.path.isdir(dst): dst = os.path.join(dst, os.path.basename(src_file))
    open(dst, 'wb').write(s)

def convert_tree_crlf(src, dst, target_os):
    if os.path.isfile(src):
        convert_crlf(src, dst, target_os)
        return

    for dir, sub, files in os.walk(src):
        reldir = os.path.relpath(dir, src)
        dstdir = os.path.join(dst, reldir)
        os.makedirs(dstdir)
        for file in files:
            if file[0] == '.': continue
            if re.search(r'\.pyc$', file): continue
            src_file = os.path.join(dir, file)
            dst_file = os.path.join(dstdir, file)
            convert_crlf(src_file, dst_file, target_os)

def get_version():
    v_str = subprocess.check_output(args=['git', 'describe', '--always', '--dirty=*'])
    v_str = v_str.strip()
    return re.match('v(\d+)\.(\d+).(\d+)(-\d+-g[\dabcdef]+)?', v_str).groups()

def src_compress(out_path, rel_name, target_os):
    rel_path = os.path.join(out_path, rel_name)
    if target_os == 'dos':
        z = zipfile.ZipFile(rel_path + '.zip', 'w', zipfile.ZIP_DEFLATED)
        for dir, sub, files in os.walk(rel_path):
            reldir = os.path.relpath(dir, out_path)
            for file in files:
                z.write(os.path.join(dir, file), os.path.join(reldir, file))
        z.close()
    elif target_os == 'unix':
        b = tarfile.open(rel_path + '.tar.bz2', 'w|bz2')
        for dir, sub, files in os.walk(rel_path):
            reldir = os.path.relpath(dir, out_path)
            for file in files:
                b.add(os.path.join(dir, file), os.path.join(reldir, file))
        b.close()

def src_release(root_path, out_path, name, rel_version, target_os):
    rel_name = name + '-' + rel_version
    print 'making ' + target_os + ' version'
    rel_path = os.path.join(out_path, rel_name)
    if os.path.exists(rel_path): shutil.rmtree(rel_path)
    os.makedirs(rel_path)

    obj_list = [
                'apps', 'binding', 'include', 'lib', 'build',
                'doc', 'external', 'sample_data',
                'readme.txt', 'jamroot.jam'
                ]
    for obj in obj_list:
        src = os.path.join(root_path, obj)
        dst = os.path.join(rel_path, obj)
        convert_tree_crlf(src, dst, target_os)

    s = open(os.path.join(rel_path, obj_list[-1]), 'r').read()
    patt = re.compile(r'(constant\s+OWLCPP_VERSION\s+:\s+)(\[[^[]+\])')
    repl = r'\1"' + rel_version + '"'
    t = re.subn(patt, repl, s)
    open(os.path.join(rel_path, obj_list[-1]), 'w').write(t[0])

    s = open(os.path.join(rel_path, obj_list[-2]), 'r').read()
    s = name + ' version ' + rel_version + s
    open(os.path.join(rel_path, obj_list[-2]), 'w').write(s)

    src_compress(out_path, rel_name, target_os)

if __name__ == '__main__':
    root_path = sys.argv[1]
    out_path = os.path.join(root_path, 'out', 'release')
    if os.path.exists(out_path): shutil.rmtree(out_path)
    v = get_version()
    if len(v) == 4: rel_version = '%s.%s.%s%s' % v
    else: rel_version = '%s.%s.%s' % v
    rel_name = 'owlcpp'
    print 'release ' + rel_name + ' v' + rel_version
    src_release(root_path, out_path, rel_name, rel_version, 'dos')
    src_release(root_path, out_path, rel_name, rel_version, 'unix')
