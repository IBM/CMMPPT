# This needs to be "source"d in csh or tcsh after you have done setup.sh
# DS is short for Decision Support
#
# $Id: setvars.csh.sh,v 1.3 2001/12/05 19:56:14 fasano Exp $

if ($?HOST) then
    # HOST environment variable is set, make sure the value is what
    # the makefiles expect
    if ($HOST != `hostname`) then
	echo 'Warning: 	$HOST has an unexpected value, make may not work'
	echo '          See config/powerGen.mk'
    endif
else
    # Set the HOST environment variable
    setenv HOST `hostname`
endif


#skip for roch 
if (`hostname` =~ *.watson.ibm.com || `hostname` == wit) then

  if (! $?XLC_SERVER) then
    setenv XLC_SERVER       woodenshoe
  endif

  # Set DB2 environment variables (overridden below, for wit.watson.ibm.com)
  if (! $?DB2INSTANCE) then
    setenv DB2INSTANCE      kon2	# on autoclock, has Distributed
					# Database Connection Services
#   setenv DB2INSTANCE      witdb2	# on habanero
  endif

endif

if (`hostname` =~ *.watson.ibm.com || `hostname` == wit) then
  setenv DS_DIR 		/u/wit/
  setenv DS_A_DIR		${DS_A_DIR}
else 
  setenv DS_DIR 		${HOME}/dev/
  setenv DS_A_DIR		${HOME}/dev/
endif

setenv DSD_DIR		${DS_DIR}devbase/
setenv DSCONF_DIR 	${DSD_DIR}config/
setenv DSWIT_DIR 	${DSD_DIR}wit/
setenv DSPMRP_DIR 	${DSD_DIR}pmrp/

# The following sample line can be put into your own profile to control
# distCompile (used by parComp in our makefiles)
# setenv DS_SERVER_POOL   'ancho woodenshoe habanero'

# There should be power, sun4, etc. directories under RW_ROOT_DIR and there
# should be lib and include/rw directories under them for the librw*.a and
# *.h files respectively
if (`hostname` =~ *.watson.ibm.com || `hostname` == wit) then
  if (`hostname` == mfglog2.watson.ibm.com) then
  	setenv RW_ROOT_DIR	${DS_DIR}
	setenv OSL_ROOT_DIR	${DS_DIR}
  else
    if (`hostname` == wit) then
	setenv RW_ROOT_DIR	/u/witdemo1/
	setenv OSL_ROOT_DIR	/u/witdemo1/
        setenv DB2INSTANCE      witdemo1
    else
	setenv RW_ROOT_DIR	${DS_A_DIR}
	setenv OSL_ROOT_DIR	${DS_A_DIR}
    endif
  endif
else 
	#rochester stuff
	setenv RW_ROOT_DIR	/afs/rchland.ibm.com/usr5/prm/implodeWork/
	setenv OSL_ROOT_DIR	/afs/watson.ibm.com/u/I/wit/
endif

# Where the PRM makefiles will look for libraries people usually make
# for themselves:
# The libraries will be looked for in subdirectories of these directory.
# You can change these after you source this file if you want the makefiles
# to look somewhere else.
setenv WIT_ROOT_DIR /u/wit/$USER/wit
setenv SCENARIO_ROOT_DIR /u/wit/$USER/scenario

# Where we get "makedepend".  
# Note: This expression evaluates to true if the 
# host-name ends in ".watson.ibm.com".  The right-hand side of this expression
# must not be quoted; if it is, csh will not give '*' it's special meaning.
if (`hostname` =~ *.watson.ibm.com) then
	# The links Agora has set up handle different machine architectures
	setenv X11R5_BIN_DIR	/usr/lpp/X11/bin/
else
  if (`hostname` == wit || `hostname` == wit.watson.ibm.com) then
	# Go with something hard-coded
	setenv X11R5_BIN_DIR	/afs/watson.ibm.com/agora/share/power/X11R5/bin/
  else 	
	setenv X11R5_BIN_DIR	/afs/rchland.ibm.com/usr7/huizenga/bin/
  endif

endif


# Used by Concurrent Version System
if (`hostname` =~ *.watson.ibm.com || `hostname` == wit) then
  setenv CVSROOT ${DS_DIR}devbase	
  setenv RCSBIN /usr/agora/bin

  # Used by Objectory
  setenv ORYHOME    ${DS_A_DIR}orySE
  setenv ORYPROJDIR ${DS_DIR}joint/ooad
  setenv MANPATH    ${ORYHOME}/man:$MANPATH
  setenv LSHOST mfglog2.watson.ibm.com

  # Used by SCCS
  setenv PROJECTDIR ${DS_DIR}wit

  # Used by Purify on Suns
  # Allow user to have extra purify options, such as "mail-to-user=wally" 
  # or "inuse-at-exit=yes"
  if ($?PURIFYOPTIONS == 0) then	# purify option variable undefined?
      setenv PURIFYOPTIONS ""	# define it
  endif
  setenv PURIFYOPTIONS \
   "$PURIFYOPTIONS -auto-mount-prefix=/a -free-queue-length=1000 -chain-length=20"

  # Used by Sentinel on RS/6Ks
  setenv SE_ENABLED yes
endif

umask 0007

if (`hostname` =~ *.watson.ibm.com || `hostname` == wit) then
    set path = (. ${DS_DIR}orySE/bin ${DS_DIR}wit/scripts ${DS_DIR}bin $path)
else
    #roch needs DS_DIR}bin in path should be /afs.watson.ibm/u/I/wit/bin, this is production version 
    set path = (. ${DS_DIR}bin $path)
endif
