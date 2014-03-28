APPNAME="base-utils"
VERSION="0.1.3"

def options(opt):
    opt.load("compiler_cxx")

def configure(conf):
    conf.load("compiler_cxx")
    _enable_pic(conf)
    _enable_cxx11(conf)
    _enable_debug(conf)
    _extra_warnings(conf)
    conf.env.LIB_PTHREAD = 'pthread'

def build(bld):
    bld.stlib(source=bld.path.ant_glob("base/*.cc"), target="base", includes="base", use="PTHREAD")
    bld.program(source=bld.path.ant_glob("test/test*.cc"), target="unittest", includes=".", use="base PTHREAD")

#
# waf helper code
#

import os
import sys
from waflib import Logs

# use clang++ as default compiler (for c++11 support on mac)
if sys.platform == 'darwin' and "CXX" not in os.environ:
    os.environ["CXX"] = "clang++"

def _enable_pic(conf):
    conf.env.append_value("CXXFLAGS", "-fPIC")
    conf.env.append_value("LINKFLAGS", "-fPIC")

def _enable_cxx11(conf):
    Logs.pprint("PINK", "C++11 features enabled")
    if sys.platform == "darwin":
        conf.env.append_value("CXXFLAGS", "-stdlib=libc++")
        conf.env.append_value("LINKFLAGS", "-stdlib=libc++")
    conf.env.append_value("CXXFLAGS", "-std=c++0x")

def _enable_debug(conf):
    if os.getenv("DEBUG") == "1":
        Logs.pprint("PINK", "Debug support enabled")
        conf.env.append_value("CXXFLAGS", "-Wall -pthread -ggdb".split())
    else:
        conf.env.append_value("CXXFLAGS", "-Wall -pthread -O3 -ggdb -fno-omit-frame-pointer -DNDEBUG".split())

def _extra_warnings(conf):
    warning_flags = "-Wextra -pedantic -Wformat=2 -Wno-unused-parameter -Wshadow -Wwrite-strings -Wredundant-decls -Wmissing-include-dirs -Wno-format-nonliteral"
    if sys.platform == "darwin":
        warning_flags += " -Wno-gnu -Wstrict-prototypes -Wold-style-definition -Wnested-externs"
    conf.env.append_value("CXXFLAGS", warning_flags.split())
