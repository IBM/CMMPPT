#------------------------------------------------------------------------------
# Make file for building MCL on z/Linux 64-bit
#------------------------------------------------------------------------------

platform   = zlinux64

sep        = /

config_dir = ../../config/

include ../../config/p_$(platform).mk

# Override of ds_comp_std_flags value as set in p_zlinux64.mk:
#
ds_comp_std_flags = -Werror

# Compiler flag to specify the output file.
# We just let gcc use its default output file name.
#
OUTPUT_OPTION =

include ../src/appl.mk

include ../src/appl_unix.mk

#------------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- make depend depends on it.

arg.o: ../../mcl/src/arg.h ../../mcl/src/defs.h
arg.o: ../../config/dummy_headers/stddef.h ../../config/dummy_headers/string
arg.o: ../../config/dummy_headers/vector ../../mcl/src/facility.h
arg.o: ../../config/dummy_headers/map ../../mcl/src/msg.h
arg.o: ../../config/dummy_headers/stdlib.h
arg.o: ../../config/dummy_headers/iostream
arg.o: ../../config/dummy_headers/assert.h ../../config/dummy_headers/stdio.h
arg.o: ../../config/dummy_headers/string.h
facility.o: ../../mcl/src/facility.h ../../mcl/src/defs.h
facility.o: ../../config/dummy_headers/string ../../config/dummy_headers/map
facility.o: ../../mcl/src/arg.h ../../config/dummy_headers/stddef.h
facility.o: ../../config/dummy_headers/vector ../../mcl/src/level.h
facility.o: ../../mcl/src/msg.h ../../config/dummy_headers/stdlib.h
facility.o: ../../mcl/src/output.h ../../config/dummy_headers/stdio.h
facility.o: ../../mcl/src/set.h ../../config/dummy_headers/assert.h
facility.o: ../../config/dummy_headers/iostream
level.o: ../../mcl/src/level.h ../../mcl/src/defs.h
level.o: ../../config/dummy_headers/stddef.h ../../mcl/src/facility.h
level.o: ../../config/dummy_headers/string ../../config/dummy_headers/map
level.o: ../../config/dummy_headers/iostream
level.o: ../../config/dummy_headers/assert.h
msg.o: ../../mcl/src/msg.h ../../config/dummy_headers/stdlib.h
msg.o: ../../config/dummy_headers/string ../../mcl/src/defs.h
msg.o: ../../mcl/src/arg.h ../../config/dummy_headers/stddef.h
msg.o: ../../config/dummy_headers/vector ../../mcl/src/facility.h
msg.o: ../../config/dummy_headers/map ../../mcl/src/level.h
msg.o: ../../config/dummy_headers/iostream ../../config/dummy_headers/stdio.h
msg.o: ../../config/dummy_headers/assert.h
output.o: ../../mcl/src/output.h ../../mcl/src/defs.h
output.o: ../../config/dummy_headers/string
output.o: ../../config/dummy_headers/stdio.h ../../mcl/src/facility.h
output.o: ../../config/dummy_headers/map ../../config/dummy_headers/iostream
output.o: ../../config/dummy_headers/stdlib.h
output.o: ../../config/dummy_headers/string.h
set.o: ../../config/dummy_headers/assert.h ../../mcl/src/msg.h
set.o: ../../config/dummy_headers/stdlib.h ../../config/dummy_headers/string
set.o: ../../mcl/src/defs.h ../../mcl/src/set.h
set.o: ../../config/dummy_headers/map
unitTest.o: ../../mcl/src/unitTest.h ../../mcl/src/msg.h
unitTest.o: ../../config/dummy_headers/stdlib.h
unitTest.o: ../../config/dummy_headers/string ../../mcl/src/defs.h
unitTest.o: ../../mcl/src/output.h ../../config/dummy_headers/stdio.h
unitTest.o: ../../mcl/src/arg.h ../../config/dummy_headers/stddef.h
unitTest.o: ../../config/dummy_headers/vector ../../mcl/src/facility.h
unitTest.o: ../../config/dummy_headers/map ../../mcl/src/level.h
unitTest.o: ../../mcl/src/set.h ../../config/dummy_headers/assert.h
unitTest.o: ../../config/dummy_headers/iostream
