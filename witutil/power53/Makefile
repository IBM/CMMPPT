# Makefile for the program for this architecture

# $Header: /gsa/yktgsa/projects/w/wit/devbase/witutil/power53/Makefile,v 1.1 2006/04/19 21:50:30 rjw Exp $

platform = power53

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

shrtFall.o: /usr/vacpp/include/stdio.h /usr/include/stdio.h
shrtFall.o: /usr/include/standards.h /usr/include/va_list.h
shrtFall.o: /usr/include/sys/types.h /usr/include/strict_stdtypes.h
shrtFall.o: /usr/include/sys/inttypes.h /usr/include/stdint.h
shrtFall.o: /usr/include/end_strict_stdtypes.h /usr/include/sys/m_types.h
shrtFall.o: /usr/include/sys/vm_types.h /usr/include/sys/limits.h
shrtFall.o: /usr/include/float.h /usr/vacpp/include/stdlib.h
shrtFall.o: /usr/include/stdlib.h /usr/vacpp/include/sys/wait.h
shrtFall.o: /usr/include/sys/wait.h /usr/include/sys/resource.h
shrtFall.o: /usr/vacpp/include/sys/time.h /usr/include/sys/time.h
shrtFall.o: /usr/vacpp/include/sys/signal.h /usr/include/sys/signal.h
shrtFall.o: /usr/include/sys/context.h /usr/include/sys/m_param.h
shrtFall.o: /usr/include/sys/mstsave.h /usr/include/sys/m_signal.h
shrtFall.o: /usr/include/sys/localedef.h /usr/include/sys/lc_core.h
shrtFall.o: /usr/vacpp/include/locale.h /usr/include/locale.h
shrtFall.o: /usr/include/sys/localedef31.h /usr/vacpp/include/assert.h
shrtFall.o: /usr/include/assert.h ../../wit/src/wit.h /usr/include/limits.h
shrtFall.o: ../../witutil/src/witU5.h ../../wit/src/wit.h
shrtFall.o: ../../witutil/src/machdep.h ../../wit/src/defines.h
shrtFll2.o: /usr/vacpp/include/stdio.h /usr/include/stdio.h
shrtFll2.o: /usr/include/standards.h /usr/include/va_list.h
shrtFll2.o: /usr/include/sys/types.h /usr/include/strict_stdtypes.h
shrtFll2.o: /usr/include/sys/inttypes.h /usr/include/stdint.h
shrtFll2.o: /usr/include/end_strict_stdtypes.h /usr/include/sys/m_types.h
shrtFll2.o: /usr/include/sys/vm_types.h /usr/include/sys/limits.h
shrtFll2.o: /usr/include/float.h /usr/vacpp/include/stdlib.h
shrtFll2.o: /usr/include/stdlib.h /usr/vacpp/include/sys/wait.h
shrtFll2.o: /usr/include/sys/wait.h /usr/include/sys/resource.h
shrtFll2.o: /usr/vacpp/include/sys/time.h /usr/include/sys/time.h
shrtFll2.o: /usr/vacpp/include/sys/signal.h /usr/include/sys/signal.h
shrtFll2.o: /usr/include/sys/context.h /usr/include/sys/m_param.h
shrtFll2.o: /usr/include/sys/mstsave.h /usr/include/sys/m_signal.h
shrtFll2.o: /usr/include/sys/localedef.h /usr/include/sys/lc_core.h
shrtFll2.o: /usr/vacpp/include/locale.h /usr/include/locale.h
shrtFll2.o: /usr/include/sys/localedef31.h /usr/vacpp/include/assert.h
shrtFll2.o: /usr/include/assert.h ../../wit/src/wit.h /usr/include/limits.h
shrtFll2.o: ../../witutil/src/fullExpl.h ../../wit/src/wit.h
shrtFll2.o: ../../witutil/src/machdep.h ../../wit/src/defines.h
shrtFll2.o: /usr/vacpp/include/iostream /usr/vacpp/include/istream
shrtFll2.o: /usr/vacpp/include/ostream /usr/vacpp/include/yvals.h
shrtFll2.o: /usr/vacpp/include/ios /usr/vacpp/include/xlocnum
shrtFll2.o: /usr/vacpp/include/cerrno /usr/include/errno.h
shrtFll2.o: /usr/vacpp/include/climits /usr/vacpp/include/cstdio
shrtFll2.o: /usr/vacpp/include/cstdlib /usr/vacpp/include/streambuf
shrtFll2.o: /usr/vacpp/include/xiosbase /usr/vacpp/include/xlocale
shrtFll2.o: /usr/vacpp/include/cstring /usr/vacpp/include/string.h
shrtFll2.o: /usr/include/string.h /usr/vacpp/include/stdexcept
shrtFll2.o: /usr/vacpp/include/exception /usr/vacpp/include/xstddef
shrtFll2.o: /usr/vacpp/include/cstddef /usr/vacpp/include/stddef.h
shrtFll2.o: /usr/include/stddef.h /usr/vacpp/include/xstring
shrtFll2.o: /usr/vacpp/include/xmemory /usr/vacpp/include/new
shrtFll2.o: /usr/vacpp/include/xutility /usr/vacpp/include/utility
shrtFll2.o: /usr/vacpp/include/iosfwd /usr/vacpp/include/cwchar
shrtFll2.o: /usr/include/wchar.h /usr/include/time.h
shrtFll2.o: /usr/vacpp/include/xutility.t /usr/vacpp/include/xstring.t
shrtFll2.o: /usr/vacpp/include/typeinfo /usr/vacpp/include/xlocinfo
shrtFll2.o: /usr/vacpp/include/xlocinfo.h /usr/include/ctype.h
shrtFll2.o: /usr/vacpp/include/xlocinfo.t /usr/vacpp/include/ostream.t
shrtFll2.o: /usr/vacpp/include/istream.t /usr/vacpp/include/fstream
shrtFll2.o: /usr/vacpp/include/fstream.t
unitTest.o: /usr/vacpp/include/assert.h /usr/include/assert.h
unitTest.o: /usr/include/standards.h /usr/vacpp/include/iostream
unitTest.o: /usr/vacpp/include/istream /usr/vacpp/include/ostream
unitTest.o: /usr/vacpp/include/yvals.h /usr/vacpp/include/ios
unitTest.o: /usr/vacpp/include/xlocnum /usr/vacpp/include/cerrno
unitTest.o: /usr/include/errno.h /usr/vacpp/include/climits
unitTest.o: /usr/include/limits.h /usr/include/sys/limits.h
unitTest.o: /usr/include/float.h /usr/vacpp/include/cstdio
unitTest.o: /usr/vacpp/include/stdio.h /usr/include/stdio.h
unitTest.o: /usr/include/va_list.h /usr/include/sys/types.h
unitTest.o: /usr/include/strict_stdtypes.h /usr/include/sys/inttypes.h
unitTest.o: /usr/include/stdint.h /usr/include/end_strict_stdtypes.h
unitTest.o: /usr/include/sys/m_types.h /usr/include/sys/vm_types.h
unitTest.o: /usr/vacpp/include/cstdlib /usr/vacpp/include/stdlib.h
unitTest.o: /usr/include/stdlib.h /usr/vacpp/include/sys/wait.h
unitTest.o: /usr/include/sys/wait.h /usr/include/sys/resource.h
unitTest.o: /usr/vacpp/include/sys/time.h /usr/include/sys/time.h
unitTest.o: /usr/vacpp/include/sys/signal.h /usr/include/sys/signal.h
unitTest.o: /usr/include/sys/context.h /usr/include/sys/m_param.h
unitTest.o: /usr/include/sys/mstsave.h /usr/include/sys/m_signal.h
unitTest.o: /usr/include/sys/localedef.h /usr/include/sys/lc_core.h
unitTest.o: /usr/vacpp/include/locale.h /usr/include/locale.h
unitTest.o: /usr/include/sys/localedef31.h /usr/vacpp/include/streambuf
unitTest.o: /usr/vacpp/include/xiosbase /usr/vacpp/include/xlocale
unitTest.o: /usr/vacpp/include/cstring /usr/vacpp/include/string.h
unitTest.o: /usr/include/string.h /usr/vacpp/include/stdexcept
unitTest.o: /usr/vacpp/include/exception /usr/vacpp/include/xstddef
unitTest.o: /usr/vacpp/include/cstddef /usr/vacpp/include/stddef.h
unitTest.o: /usr/include/stddef.h /usr/vacpp/include/xstring
unitTest.o: /usr/vacpp/include/xmemory /usr/vacpp/include/new
unitTest.o: /usr/vacpp/include/xutility /usr/vacpp/include/utility
unitTest.o: /usr/vacpp/include/iosfwd /usr/vacpp/include/cwchar
unitTest.o: /usr/include/wchar.h /usr/include/time.h
unitTest.o: /usr/vacpp/include/xutility.t /usr/vacpp/include/xstring.t
unitTest.o: /usr/vacpp/include/typeinfo /usr/vacpp/include/xlocinfo
unitTest.o: /usr/vacpp/include/xlocinfo.h /usr/include/ctype.h
unitTest.o: /usr/vacpp/include/xlocinfo.t /usr/vacpp/include/ostream.t
unitTest.o: /usr/vacpp/include/istream.t /usr/vacpp/include/fstream
unitTest.o: /usr/vacpp/include/fstream.t /usr/vacpp/include/string
unitTest.o: /usr/vacpp/include/string.t ../../witutil/src/fullExpl.h
unitTest.o: ../../wit/src/wit.h
whereUse.o: /usr/vacpp/include/stdio.h /usr/include/stdio.h
whereUse.o: /usr/include/standards.h /usr/include/va_list.h
whereUse.o: /usr/include/sys/types.h /usr/include/strict_stdtypes.h
whereUse.o: /usr/include/sys/inttypes.h /usr/include/stdint.h
whereUse.o: /usr/include/end_strict_stdtypes.h /usr/include/sys/m_types.h
whereUse.o: /usr/include/sys/vm_types.h /usr/include/sys/limits.h
whereUse.o: /usr/include/float.h /usr/vacpp/include/stdlib.h
whereUse.o: /usr/include/stdlib.h /usr/vacpp/include/sys/wait.h
whereUse.o: /usr/include/sys/wait.h /usr/include/sys/resource.h
whereUse.o: /usr/vacpp/include/sys/time.h /usr/include/sys/time.h
whereUse.o: /usr/vacpp/include/sys/signal.h /usr/include/sys/signal.h
whereUse.o: /usr/include/sys/context.h /usr/include/sys/m_param.h
whereUse.o: /usr/include/sys/mstsave.h /usr/include/sys/m_signal.h
whereUse.o: /usr/include/sys/localedef.h /usr/include/sys/lc_core.h
whereUse.o: /usr/vacpp/include/locale.h /usr/include/locale.h
whereUse.o: /usr/include/sys/localedef31.h ../../witutil/src/machdep.h
whereUse.o: ../../wit/src/defines.h /usr/vacpp/include/iostream
whereUse.o: /usr/vacpp/include/istream /usr/vacpp/include/ostream
whereUse.o: /usr/vacpp/include/yvals.h /usr/vacpp/include/ios
whereUse.o: /usr/vacpp/include/xlocnum /usr/vacpp/include/cerrno
whereUse.o: /usr/include/errno.h /usr/vacpp/include/climits
whereUse.o: /usr/include/limits.h /usr/vacpp/include/cstdio
whereUse.o: /usr/vacpp/include/cstdlib /usr/vacpp/include/streambuf
whereUse.o: /usr/vacpp/include/xiosbase /usr/vacpp/include/xlocale
whereUse.o: /usr/vacpp/include/cstring /usr/vacpp/include/string.h
whereUse.o: /usr/include/string.h /usr/vacpp/include/stdexcept
whereUse.o: /usr/vacpp/include/exception /usr/vacpp/include/xstddef
whereUse.o: /usr/vacpp/include/cstddef /usr/vacpp/include/stddef.h
whereUse.o: /usr/include/stddef.h /usr/vacpp/include/xstring
whereUse.o: /usr/vacpp/include/xmemory /usr/vacpp/include/new
whereUse.o: /usr/vacpp/include/xutility /usr/vacpp/include/utility
whereUse.o: /usr/vacpp/include/iosfwd /usr/vacpp/include/cwchar
whereUse.o: /usr/include/wchar.h /usr/include/time.h
whereUse.o: /usr/vacpp/include/xutility.t /usr/vacpp/include/xstring.t
whereUse.o: /usr/vacpp/include/typeinfo /usr/vacpp/include/xlocinfo
whereUse.o: /usr/vacpp/include/xlocinfo.h /usr/include/ctype.h
whereUse.o: /usr/vacpp/include/xlocinfo.t /usr/vacpp/include/ostream.t
whereUse.o: /usr/vacpp/include/istream.t /usr/vacpp/include/fstream
whereUse.o: /usr/vacpp/include/fstream.t /usr/vacpp/include/assert.h
whereUse.o: /usr/include/assert.h ../../wit/src/wit.h
whereUse.o: ../../witutil/src/fullExpl.h ../../wit/src/wit.h
aMrpMain.o: /usr/vacpp/include/stdio.h /usr/include/stdio.h
aMrpMain.o: /usr/include/standards.h /usr/include/va_list.h
aMrpMain.o: /usr/include/sys/types.h /usr/include/strict_stdtypes.h
aMrpMain.o: /usr/include/sys/inttypes.h /usr/include/stdint.h
aMrpMain.o: /usr/include/end_strict_stdtypes.h /usr/include/sys/m_types.h
aMrpMain.o: /usr/include/sys/vm_types.h /usr/include/sys/limits.h
aMrpMain.o: /usr/include/float.h /usr/vacpp/include/stdlib.h
aMrpMain.o: /usr/include/stdlib.h /usr/vacpp/include/sys/wait.h
aMrpMain.o: /usr/include/sys/wait.h /usr/include/sys/resource.h
aMrpMain.o: /usr/vacpp/include/sys/time.h /usr/include/sys/time.h
aMrpMain.o: /usr/vacpp/include/sys/signal.h /usr/include/sys/signal.h
aMrpMain.o: /usr/include/sys/context.h /usr/include/sys/m_param.h
aMrpMain.o: /usr/include/sys/mstsave.h /usr/include/sys/m_signal.h
aMrpMain.o: /usr/include/sys/localedef.h /usr/include/sys/lc_core.h
aMrpMain.o: /usr/vacpp/include/locale.h /usr/include/locale.h
aMrpMain.o: /usr/include/sys/localedef31.h /usr/vacpp/include/assert.h
aMrpMain.o: /usr/include/assert.h ../../wit/src/wit.h /usr/include/limits.h
aMrpMain.o: ../src/./allocMrp.h /usr/vacpp/include/string
aMrpMain.o: /usr/vacpp/include/yvals.h /usr/vacpp/include/istream
aMrpMain.o: /usr/vacpp/include/ostream /usr/vacpp/include/ios
aMrpMain.o: /usr/vacpp/include/xlocnum /usr/vacpp/include/cerrno
aMrpMain.o: /usr/include/errno.h /usr/vacpp/include/climits
aMrpMain.o: /usr/vacpp/include/cstdio /usr/vacpp/include/cstdlib
aMrpMain.o: /usr/vacpp/include/streambuf /usr/vacpp/include/xiosbase
aMrpMain.o: /usr/vacpp/include/xlocale /usr/vacpp/include/cstring
aMrpMain.o: /usr/vacpp/include/string.h /usr/include/string.h
aMrpMain.o: /usr/vacpp/include/stdexcept /usr/vacpp/include/exception
aMrpMain.o: /usr/vacpp/include/xstddef /usr/vacpp/include/cstddef
aMrpMain.o: /usr/vacpp/include/stddef.h /usr/include/stddef.h
aMrpMain.o: /usr/vacpp/include/xstring /usr/vacpp/include/xmemory
aMrpMain.o: /usr/vacpp/include/new /usr/vacpp/include/xutility
aMrpMain.o: /usr/vacpp/include/utility /usr/vacpp/include/iosfwd
aMrpMain.o: /usr/vacpp/include/cwchar /usr/include/wchar.h
aMrpMain.o: /usr/include/time.h /usr/vacpp/include/xutility.t
aMrpMain.o: /usr/vacpp/include/xstring.t /usr/vacpp/include/typeinfo
aMrpMain.o: /usr/vacpp/include/xlocinfo /usr/vacpp/include/xlocinfo.h
aMrpMain.o: /usr/include/ctype.h /usr/vacpp/include/xlocinfo.t
aMrpMain.o: /usr/vacpp/include/ostream.t /usr/vacpp/include/istream.t
aMrpMain.o: /usr/vacpp/include/string.t ../../witutil/src/machdep.h
aMrpMain.o: ../../wit/src/defines.h /usr/vacpp/include/iostream
aMrpMain.o: /usr/vacpp/include/fstream /usr/vacpp/include/fstream.t
witU5.o: /usr/vacpp/include/assert.h /usr/include/assert.h
witU5.o: /usr/include/standards.h /usr/vacpp/include/stdlib.h
witU5.o: /usr/include/stdlib.h /usr/vacpp/include/sys/wait.h
witU5.o: /usr/include/sys/wait.h /usr/include/strict_stdtypes.h
witU5.o: /usr/include/end_strict_stdtypes.h /usr/include/sys/resource.h
witU5.o: /usr/vacpp/include/sys/time.h /usr/include/sys/time.h
witU5.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
witU5.o: /usr/include/stdint.h /usr/include/sys/m_types.h
witU5.o: /usr/include/sys/vm_types.h /usr/vacpp/include/sys/signal.h
witU5.o: /usr/include/sys/signal.h /usr/include/sys/context.h
witU5.o: /usr/include/sys/m_param.h /usr/include/sys/mstsave.h
witU5.o: /usr/include/sys/m_signal.h /usr/include/sys/localedef.h
witU5.o: /usr/include/sys/limits.h /usr/include/float.h
witU5.o: /usr/include/sys/lc_core.h /usr/vacpp/include/locale.h
witU5.o: /usr/include/locale.h /usr/include/sys/localedef31.h
witU5.o: /usr/vacpp/include/math.h /usr/include/errno.h /usr/include/math.h
witU5.o: ../../witutil/src/witU5.h ../../wit/src/wit.h
witU5.o: ../../witutil/src/machdep.h ../../wit/src/defines.h
witU5.o: /usr/vacpp/include/iostream /usr/vacpp/include/istream
witU5.o: /usr/vacpp/include/ostream /usr/vacpp/include/yvals.h
witU5.o: /usr/vacpp/include/ios /usr/vacpp/include/xlocnum
witU5.o: /usr/vacpp/include/cerrno /usr/vacpp/include/climits
witU5.o: /usr/include/limits.h /usr/vacpp/include/cstdio
witU5.o: /usr/vacpp/include/stdio.h /usr/include/stdio.h
witU5.o: /usr/include/va_list.h /usr/vacpp/include/cstdlib
witU5.o: /usr/vacpp/include/streambuf /usr/vacpp/include/xiosbase
witU5.o: /usr/vacpp/include/xlocale /usr/vacpp/include/cstring
witU5.o: /usr/vacpp/include/string.h /usr/include/string.h
witU5.o: /usr/vacpp/include/stdexcept /usr/vacpp/include/exception
witU5.o: /usr/vacpp/include/xstddef /usr/vacpp/include/cstddef
witU5.o: /usr/vacpp/include/stddef.h /usr/include/stddef.h
witU5.o: /usr/vacpp/include/xstring /usr/vacpp/include/xmemory
witU5.o: /usr/vacpp/include/new /usr/vacpp/include/xutility
witU5.o: /usr/vacpp/include/utility /usr/vacpp/include/iosfwd
witU5.o: /usr/vacpp/include/cwchar /usr/include/wchar.h /usr/include/time.h
witU5.o: /usr/vacpp/include/xutility.t /usr/vacpp/include/xstring.t
witU5.o: /usr/vacpp/include/typeinfo /usr/vacpp/include/xlocinfo
witU5.o: /usr/vacpp/include/xlocinfo.h /usr/include/ctype.h
witU5.o: /usr/vacpp/include/xlocinfo.t /usr/vacpp/include/ostream.t
witU5.o: /usr/vacpp/include/istream.t
allocMrp.o: /usr/vacpp/include/stdio.h /usr/include/stdio.h
allocMrp.o: /usr/include/standards.h /usr/include/va_list.h
allocMrp.o: /usr/include/sys/types.h /usr/include/strict_stdtypes.h
allocMrp.o: /usr/include/sys/inttypes.h /usr/include/stdint.h
allocMrp.o: /usr/include/end_strict_stdtypes.h /usr/include/sys/m_types.h
allocMrp.o: /usr/include/sys/vm_types.h /usr/include/sys/limits.h
allocMrp.o: /usr/include/float.h /usr/vacpp/include/stdlib.h
allocMrp.o: /usr/include/stdlib.h /usr/vacpp/include/sys/wait.h
allocMrp.o: /usr/include/sys/wait.h /usr/include/sys/resource.h
allocMrp.o: /usr/vacpp/include/sys/time.h /usr/include/sys/time.h
allocMrp.o: /usr/vacpp/include/sys/signal.h /usr/include/sys/signal.h
allocMrp.o: /usr/include/sys/context.h /usr/include/sys/m_param.h
allocMrp.o: /usr/include/sys/mstsave.h /usr/include/sys/m_signal.h
allocMrp.o: /usr/include/sys/localedef.h /usr/include/sys/lc_core.h
allocMrp.o: /usr/vacpp/include/locale.h /usr/include/locale.h
allocMrp.o: /usr/include/sys/localedef31.h /usr/vacpp/include/assert.h
allocMrp.o: /usr/include/assert.h ../../wit/src/wit.h
allocMrp.o: ../../witutil/src/machdep.h ../../wit/src/defines.h
allocMrp.o: ../../witutil/src/allocMrp.h /usr/vacpp/include/string
allocMrp.o: /usr/vacpp/include/yvals.h /usr/vacpp/include/istream
allocMrp.o: /usr/vacpp/include/ostream /usr/vacpp/include/ios
allocMrp.o: /usr/vacpp/include/xlocnum /usr/vacpp/include/cerrno
allocMrp.o: /usr/include/errno.h /usr/vacpp/include/climits
allocMrp.o: /usr/include/limits.h /usr/vacpp/include/cstdio
allocMrp.o: /usr/vacpp/include/cstdlib /usr/vacpp/include/streambuf
allocMrp.o: /usr/vacpp/include/xiosbase /usr/vacpp/include/xlocale
allocMrp.o: /usr/vacpp/include/cstring /usr/vacpp/include/string.h
allocMrp.o: /usr/include/string.h /usr/vacpp/include/stdexcept
allocMrp.o: /usr/vacpp/include/exception /usr/vacpp/include/xstddef
allocMrp.o: /usr/vacpp/include/cstddef /usr/vacpp/include/stddef.h
allocMrp.o: /usr/include/stddef.h /usr/vacpp/include/xstring
allocMrp.o: /usr/vacpp/include/xmemory /usr/vacpp/include/new
allocMrp.o: /usr/vacpp/include/xutility /usr/vacpp/include/utility
allocMrp.o: /usr/vacpp/include/iosfwd /usr/vacpp/include/cwchar
allocMrp.o: /usr/include/wchar.h /usr/include/time.h
allocMrp.o: /usr/vacpp/include/xutility.t /usr/vacpp/include/xstring.t
allocMrp.o: /usr/vacpp/include/typeinfo /usr/vacpp/include/xlocinfo
allocMrp.o: /usr/vacpp/include/xlocinfo.h /usr/include/ctype.h
allocMrp.o: /usr/vacpp/include/xlocinfo.t /usr/vacpp/include/ostream.t
allocMrp.o: /usr/vacpp/include/istream.t /usr/vacpp/include/string.t
fullExpl.o: ../../wit/src/wit.h ../../witutil/src/machdep.h
fullExpl.o: ../../wit/src/defines.h /usr/vacpp/include/stdlib.h
fullExpl.o: /usr/include/stdlib.h /usr/vacpp/include/sys/wait.h
fullExpl.o: /usr/include/sys/wait.h /usr/include/strict_stdtypes.h
fullExpl.o: /usr/include/end_strict_stdtypes.h /usr/include/sys/resource.h
fullExpl.o: /usr/vacpp/include/sys/time.h /usr/include/sys/time.h
fullExpl.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
fullExpl.o: /usr/include/stdint.h /usr/include/standards.h
fullExpl.o: /usr/include/sys/m_types.h /usr/include/sys/vm_types.h
fullExpl.o: /usr/vacpp/include/sys/signal.h /usr/include/sys/signal.h
fullExpl.o: /usr/include/sys/context.h /usr/include/sys/m_param.h
fullExpl.o: /usr/include/sys/mstsave.h /usr/include/sys/m_signal.h
fullExpl.o: /usr/include/sys/localedef.h /usr/include/sys/limits.h
fullExpl.o: /usr/include/float.h /usr/include/sys/lc_core.h
fullExpl.o: /usr/vacpp/include/locale.h /usr/include/locale.h
fullExpl.o: /usr/include/sys/localedef31.h /usr/vacpp/include/assert.h
fullExpl.o: /usr/include/assert.h /usr/vacpp/include/math.h
fullExpl.o: /usr/include/errno.h /usr/include/math.h
fullExpl.o: ../../witutil/src/fullExpl.h /usr/vacpp/include/iostream
fullExpl.o: /usr/vacpp/include/istream /usr/vacpp/include/ostream
fullExpl.o: /usr/vacpp/include/yvals.h /usr/vacpp/include/ios
fullExpl.o: /usr/vacpp/include/xlocnum /usr/vacpp/include/cerrno
fullExpl.o: /usr/vacpp/include/climits /usr/include/limits.h
fullExpl.o: /usr/vacpp/include/cstdio /usr/vacpp/include/stdio.h
fullExpl.o: /usr/include/stdio.h /usr/include/va_list.h
fullExpl.o: /usr/vacpp/include/cstdlib /usr/vacpp/include/streambuf
fullExpl.o: /usr/vacpp/include/xiosbase /usr/vacpp/include/xlocale
fullExpl.o: /usr/vacpp/include/cstring /usr/vacpp/include/string.h
fullExpl.o: /usr/include/string.h /usr/vacpp/include/stdexcept
fullExpl.o: /usr/vacpp/include/exception /usr/vacpp/include/xstddef
fullExpl.o: /usr/vacpp/include/cstddef /usr/vacpp/include/stddef.h
fullExpl.o: /usr/include/stddef.h /usr/vacpp/include/xstring
fullExpl.o: /usr/vacpp/include/xmemory /usr/vacpp/include/new
fullExpl.o: /usr/vacpp/include/xutility /usr/vacpp/include/utility
fullExpl.o: /usr/vacpp/include/iosfwd /usr/vacpp/include/cwchar
fullExpl.o: /usr/include/wchar.h /usr/include/time.h
fullExpl.o: /usr/vacpp/include/xutility.t /usr/vacpp/include/xstring.t
fullExpl.o: /usr/vacpp/include/typeinfo /usr/vacpp/include/xlocinfo
fullExpl.o: /usr/vacpp/include/xlocinfo.h /usr/include/ctype.h
fullExpl.o: /usr/vacpp/include/xlocinfo.t /usr/vacpp/include/ostream.t
fullExpl.o: /usr/vacpp/include/istream.t /usr/vacpp/include/fstream
fullExpl.o: /usr/vacpp/include/fstream.t
