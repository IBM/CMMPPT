#!/bin/ksh
# This needs to be "source"d in ksh (and probably sh) after you have done setup.sh
# DS is short for Decision Support
#
# $Id: setvars.ksh.sh,v 1.3 2001/12/05 19:56:15 fasano Exp $


if [[ -n $HOST ]]
then
    # HOST environment variable is set, make sure the value is what
    # the makefiles expect
    if [[ $HOST != $(hostname) ]]
    then
        print 'Warning:  $HOST has an unexpected value, make may not work'
        print '          See config/powerGen.mk'
    fi
else
    # Set the HOST environment variable
    export HOST=$(hostname)
fi


# Set name of prefered xlC compile server if not already defined
if [[ -z $XLC_SERVER ]]
then
   export XLC_SERVER=woodenshoe
fi

export     DS_DIR=/u/wit/
#export   DS_A_DIR=/afs/watson.ibm.com/u/I/implode/
export   DS_A_DIR=$DS_DIR
export    DSD_DIR=${DS_DIR}devbase/
export DSCONF_DIR=${DSD_DIR}config/
export  DSWIT_DIR=${DSD_DIR}wit/
export DSPMRP_DIR=${DSD_DIR}pmrp/

# The following sample line can be put into your own profile to control
# distCompile (used by parComp in our makefiles)
# export DS_SERVER_POOL='woodenshoe habanero'

# There should be power, sun4, etc. directories under RW_ROOT_DIR and there
# should be lib and include/rw directories under them for the librw*.a and
# *.h files respectively
if [[ $(hostname) = "mfglog2.watson.ibm.com" ]]
then
   export     RW_ROOT_DIR=${DS_DIR}
   export     OSL_ROOT_DIR=${DS_DIR}
else
  if [[ $(hostname) = "wit" ]]
  then
   export     RW_ROOT_DIR=/u/witdemo1/
   export     OSL_ROOT_DIR=/u/witdemo1/
  else
   export     RW_ROOT_DIR=${DS_A_DIR}
   export     OSL_ROOT_DIR=${DS_A_DIR}
  fi
fi

# Where the PRM makefiles will look for libraries people usually make
# for themselves:
# The libraries will be looked for in subdirectories of these directory.
# You can change these after you source this file if you want the makefiles
# to look somewhere else.
export WIT_ROOT_DIR=/u/wit/$USER/wit
export MCL_ROOT_DIR=/u/wit/$USER/mcl
export SCENARIO_ROOT_DIR=/u/wit/$USER/scenario

# Where we get "makedepend".
# Note: This expression evaluates to true if the
# host-name ends in ".watson.ibm.com".
if [[ $(hostname) = *.watson.ibm.com ]]
then
#       The links Agora has set up handle different machine architectures
        export X11R5_BIN_DIR=/usr/lpp/X11/bin/
else
  if [[ $(hostname) = "wit" ]]
  then
        export X11R5_BIN_DIR=/usr/local/X11R5/bin/
  else
#       Go with something hard-coded
        export X11R5_BIN_DIR=/afs/watson.ibm.com/agora/share/power/X11R5/bin/
  fi
fi


# Used by Concurrent Version System
export CVSROOT=${DS_DIR}devbase
export RCSBIN=/usr/agora/bin

# Used by SCCS
#export PROJECTDIR=${DS_DIR}wit

#Used by Purify on Suns
#Allow user to have extra purify options, such as "mail-to-user=wally"
#or "inuse-at-exit=yes"

# csh method
#if ($?PURIFYOPTIONS == 0) then  purify option variable undefined?
#    setenv PURIFYOPTIONS ""     define it
#endif
#setenv PURIFYOPTIONS \
# "$PURIFYOPTIONS -auto-mount-prefix=/a -free-queue-length=1000 -chain-length=20"
export PURIFYOPTIONS="$PURIFYOPTIONS -auto-mount-prefix=/a -free-queue-length=1000 -chain-length=20"

# Used by Sentinel on RS/6Ks
#export SE_ENABLED="yes"

umask 0007
#export PATH=".:${DS_DIR}wit/scripts:${DS_DIR}bin:$PATH"
export PATH=".:${DS_DIR}bin:$PATH"
