# Copyright 2008, Google Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
#
#  1. Redistributions of source code must retain the above copyright notice, 
#     this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
#  3. Neither the name of Google Inc. nor the names of its contributors may be
#     used to endorse or promote products derived from this software without
#     specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import os
import os.path
from SCons.Script.SConscript import SConsEnvironment

# -------------------------------------
# SCons version checking
# -------------------------------------
EnsureSConsVersion(0,97)  # v0.97 or greater.

# -------------------------------------
# Enviroment initialization and command line parsing
# -------------------------------------
env = Environment()
option_filename = 'libkml.conf'
opts = Options(option_filename)

# Setup installation directory prefixes.
if env['PLATFORM'] == 'win32':
  opts.Add(PathOption('PREFIX', 'Installation prefix directory',
                      'c:\\\\libkml', PathOption.PathAccept))
else:
  opts.Add(PathOption('PREFIX', 'Installation prefix directory',
                      '/usr/local', PathOption.PathAccept))
opts.Update(env)

# Installation locations of this software.
opts.AddOptions(
  BoolOption('DEBUG', 'Build with debug symbols', False),
  # Where shall install the libraries and headers?
  PathOption('LIBDIR', 'Installation prefix directory for static libraries',
             os.path.join(env['PREFIX'], 'lib'),
             PathOption.PathAccept),
  PathOption('INCDIR', 'Installation prefix directory for header files',
             os.path.join(env['PREFIX'], 'include'),
             PathOption.PathAccept),
)

# Build options and locations of third-party dependencies.
# By default we'll look for our dependent libraries and headers in these
# locations.
if env['PLATFORM'] in ['posix', 'darwin']:
  default_incdir = '/usr/local/include'
  default_libdir = '/usr/local/lib'
else:
  default_incdir = ''
  default_libdir = ''

env['HAS_CPPUNIT'] = False
opts.AddOptions(
  # Where are the expat library and headers?
  PathOption('EXPAT_LIBDIR', 'Location of expat library',
             default_libdir, PathOption.PathAccept),
  PathOption('EXPAT_INCDIR', 'Location of expat include directory',
             default_incdir, PathOption.PathAccept),
  # Where are the cppunit library and headers?
  PathOption('CPPUNIT_LIBDIR', 'Location of cppunit library',
             default_libdir, PathOption.PathAccept),
  PathOption('CPPUNIT_INCDIR', 'Location of cppunit include directory',
             default_incdir, PathOption.PathAccept),
  # By default, try to SWIG all available languages.
  BoolOption('RUNSWIG', 'Attempt to generate SWIG bindings', 1),
)

opts.Update(env)
opts.Save(option_filename, env)  # We remember previous user-set options.

# Print help text for options with 'scons -h'
Help(opts.GenerateHelpText(env))

# Now update the environment with the paths.
env.Append(CPPPATH=['#src',
                    env['EXPAT_INCDIR'],
                    env['CPPUNIT_INCDIR']])
env.Append(LIBPATH=['#build', '#build/kml/dom', '#build/kml/util',
                    env['EXPAT_LIBDIR'],
                    env['CPPUNIT_LIBDIR']])

# zlib is required. We assume it exists on linux and mac. We supply our own
# third_party package for windows:
if env['PLATFORM'] == 'win32':
  env.Append(CPPPATH=['#/third_party/zlib-1.2.3'])
  env.Append(LIBPATH=['#/third_party/zlib-1.2.3.win32/lib'])
  # The names for these libraries are different on windows:
  env.Append(LIBZ='zlib')
  env.Append(EXPAT='libexpat')
else:
  env.Append(LIBZ='z')
  env.Append(EXPAT='expat')

