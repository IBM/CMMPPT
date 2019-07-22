#------------------------------------------------------------------------------
# Make file for aspects of building MCL that follow a Unix-like convention,
# but are otherwise platform-independent
#------------------------------------------------------------------------------


# Dependency list for the MCL unit test program:
#
unitTest:	unitTest.o libmcl.a 


# Rule for building the MCL static library:
#
libmcl.a:	$(lib_objects) 
	$(AR) $(ds_ar_update_flags) libmcl.a $(lib_objects)
	$(RANLIB) libmcl.a
