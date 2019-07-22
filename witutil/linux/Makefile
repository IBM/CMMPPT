# Makefile for the program for this architecture

# $Header: /gsa/yktgsa/projects/w/wit/devbase/witutil/linux/Makefile,v 1.2 2006/04/19 20:38:36 rjw Exp $

platform = linux

sep = /
config_dir	= ..$(sep)..$(sep)config$(sep)

include $(config_dir)p_$(platform).mk

# exe_type should be debug, profile, slow, fast, beta, or released.  
# To override it without
# changing this makefile, use "exe_type=debug" (for example) as an 
# argument to make on the command line.  This default was chosen so that
# we don't accidentally ship product code with symbol tables and other
# info for reverse-engineering enclosed.
exe_type = debug

include ..$(sep)src$(sep)appl.mk


all:	unitTest $(objects) libwutl.a shrtFall

# Following is for doing compiles locally.  To use it, execute "make dotohs".
# The is useful because doing manipulating libraries and linking
# runs much faster when it is run on the machine that has the files, but
# it does not make much difference when compiling
dotohs:	$(objects)

# Following is for doing compiles locally and link on mfglog2.  It assumes
# you have an alias for setvars.[ck]sh and that your working directory is named
# after your userid or that there is a soft link to do the same thing.
remlink:	dotohs
	sleep 2		# Give NFS time to catch up
	rsh mfglog2 "setvars; cd /u/implode/$(USER)/witutil/$(platform); pwd; nice $(MAKE) -k shrtFall "

remlink1:	dotohs
	sleep 2		# Give NFS time to catch up
	rsh $(XLC_SERVER) "setvars; cd /u/implode/$(USER)/witutil/$(platform); pwd; nice $(MAKE) -k shrtFall"


remlinkKsh:	dotohs
	sleep 2		# Give NFS time to catch up
	rsh $(XLC_SERVER) "export PATH=/usr/gnu/bin/:$(PATH); . /u/implode/bin/setvars.ksh; cd /u/implode/$(USER)/witutil/$(platform); pwd; nice $(MAKE) -k shrtFall "

# Following is for doing compiles on woodenshoe and links locally.  They assume
# that your working directory is named after your userid or that there is a soft 
# link to do the same thing.
#
# remcomp further assumes :
#    - Your default shell is csh.
#    - You have an alias for setvars.csh named setvars.
#    - /usr/gnu/bin is in your PATH
#
# remcompKsh further assumes:
#    - Your default shell is ksh.
# 
remcomp:
	rsh $(XLC_SERVER) "setvars; cd /u/implode/$(USER)/witutil/$(platform); pwd; nice /usr/gnu/bin/make dotohs exe_type=$(exe_type); hostname"
	sleep 2         # Give NFS time to catch up
	$(MAKE)  shrtFall; hostname


remcompKsh:
	rsh $(XLC_SERVER) "export PATH=/usr/gnu/bin/:$(PATH); . /u/implode/bin/setvars.ksh; cd /u/implode/$(USER)/scenario/$(platform); pwd; nice $(MAKE) dotohs exe_type=$(exe_type); hostname"
	sleep 2         # Give NFS time to catch up
	$(MAKE)  shrtFall

../src/TAGS:	$(sources)
		cd ..$(sep)src; etags -C $(sources)

parTestRun:	
	$(MAKE) parComp
	$(MAKE) shrtFall

shrtFallRun:	shrtFall
	$<

sentinelShortFall:	shrtFall.o $(lib_objects) \
			../../wit/$(platform)/libwit.a \
			/lib/libm.a /lib/libxlf.a
	SE_HOME=/u/dcwong/SE_HOME; export SE_HOME; \
		/u/dcwong/SE_HOME/sentinel $(CC)  $(CPPFLAGS) $(CXXFLAGS) \
		$(LDFLAGS) $^ $(LOADLIBES) \
		-o $@


# DO NOT DELETE THIS LINE -- make depend depends on it.

