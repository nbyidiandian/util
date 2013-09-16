Import("env")

env = env.Clone()

lib_util_sources = ['scoped_latency.cpp',
                    'time_util.cpp']

env.aLibrary(target = 'util', source = lib_util_sources)

env.aDirs('test')

Return('lib_util_sources')
