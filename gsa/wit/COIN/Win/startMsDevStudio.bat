REM This bat file is an attempt to start MS Visual Studio
REM Version 6 with a workspace and project files for
REM use with COIN. You should use this bat file to 
REM start MS Visual Studio after editing this file as
REM indicated below.
REM 
REM
REM COIN can be used with many optional solvers that 
REM are external to COIN.
REM 1. Environment variables are set in this bat file
REM    to indicate where these optional solvers are 
REM    on your computer.
REM 2. For some projects, compiler preprocessor symbols
REM    COIN_USE_solver must be defined (or undefined)
REM    in the development studio to indicate which
REM    solver(s) the project is using.
REM 3. As source files get added(or deleted), then the
REM    files belonging to projects will need to updated.
REM    There is not an automatic way to keep the MS project
REM    files updated. If a file is missing when building a 
REM    project, then the file may have been removed from COIN.
REM    If when linking there are undefined symbols, then 
REM    there may be new files which need to be added to 
REM    a project.
REM    
REM
REM -------------------------------------------------
REM Set path to solvers.
REM The Open Solver Interface (OSI) can be built to
REM interface with different solvers.  
REM For the solvers you choose to build OSI with
REM Microsoft Development Studio needs to know these
REM solvers are located on your computer.
REM Set the following variables for the solvers
REM you wish to build OSI with.
REM
REM XxxIncDir is the directory containing the 
REM    include files for solver Xxx.
REM    This symbol is used by project oslXxxLib
REM    and osiUnitTest.
REM XxxLibDir is the directory containing the 
REM    library for solver Xxx.
REM    This symbol is used by project osiUnitTest.
REM XxxLibName is the name of the library for 
REM    solver Xxx.
REM    This symbol is used by project osiUnitTest.
REM
REM Note: It is only necessary to set these 
REM enviromnent variables for solvers that 
REM are not part of COIN. So it it is not necessary
REM to set variables for Clp and Vol solvers.
REM -------------------------------------------------
set OslIncDir=e:\osl\o_osl_v2_h
set OslLibDir=e:\osl\lib
set OslLibName=oslmd6030.lib

set XprIncDir=f:\xpressmp\include
set XprLibDir=f:\xpressmp\lib
set XprLibName=xprs.lib

set GlpkIncDir=e:\glpk-3.2.3\include
set GlpkLibDir=e:\glpk-3.2.3\mds\glpkLib\Release
set GlpkLibName=glpkLib.lib

set DylpIncDir1=e:\dylp\OsiDylp
set DylpIncDir2=e:\dylp\DylpLib
set DylpLibDir1=e:\dylp\mds\OsiDylp\Release
set DylpLibDir2=e:\dylp\mds\DylpLib\Release
set DylpLibNames=osiDylp.lib dylpLib.lib

set SpxIncDir=e:\soplex-1.2.1\src
set SpxLibDir=e:\soplex-1.2.1\mds\soplexLib\Release
set SpxLibName=soplexLib.lib

set CpxIncDir=F:\ILOG\cplex81\include\ilcplex
set CpxLibDir=F:\ILOG\cplex81\lib\msvc6\stat_st
set CpxBinDir=F:\ILOG\cplex81\bin\msvc6
set CpxLibName=cplex81.lib

REM -------------------------------------------------
REM Set path to solver dll's
REM -------------------------------------------------
set PATH=%CpxBinDir%;%OslLibDir%;%PATH%
echo %PATH%

REM -------------------------------------------------
REM Start MS Visual Studio specifying the workspace
REM file.
REM -------------------------------------------------
Rem vcvars32
msdev coin.dsw

REM -------------------------------------------------
REM Some fields need to be set in the development
REM studio to specify your solvers when using OSI.
REM When building osiUnitTest then the preprocessor
REM symbol COIN_USE_XXXX must be defined.
REM XXXX indicates the solver(s) you are using.
REM Some choices are:
REM   COIN_USE_VOL 
REM   COIN_USE_OSL 
REM   COIN_USE_XPR 
REM   COIN_USE_CPX 
REM   COIN_USE_CPL  
REM   COIN_USE_GLPK  
REM   COIN_USE_DYLP
REM   COIN_USE_SPX
REM From within the development studio select
REM Project, Settings, C++, Preprocssor definitions
REM define solver(s) to be used.
