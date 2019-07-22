#-------------------------------------------------------------------------------
# legacy.mk
#
# This file is included by ds.mk.
# Its contents are items that have the following three properties:
#
# * They used to be specified somewhere in the config directory.
# * They seem to serve no current purpose.
# * They are referenced/invoked by other Makefiles.
#
# The items are being kept in this file in order to avoid doing harm to the
# Makefiles that use them.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# rw_library_dir
#
# This used to identify where the Rogue Wave library was located.
#-------------------------------------------------------------------------------

rw_library_dir = $(RW_ROOT_DIR)$(platform)/lib

#-------------------------------------------------------------------------------
# ds_static_fortran_flags
#-------------------------------------------------------------------------------

ds_static_fortran_flags =

#-------------------------------------------------------------------------------
# Parallel compilation targets
# 
# This will run compiles on compile-servers concurrently, 
# assuming that it is started from a server machine.
# To use it, you have to be logged onto the serverss
# and have current AFS tokens.  (AFS tokens are passed to remote shell
# executions between Agora machines, but not to or from non-Agora 
# machines.)  You also have to be able to use rsh to run commands on the
# two remote machines, which will generally mean having lines in your .rhosts
# of the form
# host userid
# file to allow it.  See the rsh documentation.
#-------------------------------------------------------------------------------

parComp:
	$(save_exe_type)
	# Make names of object files that depend on the changed source files
	# oodObjs: file for out-of-date-object file names
	$(MAKE) -n dotohs \
	| sed -e '/^mv /d' -e '/^yacc /d' -e '/^lex /d' \
	| sed -e '1d' -e '$$d' -e '/Nothing to be done/d' \
	| sed -e 's/^.* \([a-zA-Z0-9_]*\.o\) .*$$/\1/' -e 's/^.* \([a-zA-Z0-9_]*\.o\)$$/\1/' >oodObjs
	# Compile the .o files
	$(DS_DIR)bin/distCompile oodObjs $(MAKEOVERRIDES)
	rm oodObjs

#-------------------------------------------------------------------------------
# this is a variation of the above.
# here, we save the whole command in oodObjs.
# the script newdistCompile starts up slave processes and sends commands
# to the slaves.  I don't think assumptions are made about what the commands are,
# although they are executed individually, so builtins like "cd" won't have
# the desired effect.
# There is no point in MAKEOVERRIDES now.
#-------------------------------------------------------------------------------

newparComp:
	$(save_exe_type)
	# Make names of object files that depend on the changed source files
	# oodObjs: file for out-of-date-object file names
	$(MAKE) -n dotohs \
	| sed -e '/^mv /d' -e '/^yacc /d' -e '/^lex /d' \
	| sed -e '1d' -e '$$d' -e '/Nothing to be done/d' > oodObjs
	# Compile the .o files
	$(DS_DIR)bin/newdistCompile oodObjs 
	rm oodObjs
