#! /usr/bin/env python
# encoding: utf-8
import sys
#import Options

# the following two variables are used by the target "waf dist"
VERSION='0.0.1'
APPNAME='beam'

# these variables are mandatory ('/' are converted automatically)
top = '.'
out = 'build'

def options(opt):
  opt.load('compiler_c')
  opt.add_option('--exe', action='store_true', default=False,
      help='execute the program after it is built')
  opt.add_option('--debug', action='store_true', dest='debug', default=False, help='Build with DEBUG')


def configure(conf):
  conf.env["CC"] = ["clang"]
  conf.load('compiler_c')
  #conf.check(header_name='stdio.h', features='cxx cxxprogram', mandatory=False)
  conf.check(header_name='stdio.h', features='c cprogram', mandatory=False)
  conf.check_cfg(package='elementary', uselib_store='elementary', atleast_version='0.0.1', args='--cflags --libs', mandatory=True)
  #conf.check_cfg(package='libpng', uselib_store='png', atleast_version='0.0.0', mandatory=1, args='--cflags --libs')

  #conf.define ('DEBUG', 0)
  #conf.env['CXXFLAGS']=['-O2']
    
  # set some debug relevant config values
  #if Options.options.debug:
  conf.define ('DEBUG', 1)
  conf.env['CXXFLAGS'] = ['-O0', '-g3']
  conf.env.CFLAGS = ['-g'] 

def post(ctx):
  if ctx.options.exe:
    ctx.exec_command('./build/beam')


def build(bld):

  c_files = bld.path.ant_glob('src/*.c')
  bld.program(
      source= c_files, 
      target='beam', 
      use='elementary',
      includes = 'src',
      defines = ['EDITOR']
      )

  bld.add_post_fun(post)
  #bld.add_pre_fun(pre)