shrtFall.o: /usr/include/stdio.h /usr/include/features.h
shrtFall.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
shrtFall.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.3/include/stddef.h
shrtFall.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
shrtFall.o: /usr/include/bits/typesizes.h /usr/include/libio.h
shrtFall.o: /usr/include/_G_config.h /usr/include/wchar.h
shrtFall.o: /usr/include/bits/wchar.h /usr/include/gconv.h
shrtFall.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.3/include/stdarg.h
shrtFall.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
shrtFall.o: /usr/include/stdlib.h /usr/include/assert.h ../../wit/src/wit.h
shrtFall.o: /usr/include/limits.h
shrtFall.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.3/include/limits.h
shrtFall.o: ../../witutil/src/witU5.h ../../wit/src/wit.h
shrtFall.o: ../../witutil/src/machdep.h ../../wit/src/defines.h
shrtFll2.o: /usr/include/stdio.h /usr/include/features.h
shrtFll2.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
shrtFll2.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.3/include/stddef.h
shrtFll2.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
shrtFll2.o: /usr/include/bits/typesizes.h /usr/include/libio.h
shrtFll2.o: /usr/include/_G_config.h /usr/include/wchar.h
shrtFll2.o: /usr/include/bits/wchar.h /usr/include/gconv.h
shrtFll2.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.3/include/stdarg.h
shrtFll2.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
shrtFll2.o: /usr/include/stdlib.h /usr/include/assert.h ../../wit/src/wit.h
shrtFll2.o: /usr/include/limits.h
shrtFll2.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.3/include/limits.h
shrtFll2.o: ../../witutil/src/fullExpl.h ../../wit/src/wit.h
shrtFll2.o: ../../witutil/src/machdep.h ../../wit/src/defines.h
unitTest.o: /usr/include/assert.h /usr/include/features.h
unitTest.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
unitTest.o: ../../witutil/src/fullExpl.h ../../wit/src/wit.h
whereUse.o: /usr/include/stdio.h /usr/include/features.h
whereUse.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
whereUse.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.3/include/stddef.h
whereUse.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
whereUse.o: /usr/include/bits/typesizes.h /usr/include/libio.h
whereUse.o: /usr/include/_G_config.h /usr/include/wchar.h
whereUse.o: /usr/include/bits/wchar.h /usr/include/gconv.h
whereUse.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.3/include/stdarg.h
whereUse.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
whereUse.o: /usr/include/stdlib.h ../../witutil/src/machdep.h
whereUse.o: ../../wit/src/defines.h /usr/include/assert.h ../../wit/src/wit.h
whereUse.o: /usr/include/limits.h
whereUse.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.3/include/limits.h
whereUse.o: ../../witutil/src/fullExpl.h ../../wit/src/wit.h
aMrpMain.o: /usr/include/stdio.h /usr/include/features.h
aMrpMain.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
aMrpMain.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.3/include/stddef.h
aMrpMain.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
aMrpMain.o: /usr/include/bits/typesizes.h /usr/include/libio.h
aMrpMain.o: /usr/include/_G_config.h /usr/include/wchar.h
aMrpMain.o: /usr/include/bits/wchar.h /usr/include/gconv.h
aMrpMain.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.3/include/stdarg.h
aMrpMain.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
aMrpMain.o: /usr/include/stdlib.h /usr/include/assert.h ../../wit/src/wit.h
aMrpMain.o: /usr/include/limits.h
aMrpMain.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.3/include/limits.h
aMrpMain.o: ../src/./allocMrp.h ../../witutil/src/machdep.h
aMrpMain.o: ../../wit/src/defines.h
witU5.o: /usr/include/assert.h /usr/include/features.h
witU5.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
witU5.o: /usr/include/stdlib.h
witU5.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.3/include/stddef.h
witU5.o: /usr/include/math.h /usr/include/bits/huge_val.h
witU5.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
witU5.o: ../../witutil/src/witU5.h ../../wit/src/wit.h
witU5.o: ../../witutil/src/machdep.h ../../wit/src/defines.h
allocMrp.o: /usr/include/stdio.h /usr/include/features.h
allocMrp.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
allocMrp.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.3/include/stddef.h
allocMrp.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
allocMrp.o: /usr/include/bits/typesizes.h /usr/include/libio.h
allocMrp.o: /usr/include/_G_config.h /usr/include/wchar.h
allocMrp.o: /usr/include/bits/wchar.h /usr/include/gconv.h
allocMrp.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.3/include/stdarg.h
allocMrp.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
allocMrp.o: /usr/include/stdlib.h /usr/include/assert.h ../../wit/src/wit.h
allocMrp.o: ../../witutil/src/machdep.h ../../wit/src/defines.h
allocMrp.o: ../../witutil/src/allocMrp.h
fullExpl.o: ../../wit/src/wit.h ../../witutil/src/machdep.h
fullExpl.o: ../../wit/src/defines.h /usr/include/stdlib.h
fullExpl.o: /usr/include/features.h /usr/include/sys/cdefs.h
fullExpl.o: /usr/include/gnu/stubs.h
fullExpl.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.3/include/stddef.h
fullExpl.o: /usr/include/assert.h /usr/include/math.h
fullExpl.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
fullExpl.o: /usr/include/bits/mathcalls.h ../../witutil/src/fullExpl.h