# -------------------------------------
# Configure contexts
# -------------------------------------
if not env.GetOption('clean'):
  conf = Configure(env.Clone())
  if not conf.CheckLibWithHeader(
      env['EXPAT'],
      os.path.join(env['EXPAT_INCDIR'], 'expat.h'),
      'C'):
    # Bail if we can't find an expat:
    print 'error: the expat library could not be found on your system. run'
    print 'scons -h for help in specifying non-default locations'
    Exit(1)
  if not conf.CheckCHeader('zlib.h'):
    # This is really a check for mac/linux since we supply zlib for windows.
    print 'error: no zlib header found'
    Exit(1)
  # We check to see if cppunit installation is sane and set a flag. The unit
  # test extension will test this flag to see if we can run the tests.
  if conf.CheckLibWithHeader(
      'cppunit',
      os.path.join(env['CPPUNIT_INCDIR'], 'cppunit', 'Test.h'),
      'C++'):
    env['HAS_CPPUNIT'] = True
  conf.Finish()

# -------------------------------------
# Unit Test Extension
# -------------------------------------
# TODO: we currently require libcppunit to have been built with
# ./configure --disable-typeinfo-name. Can we work around this?
def RunUnitTest(target, source, env):
  file_name = os.path.basename(target[0].abspath)
  pass_str = 'FAILED'
  if env.Execute(target[0].abspath) == 0:
    SConsEnvironment.UnitTestPassed += 1
    pass_str = 'PASSED'
  else:
    SConsEnvironment.UnitTestFailed += 1

  print "%s %s (Test Binaries: %d passed, %d failed)\n" % (
      file_name, pass_str,
      SConsEnvironment.UnitTestPassed,
      SConsEnvironment.UnitTestFailed)

def UnitTest(env, name, source, **kwargs):
  env = env.Clone()
  # Special-case the occurence of runtests called when cppunit is not
  # available.
  if 'runtests' in COMMAND_LINE_TARGETS and not env['HAS_CPPUNIT']:
      msg = ('It looks like you don\'t have cppunit installed. It is '
             'available from http://cppunit.sourceforge.net/ and comes '
             'highly recommended by the developers of this software. Once '
             'you have installed it, run scons -h to see how to supply a '
             'non-default location.')
      print msg
      Exit(1)
  # We capture all other calls to env.UnitTest here and check cppunit
  # availability, bailing if we can't build.
  if not env['HAS_CPPUNIT']:
    return None
  build_dir = Dir("#/build").abspath
  if env['PLATFORM'] != 'win32':
    # CPPUNIT does not have non-virtual destructors where gcc believes it should
    # if compiling with -Wall.
    env.Append(CXXFLAGS=['-Wno-non-virtual-dtor'])
  # TODO: windows path setting.
  env.Append(ENV={'LD_LIBRARY_PATH' : [build_dir, env['CPPUNIT_LIBDIR']],
                  'DYLD_LIBRARY_PATH' : [build_dir, env['CPPUNIT_LIBDIR']],
                  'PATH' : [env['ENV']['PATH']] + [build_dir]})
  env.Append(LIBS=['kmldom', env['EXPAT'], 'cppunit'])

  test = env.Program(name, source, **kwargs)
  env.Alias('buildtests', test)
  if 'runtests' in COMMAND_LINE_TARGETS:
    env.AddPostAction(test, RunUnitTest)
    env.AlwaysBuild(test)
    env.Alias('runtests', test)
  return test

# We need to modify the SConsEnvironment class for our unit test extension.
# We can't simply copy the env variable because we need to have a modifiable
# global variable.
SConsEnvironment.UnitTest = UnitTest
SConsEnvironment.UnitTestFailed = 0
SConsEnvironment.UnitTestPassed = 0

# -------------------------------------
# Subscripts
# -------------------------------------
SConscript('third_party/SConscript',exports='env')
SConscript('src/SConscript',exports='env',build_dir='build',duplicate=0)
if 'swig' in env['TOOLS'] and env['RUNSWIG']:
  # Generate SWIG bindings if the tool is in PATH or if the binary was
  # specified on the command line:
  SConscript('src/swig/SConscript',exports='env',duplicate=0)

# -------------------------------------
# Install
# -------------------------------------
try:
  umask = os.umask(022)
except OSError:  # ignore on systems that don't have umask
  pass
env.Alias('install', env['PREFIX'])

# -------------------------------------
# Cleaning
# -------------------------------------
if env.GetOption('clean'):
  # Delete the build directory and its contents.
  env.Execute(Delete('build'))
