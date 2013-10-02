APPNAME="b0"
VERSION="0.1"

import os
import sys
from waflib import Logs

# use clang++ as default compiler (for c++11 support on mac)
if sys.platform == 'darwin' and not os.environ.has_key("CXX"):
    os.environ["CXX"] = "clang++"

def options(opt):
    opt.load("compiler_cxx")

def configure(conf):
    conf.load("compiler_cxx")

    if sys.platform == "darwin":
        conf.env.append_value("CXXFLAGS", "-stdlib=libc++")
        conf.env.append_value("LINKFLAGS", "-stdlib=libc++")
    if os.getenv("DEBUG") in ["true", "1"]:
        Logs.pprint("PINK", "debug support enabled")
        conf.env.append_value("CXXFLAGS", "-std=c++11 -Wall -pthread -ggdb".split())
    else:
        conf.env.append_value("CXXFLAGS", "-std=c++11 -Wall -pthread -O3 -ggdb -fno-omit-frame-pointer -DNDEBUG".split())

    conf.env.LIB_PTHREAD = 'pthread'

def build(bld):
    bld.stlib(source=bld.path.ant_glob("b0/*.cc"), target="b0", includes="b0", lib="pthread")

    def _prog(source, target, includes=".", use="b0 PTHREAD"):
        bld.program(source=source, target=target, includes=includes, use=use)

    _prog(bld.path.ant_glob("test/test*.cc"), "testharness")
