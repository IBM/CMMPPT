# Makefile for the program for this architecture

platform = zlinux64

sep = /
config_dir	= ..$(sep)..$(sep)config$(sep)

include $(config_dir)p_$(platform).mk

# Default exe_type
#
exe_type = debug

include ..$(sep)src$(sep)appl.mk


all:	unitTest $(objects) eso2



# DO NOT DELETE THIS LINE -- make depend depends on it.
