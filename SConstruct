# -*- mode: python -*-

import os

env = Environment(ENV = {'PATH' : os.environ['PATH']}, tools=['default', 'ascons'])
env.requireVersion('1.0.3')

env.addOptions('version')
env.addOptions('release')

env.Append(depend_includedir = 'deps/include')
env.Append(depend_libdir = 'deps/lib')
env.Append(CXXFLAGS = '-std=c++11')

env.SConscript(dirs='.',
               exports='env',
               variant_dir=env['BUILD_DIR'],
               duplicate=1
               )
