Copyright (C) 2002, Carnegie Mellon University and others.
All Rights Reserved.
This code is published under the Common Public License.


This file (QUICKINSTALL.AMPL) describes, how to build IPOPT as a
solver for AMPL.  More detailed information regarding installation can
be found in the INSTALL file.

1. Get the source code for IPOPT from the COIN-OR webpage

   http://www-124.ibm.com/developerworks/opensource/coin/Tarballs/

   as tarball (Ipopt*.tgz - whatever the latest one is) and copy it
   into a directory where you want to build the executable.

2. Uncompress and unpack the tarball:

   gunzip Ipopt*.tgz
   tar xvf Ipopt*.tar

3. Prepare the building directories:

   cd COIN/Ipopt/
   mkdir OBJECTS
   cd OBJECTS
   ln -s ../SOURCES/Makefiles/Makefile.install Makefile
   make install

4. After this, subdirectories in the current directory will have been
   created, and you will see a file called "Makefile.defs".  This is a
   file that is included into all makefiles in this package, and you
   need to change this if you want to divert from the stardard
   definitions (see Sections 2.3 and 2.4 in the INSTALL file for more
   details).

5. Get the required third party source code:

   - BLAS and LAPACK (see Section 2.4.1 in INSTALL file)
   - Harwell Subroutines Library (see Section 2.4.2 in the INSTALL
     file; you will need at least one of the routines MA27, MA47,
     MA57)  The unmodified Makefile.defs assumes that you use MA27 and
     also the scaling routine MC19 which are both available in the
     HSL Archive.
     NOTE: If the routines MC30, MC35, and MC39 are available to you,
           we recommend to include them into your build as they increase
           robustness and efficiency of the method.
   - TRON (see Section 2.4.3 in the INSTALL file)
   - AMPL Solver Library (see Section 2.4.4 in the INSTALL file)

6. Change into the building directory for the AMPL solver executable
   and compile the solver:

   [first change into the OBJECTS directory created in Step 3 above,
    i.e. the one in which your version of Makefile.defs is located] then:

   cd IPOPT/AMPL_interface
   make

7. You now have the solver executable (called 'ipopt').  You may copy
   this into any convenient location in your PATH.

8. Enjoy, and if there are problems (or if it is working great and you
   are happy), let us know.

$Id: QUICKINSTALL.AMPL,v 1.3 2002/05/10 20:16:52 andreasw Exp $
