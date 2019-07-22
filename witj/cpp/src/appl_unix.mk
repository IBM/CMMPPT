#------------------------------------------------------------------------------
# Make file for aspects of building WIT-J's C++ native library that follow a
# Unix-like convention, but are otherwise platform-independent
#------------------------------------------------------------------------------

wit_lib_dir   = ..$(sep)..$(sep)..$(sep)wit$(sep)$(platform)

wit_lib       = $(wit_lib_dir)$(sep)libwit.a

witj_so_input = $(lib_objects) -L$(wit_lib_dir) -lwit -lm

#------------------------------------------------------------------------------
# Rule for building the WIT-J C++ native library as a shared object.
#------------------------------------------------------------------------------

libwitj.so:	$(lib_objects) $(wit_lib)
		$(CXX) -shared $(ds_link_flags) $(witj_so_input) -o libwitj.so
