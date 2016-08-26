#!/usr/bin/python

import sys
import commands
import os

CUR = os.path.split(os.path.realpath(__file__))[0]
TMP_DIR = os.path.join(CUR, ".cgservertmp")
BOOST_GIT = "https://github.com/boostorg"
INCLUDE_DIR = os.path.join(CUR, "depend/headers")
LIB_DIR = os.path.join(CUR, "depend/libs")

def exe_cmd(cmd):
    s, o = commands.getstatusoutput(cmd)
    if s != 0:
        print "Exe cmd [%s] failed: [%s]" % (cmd, o)
        exit(0)
    print "Exe cmd [%s] done." % cmd

def clone_from_git(git, version, dest):
    cmd = "git clone %s %s" % (git, dest)
    exe_cmd(cmd)
    cmd = "cd %s && git checkout %s" % (dest, version)
    exe_cmd(cmd)

def get_boost_hpp_lib(lib_name, version):
    os.system("mkdir %s" % TMP_DIR)
    cmd = "git clone %s/%s %s/%s" % (BOOST_GIT, lib_name, TMP_DIR, lib_name)
    exe_cmd(cmd)
    cmd = "cd %s && git checkout %s" % (os.path.join(TMP_DIR, lib_name), version)
    exe_cmd(cmd)
    cmd = "cp -r %s/include/boost/* %s" % (os.path.join(TMP_DIR, lib_name), os.path.join(INCLUDE_DIR, "boost"))
    exe_cmd(cmd)

def resolve_boost_dependency():
    get_boost_hpp_lib("regex", "boost-1.61.0")
    get_boost_hpp_lib("bind", "boost-1.61.0")
    get_boost_hpp_lib("type_traits", "boost-1.61.0")
    get_boost_hpp_lib("preprocessor", "boost-1.61.0")
    get_boost_hpp_lib("mpl", "boost-1.61.0")
    get_boost_hpp_lib("static_assert", "boost-1.61.0")
    get_boost_hpp_lib("iterator", "boost-1.61.0")
    get_boost_hpp_lib("smart_ptr", "boost-1.61.0")
    get_boost_hpp_lib("config", "boost-1.61.0")
    get_boost_hpp_lib("predef", "boost-1.61.0")
    get_boost_hpp_lib("assert", "boost-1.61.0")
    get_boost_hpp_lib("core", "boost-1.61.0")
    get_boost_hpp_lib("throw_exception", "boost-1.61.0")
    get_boost_hpp_lib("asio", "boost-1.61.0")
    get_boost_hpp_lib("system", "boost-1.61.0")
    get_boost_hpp_lib("date_time", "boost-1.61.0")

def resolve_glog_dependency():
    clone_from_git("https://github.com/google/glog", "v0.3.4")

if __name__ == "__main__":
    os.system("rm -rf %s" % TMP_DIR)
    exit(0)
