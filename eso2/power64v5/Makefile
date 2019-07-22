# Makefile for the program for this architecture


platform = power64v5

sep = /
config_dir	= ..$(sep)..$(sep)config$(sep)

include $(config_dir)p_$(platform).mk

# Default exe_type
#
exe_type = debug

include ..$(sep)src$(sep)appl.mk


all:	$(objects) unitTest eso2



../src/TAGS:	$(sources)
		cd ..$(sep)src; etags -C $(sources)



# DO NOT DELETE THIS LINE -- make depend depends on it.

