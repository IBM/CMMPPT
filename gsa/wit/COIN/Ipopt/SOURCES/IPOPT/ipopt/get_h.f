C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine GET_H(ITER, N, NIND, NFIX, X, IVAR, NORIG, XORIG,
     1     CSCALE, M, NNZH, LAM, HESS, IRNH, ICNH,
     2     LRW, RW, LIW, IW, IERR)
C
C*******************************************************************************
C
C    $Id: get_h.f,v 1.1 2002/05/02 18:52:17 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Evaluate Hessian of Lagrangian at X
C
C-------------------------------------------------------------------------------
C                          Programm description
C-------------------------------------------------------------------------------
C
CB    
C
C-------------------------------------------------------------------------------
C                             Author, date
C-------------------------------------------------------------------------------
C
CA    Andreas Waechter      05/01/02  Release as version IPOPT 2.0
C
C-------------------------------------------------------------------------------
C                             Documentation
C-------------------------------------------------------------------------------
C
CD
C
C-------------------------------------------------------------------------------
C                             Parameter list    
C-------------------------------------------------------------------------------
C
C    Name     I/O   Type   Meaning
C
CP   ITER      I    INT    iteration counter
CP                         (for ITER=-1: only obtain number of NNZH in original
CP                                       Hessian including fixed vars)
CP                                       everything except NORIG, M, and NNZH
CP                                       is dummy)
CP   N         I    INT    number of free variables
CP   NIND      I    INT    number of independent variables
CP   NFIX      I    INT    number of fixed variables
CP   X         I    DP     values of free variables, where F should be
CP                            evaluated
CP   IVAR      I    INT    information about partitioning
CP                            i = 1..M      XORIG(IVAR(i)) dependent
CP                            i = (M+1)..N  XORIG(IVAR(i)) independent
CP                            Note: fixed variables do not occur in IVAR
CP                              X(i) corresponds to XORIG(IVAR(i))
CP   NORIG     I    INT    number of all variables including fixed vars
CP   XORIG    I/O   DP     I: values of fixed variables
CP                         O: values of all variables (from X)
CP   CSCALE    I    DP     scaling factors for constraints
CP   M         I    INT    number of constraints
CP   NNZH      O    INT    number of NZ in HESS (for ITER=-1 includes fixed)
CP   LAM       I    DP     values of Lagrangian multipliers
CP   HESS      O    DP     values of Hessian (sparse)
CP   IRNH      O    IND    row indices of Hessian (upper triangular part)
CP   ICNH      O    IND    column indices of Hessian (upper triangular part)
CP   LRW      I/O   INT    length of RW
CP   RW       I/O   DP     can be used as DP work space but content will be
CP                            changed between calls
CP   LIW      I/O   INT    length of IW
CP   IW       I/O   INT    can be used as INT work space but content will be
CP                            changed between calls
CP   IERR      O    INT    =0: everything OK
CP                         >0: Error occured; abort optimization
CP                         <0: Warning; message to user
C
C-------------------------------------------------------------------------------
C                             local variables
C-------------------------------------------------------------------------------
C
CL
C
C-------------------------------------------------------------------------------
C                             used subroutines
C-------------------------------------------------------------------------------
C
CS    EVAL_H
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
      integer ITER
      integer N
      integer NIND
      integer NFIX
      double precision X(N)
      integer IVAR(N)
      integer NORIG
      double precision XORIG(NORIG)
      double precision CSCALE(*)
      integer M
      integer NNZH
      double precision LAM(M)
      double precision HESS(NNZH)
      integer IRNH(NNZH)
      integer ICNH(NNZH)
      integer LRW
      double precision RW(LRW)
      integer LIW
      integer IW(LIW)
      integer IERR
C
C-------------------------------------------------------------------------------
C                            Local varibales
C-------------------------------------------------------------------------------
C
      integer i, idummy, p_rwend, p_iwend, p_ivar1, p_irnhtmp, p_icnhtmp
      integer p_hesstmp, kr, kc, j
      double precision dummy

      integer NNZH_ORIG
      save    NNZH_ORIG
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
C
C     For Iter = -1 determine number of NZ in Hessian 
C
      if( ITER.eq.-1 ) then
