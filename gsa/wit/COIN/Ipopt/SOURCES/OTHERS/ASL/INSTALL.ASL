Installation of the Ampl Solver Library (ASL) for IPOPT

Note: It is YOUR RESPONSIBILITY to ensure that you are entitled to
      download and use this third party package.

1. Obtain the ASL source code, for example from
   http://www.netlib.org/ampl/solvers/ .
   Check http://www.netlib.org/ampl/solvers/README for instructions on
   how to obtain the entire directory 'solvers' in a convenient manner.

   For example:

   In this directory enter:

   ftp netlib.bell-labs.com

   and log in as 'anonymous' and enter your email address as password

   In FTP, enter

   ftp> cd netlib/ampl
   ftp> get solvers.tar
   ftp> quit

2. Unpack this package, so that the directory solvers is now located
   in $SRCROOTDIR/OTHERS/ASL , i.e. you have a directory
   $SRCROOTDIR/OTHERS/ASL/solvers containing the source code for the
   ASL.

   For example:

   If you obtained the file 'solvers.tar' as described above and it is
   in this directory, enter

   tar xvf solvers.tar

   Since the files are compressed when obtained as described above,
   you need to uncompress them, e.g. by typing

   gunzip -fr solvers
