# Microsoft Developer Studio Project File - Name="clpLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=clpLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "clpLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "clpLib.mak" CFG="clpLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "clpLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "clpLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "clpLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /O2 /I "..\..\clp\include" /I "..\..\coin\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "clpLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /I "..\..\clp\include" /I "..\..\coin\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "clpLib - Win32 Release"
# Name "clpLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Clp\ClpDualRowDantzig.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpDualRowPivot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpDualRowSteepest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpFactorization.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpMatrixBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpNonLinearCost.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpPackedMatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpPrimalColumnDantzig.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpPrimalColumnPivot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpPrimalColumnSteepest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpSimplex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpSimplexDual.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpSimplexPrimal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpSimplexPrimalQuadratic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\Presolve.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\PresolveDoubleton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\PresolveDual.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\PresolveDupcol.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\PresolveEmpty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\PresolveFixed.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\PresolveForcing.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\PresolveImpliedFree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\PresolveIsolated.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\PresolveMatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\PresolvePsdebug.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\PresolveSingleton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\PresolveSubst.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\PresolveTighten.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\PresolveUseless.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\PresolveZeros.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Clp\include\ClpDualRowDantzig.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpDualRowPivot.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpDualRowSteepest.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpFactorization.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpLinearObjective.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpMatrixBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpModel.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpNetworkBasis.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpNetworkMatrix.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpNonLinearCost.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpObjective.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpPackedMatrix.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpParameters.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpPlusMinusOneMatrix.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpPrimalColumnDantzig.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpPrimalColumnPivot.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpPrimalColumnSteepest.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpSimplex.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpSimplexDual.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpSimplexPrimal.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpSimplexPrimalQuadratic.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinDistance.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinError.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinFactorization.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinFinite.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinFloatEqual.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinHelperFunctions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinIndexedVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinMessageHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinMpsIO.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPackedMatrix.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPackedVectorBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPragma.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinShallowPackedVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinSort.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinWarmStart.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinWarmStartBasis.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\Presolve.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\PresolveDoubleton.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\PresolveDual.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\PresolveDupcol.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\PresolveEmpty.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\PresolveFixed.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\PresolveForcing.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\PresolveImpliedFree.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\PresolveIsolated.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\PresolveMatrix.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\PresolvePsdebug.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\PresolveSingleton.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\PresolveSubst.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\PresolveTighten.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\PresolveUseless.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\PresolveZeros.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Clp\ClpLinearObjective.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpNetworkBasis.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpNetworkMatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpObjective.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\ClpPlusMinusOneMatrix.cpp
# End Source File
# End Target
# End Project