C
C     Fir obtain NNZH_ORIG in order to know how much work space we need
C
         call EVAL_H(0, NORIG, XORIG, M, dummy, NNZH_ORIG, dummy,
     1        idummy, idummy)
         NNZH = NNZH_ORIG
C
C     Otherwise, obtain Hessian and reorder accoring to IVAR (at the same
C     time, entries for fixed variables are eliminated)
C
      else
         IERR = 0
         p_iwend = 0
         p_rwend = 0
C
         do i = 1, N
            XORIG(IVAR(i)) = X(i)
         enddo
C
C     Compute inverse IVAR1 of IVAR
C
         p_ivar1 = p_iwend
         p_iwend = p_ivar1 + NORIG
         if( p_iwend.gt.LIW ) then
            IERR = 99
            goto 9999
         endif
         if( NFIX.gt.0 ) then
            do i = 1, NORIG
               IW(p_ivar1+i) = 0
            enddo
         endif
         do i = 1, N
            j = IVAR(i)
            IW(p_ivar1+j) = i
         enddo
C
C     Take care of scaling
C
         if( QFSCALE.ne.1d0 ) then
            if( QFSCALE.eq.0d0 ) then
               call C_OUT(2,0,1,'get_h: QFSCALE is zero. Abort.')
               IERR = 4
               goto 9999
            endif
            call DSCAL(M, 1d0/QFSCALE, LAM, 1)
         endif
         if( QSCALE.eq.2 ) then
            do i = 1, M
               LAM(i) = LAM(i)*CSCALE(i)
            enddo
         elseif( M.gt.0 .and. CSCALE(1).ne.1d0 ) then
            call DSCAL(M, CSCALE(1), LAM, 1)
         endif
C
         if( NFIX.eq.0 ) then
C
C     Get Hessian
C
            call EVAL_H( 1, NORIG, XORIG, M, LAM, NNZH_ORIG, HESS,
     1           IRNH, ICNH)
C
C     Reorder Hessian according to IVAR1
C
            do i = 1, NNZH
               IRNH(i) = IW(p_ivar1+IRNH(i))
            enddo
            do i = 1, NNZH
               ICNH(i) = IW(p_ivar1+ICNH(i))
            enddo
C
         else
C
C     Get Hessian (including entries for fixed variables)
C
            p_irnhtmp = p_iwend
            p_icnhtmp = p_irnhtmp + NNZH_ORIG
            p_iwend   = p_icnhtmp + NNZH_ORIG
            p_hesstmp = p_rwend
            p_rwend   = p_hesstmp + NNZH_ORIG
            if( p_rwend.gt.LRW ) then
               IERR = 98
               goto 9999
            elseif( p_iwend.gt.LIW ) then
               IERR = 99
               goto 9999
            endif
            call EVAL_H( 1, NORIG, XORIG, M, LAM, NNZH_ORIG,
     1           RW(p_hesstmp+1), IW(p_irnhtmp+1), IW(p_icnhtmp+1))
C
C     Reorder Hessian accoring to IVAR1 (and get rid of fixed vars)
C
            NNZH = 0
            do i = 1, NNZH_ORIG
               kr = IW(p_ivar1+IW(p_irnhtmp+i))
               kc = IW(p_ivar1+IW(p_icnhtmp+i))
               if( kr.ne.0 .and. kc.ne.0 ) then
                  NNZH = NNZH + 1
                  HESS(NNZH) = RW(p_hesstmp+i)
                  IRNH(NNZH) = kr
                  ICNH(NNZH) = kc
               endif
            enddo
C
            p_iwend = p_irnhtmp
            p_rwend = p_hesstmp
C
         endif
C
         p_iwend = p_ivar1
C
C     Unscale multipliers and scale Hessian
C
         if( QFSCALE.ne.1d0 ) then
            call DSCAL(M, QFSCALE, LAM, 1)
            call DSCAL(NNZH, QFSCALE, HESS, 1)
         endif
         if( QSCALE.eq.2 ) then
            do i = 1, M
               LAM(i) = LAM(i)/CSCALE(i)
            enddo
         elseif( M.gt.0 .and. CSCALE(1).ne.1d0 ) then
            call DSCAL(M, 1d0/CSCALE(1), LAM, 1)
         endif
C
      endif
C
C     I think that's it...
C
 9999 continue
      return
      end
      
