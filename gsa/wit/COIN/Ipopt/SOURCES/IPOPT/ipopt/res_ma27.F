C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine RES_MA27(N, NNZ, A, IRN, JCN, RHS, LRW, RW, LIW, IW,
     1     IERR)
C
C*******************************************************************************
C
C     $Id: res_ma27.F,v 1.1 2002/05/02 18:52:20 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Interface to MA27
C
C
C-------------------------------------------------------------------------------
C                             Author, date
C-------------------------------------------------------------------------------
C
CA    Andreas Waechter      05/01/02  Release as version IPOPT 2.0
C
C
C-------------------------------------------------------------------------------
C                             Parameter list    
C-------------------------------------------------------------------------------
C
C    Name     I/O   Type   Meaning
CP
C
C*******************************************************************************
C
C                              Declarations
C
C*******************************************************************************
C
      IMPLICIT NONE
C
C*******************************************************************************
C
C                              Include files
C
C*******************************************************************************
C
      include 'IPOPT.INC'
C
C-------------------------------------------------------------------------------
C                             Parameter list
C-------------------------------------------------------------------------------
C
      integer N
      integer NNZ
      double precision A(NNZ)
      integer IRN(NNZ)
      integer JCN(NNZ)
      double precision RHS(N)
      integer LRW
      double precision RW(LRW)
      integer LIW
      integer IW(LIW)
      integer IERR
C
C-------------------------------------------------------------------------------
C                            Local variables
C-------------------------------------------------------------------------------
C
      double precision U
      integer LP, MP, LDIAG
      common /MA27DD/ U, LP, MP, LDIAG

      double precision OPS
      integer IERROR, NRLTOT, NIRTOT, NRLNEC, NIRNEC, NRLADU, NIRADU
      integer NRLBDU, NIRBDU, NCMPA, NCMPBR, NCMPBI, NTWO, NEIG
      common /MA27ED/ OPS, IERROR, NRLTOT, NIRTOT, NRLNEC, NIRNEC,
     1     NRLADU, NIRADU, NRLBDU, NIRBDU, NCMPA, NCMPBR, NCMPBI,
     2     NTWO, NEIG
C
      integer p_ikeep, p_iw1, p_iwend, p_rwend, iflag, liwma, la
      integer p_iwma, p_a, p_w, maxfrt, nsteps, p_irn, p_jcn, p_rhs
      integer ir, jc, i, p_rhs2
      double precision DNRM2, rhsnorm, residnorm
      character*80 line
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
#ifdef NO_MA27
      IERR = 97
      call C_OUT(2,0,1,
     1 'res_ma27:  Compiled without MA27 - cannot execute this option.')
      goto 9999
#else
      IERR = 0
      p_iwend = 0
      p_rwend = 0
C
C     Set default values for MA27
C
      LP = QCNR
      MP = QCNR
      U  = 1d-5
c      U  = 1d-3
      
      p_irn   = p_iwend
      p_jcn   = p_irn + NNZ
      p_ikeep = p_jcn + NNZ
      p_iw1   = p_ikeep + 3*N
      p_iwend = p_iw1 + 2*N
      if( p_iwend.gt.LIW ) then
         IERR = 99
         goto 9999
      endif

      do i = 1, NNZ
         IW(p_irn+i) = IRN(i)
         IW(P_jcn+i) = JCN(i)
      enddo

      iflag = 0
      call MA27AD(N, NNZ, IRN, JCN, IW(p_iwend+1), LIW-p_iwend,
     1     IW(p_ikeep+1), IW(p_iw1+1), nsteps, iflag)
      if( iflag.ne.0 ) then
         write(line,*)
     1        'res_ma27: MA27AD returns iflag=',iflag
         call C_OUT(2,0,1,line)
         IERR = 701
         goto 9999
      endif
      p_iwend = p_iw1

      liwma = int(200d0*dble(NIRNEC))
      la    = int(200d0*dble(NRLNEC))

      p_iwma  = p_iwend
      p_iw1   = p_iwma + liwma
      p_iwend = p_iw1  + N
      if( p_iwend.gt.LIW ) then
         IERR = 99
         goto 9999
      endif
      p_a     = p_rwend
      p_rwend = p_a + la
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
      call DCOPY(NNZ, A, 1, RW(p_a+1), 1)

      call MA27BD(N, NNZ, IRN, JCN, RW(p_a+1), la, IW(p_iwma+1), liwma,
     1     IW(p_ikeep+1), nsteps, maxfrt, IW(p_iw1+1), iflag)
c      if( iflag.eq.-5 .or. iflag.eq.3 .or. iflag.eq.-3 .or.
c     1     iflag.eq.-4 ) then
      if( iflag.eq.-5 .or. iflag.eq.3 ) then
         WRITE(line,*) 'iflag in MA27 = ',iflag
	   call C_OUT(2,0,1,line)
         IERR = -5
         goto 9999
      elseif( iflag.ne.0 ) then
         write(line,*)
     1        'res_ma27: MA27BD returns iflag=',iflag
         call C_OUT(2,0,1,line)
         IERR = 701
         goto 9999
      endif
      p_iwend = p_iw1

      p_rhs   = p_rwend
      p_w     = p_rhs + N
      p_rwend = p_w + maxfrt
      p_iw1   = p_iwend
      p_iwend = p_iw1 + nsteps
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      elseif( p_iwend.gt.LIW ) then
         IERR = 99
         goto 9999
      endif
      call DCOPY(N, RHS, 1, RW(p_rhs+1), 1)
C
      call MA27CD(N, RW(p_a+1), la, IW(p_iwma+1), liwma, RW(p_w+1),
     1     maxfrt, RHS, IW(p_iw1+1), nsteps)
      p_rwend = p_w
      p_iwend = p_iw1
C
C     Check if solution is really good enough
C
      p_rhs2  = p_rwend
      p_rwend = p_rhs2 + N
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
      call DCOPY(N, 0.d0, 0, RW(p_rhs2+1), 1)
      do i = 1, NNZ
         ir = IW(p_irn+i)
         jc = IW(p_jcn+i)
         RW(p_rhs2+ir) = RW(p_rhs2+ir) + A(i)*RHS(jc)
         if( ir.ne.jc ) then
            RW(p_rhs2+jc) = RW(p_rhs2+jc) + A(i)*RHS(ir)
         endif
      enddo
      call DAXPY(N, -1.d0, RW(p_rhs+1), 1, RW(p_rhs2+1), 1)
      rhsnorm   = DNRM2(N, RW(p_rhs +1), 1)
      residnorm = DNRM2(N, RW(p_rhs2+1), 1)
      if( residnorm .gt. 1d-8*rhsnorm ) then
         write(line,*) 'res_ma27: rhsnorm = ',rhsnorm
         call C_OUT(2,0,1,line)
         write(line,*) '          residnorm = ',residnorm
         call C_OUT(2,0,1,line)
         IERR = -5
      endif
#endif
 9999 continue
      return
      end
