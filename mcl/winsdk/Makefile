#------------------------------------------------------------------------------
# Make file for building MCL with Windows SDK
#------------------------------------------------------------------------------

platform          = winsdk

sep               = /

config_dir        = ../../config/

include ../../config/p_$(platform).mk

include ../src/appl.mk

#------------------------------------------------------------------------------
# Rule for building the MCL static library:
#------------------------------------------------------------------------------

mcl.lib:	$(lib_objects)
		lib -nologo $(lib_objects) -out:mcl.lib

#------------------------------------------------------------------------------
# Rule for building the MCL unit test program:
#------------------------------------------------------------------------------

unitTest.exe:	unitTest.obj mcl.lib
		link $(ds_link_flags) unitTest.obj mcl.lib -out:unitTest.exe

#-------------------------------------------------------------------------------
# Rule to terminate with an error message when trying to build "unitTest":
#-------------------------------------------------------------------------------

unitTest:
		$(error MCL MAKE FILE ERROR:                                 \
                        "unitTest" cannot be built on the "winsdk" platform. \
                        "unitTest.exe" should be built instead)              \

#------------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- make depend depends on it.

arg.obj: ../../mcl/src/arg.h ../../mcl/src/defs.h
arg.obj: ../../config/dummy_headers/stddef.h
arg.obj: ../../config/dummy_headers/string ../../config/dummy_headers/vector
arg.obj: ../../mcl/src/facility.h ../../config/dummy_headers/map
arg.obj: ../../mcl/src/msg.h ../../config/dummy_headers/stdlib.h
arg.obj: ../../config/dummy_headers/iostream
arg.obj: ../../config/dummy_headers/assert.h
arg.obj: ../../config/dummy_headers/stdio.h
arg.obj: ../../config/dummy_headers/string.h
facility.obj: ../../mcl/src/facility.h ../../mcl/src/defs.h
facility.obj: ../../config/dummy_headers/string
facility.obj: ../../config/dummy_headers/map ../../mcl/src/arg.h
facility.obj: ../../config/dummy_headers/stddef.h
facility.obj: ../../config/dummy_headers/vector ../../mcl/src/level.h
facility.obj: ../../mcl/src/msg.h ../../config/dummy_headers/stdlib.h
facility.obj: ../../mcl/src/output.h ../../config/dummy_headers/stdio.h
facility.obj: ../../mcl/src/set.h ../../config/dummy_headers/assert.h
facility.obj: ../../config/dummy_headers/iostream
level.obj: ../../mcl/src/level.h ../../mcl/src/defs.h
level.obj: ../../config/dummy_headers/stddef.h ../../mcl/src/facility.h
level.obj: ../../config/dummy_headers/string ../../config/dummy_headers/map
level.obj: ../../config/dummy_headers/iostream
level.obj: ../../config/dummy_headers/assert.h
msg.obj: ../../mcl/src/msg.h ../../config/dummy_headers/stdlib.h
msg.obj: ../../config/dummy_headers/string ../../mcl/src/defs.h
msg.obj: ../../mcl/src/arg.h ../../config/dummy_headers/stddef.h
msg.obj: ../../config/dummy_headers/vector ../../mcl/src/facility.h
msg.obj: ../../config/dummy_headers/map ../../mcl/src/level.h
msg.obj: ../../config/dummy_headers/iostream
msg.obj: ../../config/dummy_headers/stdio.h
msg.obj: ../../config/dummy_headers/assert.h
output.obj: ../../mcl/src/output.h ../../mcl/src/defs.h
output.obj: ../../config/dummy_headers/string
output.obj: ../../config/dummy_headers/stdio.h ../../mcl/src/facility.h
output.obj: ../../config/dummy_headers/map
output.obj: ../../config/dummy_headers/iostream
output.obj: ../../config/dummy_headers/stdlib.h
output.obj: ../../config/dummy_headers/string.h
set.obj: ../../config/dummy_headers/assert.h ../../mcl/src/msg.h
set.obj: ../../config/dummy_headers/stdlib.h
set.obj: ../../config/dummy_headers/string ../../mcl/src/defs.h
set.obj: ../../mcl/src/set.h ../../config/dummy_headers/map
unitTest.obj: ../../mcl/src/unitTest.h ../../mcl/src/msg.h
unitTest.obj: ../../config/dummy_headers/stdlib.h
unitTest.obj: ../../config/dummy_headers/string ../../mcl/src/defs.h
unitTest.obj: ../../mcl/src/output.h ../../config/dummy_headers/stdio.h
unitTest.obj: ../../mcl/src/arg.h ../../config/dummy_headers/stddef.h
unitTest.obj: ../../config/dummy_headers/vector ../../mcl/src/facility.h
unitTest.obj: ../../config/dummy_headers/map ../../mcl/src/level.h
unitTest.obj: ../../mcl/src/set.h ../../config/dummy_headers/iostream
