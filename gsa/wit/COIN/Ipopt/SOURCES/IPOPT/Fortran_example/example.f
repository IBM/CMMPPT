C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C
C    $Id: example.f,v 1.1 2002/05/02 18:52:15 andreasw Exp $
C
C =============================================================================
C
C     This is an example for the usage of IPOPT.
C     It implements problem 71 from the Hock-Schittkowsky test suite:
C
C     min   x1*x4*(x1 + x2 + x3)  +  x3
C     s.t.  x1*x2*x3*x4  -  x5             -  25  =  0
C           x1**2 + x2**2 + x3**2 + x4**2  -  40  =  0
C           1 <=  x1,x2,x3,x4  <= 5
C           0 <=  x5
C
C     Starting point: 
C        x = (1, 5, 5, 1, -24)
C
C     Optimal solution:
C        x = (1.00000000, 4.74299963, 3.82114998, 1.37940829, 0)
C           
C =============================================================================
C
C
C =============================================================================
C
C                            Main driver program
C
C =============================================================================
C
      program example
C
      implicit none
C
C     Define work space:
C
      integer     LRW,         LIW
      parameter  (LRW = 10000, LIW = 10000)
      integer          IW(LIW)
      double precision RW(LRW)
C
C     Size of the problem (no. of variables and equality constraints)
C
      integer     N,     M
      parameter  (N = 5, M = 2)
      double precision LAM(M)
C
C     Vector of variables
C
      double precision X(N)
C
C     Number of lower and upper bounds
C
      integer     NLB,     NUB
      parameter  (NLB = 5, NUB = 4)
C
C     Vector of lower and upper bounds
C
      integer             ILB(NLB),    IUB(NUB)
      double precision BNDS_L(NLB), BNDS_U(NUB), V_L(NLB), V_U(NUB)
C
C     Algorithmic Parameters (INITPARAMS)
C
      integer NARGS
      double precision ARGS(50)
      character*20 CARGS(50)
C
      integer i, IERR, ITER
C
C     Set initial point and bounds:
C
      data X      / 1d0, 5d0, 5d0, 1d0, -24d0 /
      data ILB    /   1,   2,   3,   4,     5 /
      data BNDS_L / 1d0, 1d0, 1d0, 1d0,   0d0 /
      data IUB    /   1,   2,   3,   4 /
      data BNDS_U / 5d0, 5d0, 5d0, 5d0 /
C
C     Set algorithmic parameters:
C
      NARGS = 1
      ARGS(1) = 1.d-10
      CARGS(1) = 'dtol'
      call INITPARAMS(IERR, NARGS, ARGS, CARGS)
      if( IERR.ne.0 ) then
         write(*,*) 'Error initializing algorithmic parameters.'
         write(*,*) 'IERR = ',IERR
         write(*,*)
         write(*,*) 
     1        'You might want to check IPOPT.DAT (if you set QCNR>0)'
         write(*,*)
         stop
      endif
C
C     Call optimization routine:
C
      call IPOPT(N, X, M, NLB, ILB, BNDS_L, NUB, IUB, BNDS_U, V_L, V_U,
     1     LAM, LRW, RW, LIW, IW, ITER, IERR)
C
C     Output:
C
      if( IERR.eq.0 ) then
         write(*,*)
         write(*,*) 'The solution was found after ',ITER,' Iterations.'
         write(*,*)
         write(*,*) 'The optimal values of X are:'
         write(*,*)
         do i = 1, N
            write(*,*) 'X  (',i,') = ',X(i)
         enddo
         write(*,*)
         write(*,*) 'The multipliers for the lower bounds are:'
         write(*,*)
         do i = 1, NLB
            write(*,*) 'V_L(',ILB(i),') = ',V_L(i)
         enddo
         write(*,*)
         write(*,*) 'The multipliers for the upper bounds are:'
         write(*,*)
         do i = 1, NUB
            write(*,*) 'V_U(',IUB(i),') = ',V_U(i)
         enddo
         write(*,*)
         write(*,*) 'The multipliers for the equality constraints are:'
         write(*,*)
         do i = 1, M
            write(*,*) 'LAM(',i,') = ',LAM(i)
         enddo
         write(*,*)
      else
         write(*,*)
         write(*,*) 'An error occoured after ',ITER,' Iterations.'
         write(*,*) 'The error code is ',IERR
         write(*,*)
      endif
C
      end
C
C =============================================================================
C
C                    Computation of objective function
C
C =============================================================================
C
      subroutine EVAL_F(N, X, F)
      implicit none
      integer N
      double precision F, X(N)
      F = X(1)*X(4)*(X(1)+X(2)+X(3)) + X(3)
      return
      end
C
C =============================================================================
C
C                Computation of gradient of objective function
C
C =============================================================================
C
      subroutine EVAL_G(N, X, G)
      implicit none
      integer N
      double precision G(N), X(N)
      G(1) = X(4)*(2d0*X(1)+X(2)+X(3))
      G(2) = X(1)*X(4)
      G(3) = X(1)*X(4) + 1d0
      G(4) = X(1)*(X(1)+X(2)+X(3))
      G(5) = 0d0
      return
      end
C
C =============================================================================
C
C                     Computation of equality constraints
C
C =============================================================================
C
      subroutine EVAL_C(N, X, M, C)
      implicit none
      integer N, M
      double precision C(M), X(N)
      C(1) = X(1)*X(2)*X(3)*X(4) - X(5) - 25d0
      C(2) = X(1)**2 + X(2)**2 + X(3)**2 + X(4)**2 - 40d0
      return
      end
C
C =============================================================================
C
C                Computation of Jacobian of equality constraints
C
C =============================================================================
C
      subroutine EVAL_A(TASK, N, X, NZ, A, ACON, AVAR)
      integer TASK, N, NZ
      double precision X(N), A(NZ)
      integer ACON(NZ), AVAR(NZ), I
C
C     structure of Jacobian:
C
      integer AVAR1(9), ACON1(9)
      data  AVAR1 /1, 2, 3, 4, 5, 1, 2, 3, 4/
      data  ACON1 /1, 1, 1, 1, 1, 2, 2, 2, 2/
      save  AVAR1, ACON1
C
      if( TASK.eq.0 ) then
         NZ = 9
      else
        Do I = 1, 9
          AVAR(I) = AVAR1(I)
          ACON(I) = ACON1(I)
        EndDo
        A(1) = X(2)*X(3)*X(4)
        A(2) = X(1)*X(3)*X(4)
        A(3) = X(1)*X(2)*X(4)
        A(4) = X(1)*X(2)*X(3)
        A(5) = -1d0
        A(6) = 2d0*X(1)
        A(7) = 2d0*X(2)
        A(8) = 2d0*X(3)
        A(9) = 2d0*X(4)
      endif
      return
      end
C
C =============================================================================
C
C                Computation of Hessian of Lagrangian
C
C =============================================================================
C
      subroutine EVAL_H(TASK, N, X, M, LAM, NNZH, HESS, IRNH, ICNH)
      implicit none
      integer TASK, N, M, NNZH, i
      double precision X(N), LAM(M), HESS(NNZH)
      integer IRNH(NNZH), ICNH(NNZH)
C
C     structure of Hessian:
C
      integer IRNH1(10), ICNH1(10)
      data  IRNH1 /1, 2, 2, 3, 3, 3, 4, 4, 4, 4/
      data  ICNH1 /1, 1, 2, 1, 2, 3, 1, 2, 3, 4/
      save  IRNH1, ICNH1

      if( TASK.eq.0 ) then
         NNZH = 10
      else
         do i = 1, 10
            IRNH(i) = IRNH1(i)
            ICNH(i) = ICNH1(i)
            HESS(i) = 0d0
         enddo
C
C     objective function
C
         HESS(1) = 2d0*X(4)
         HESS(2) = X(4)
         HESS(4) = X(4)
         HESS(7) = 2d0*X(1) + X(2) + X(3)
         HESS(8) = X(1)
         HESS(9) = X(1)
C
C     first constraint
C
         HESS(2) = HESS(2) + LAM(1) * X(3)*X(4)
         HESS(4) = HESS(4) + LAM(1) * X(2)*X(4)
         HESS(5) = HESS(5) + LAM(1) * X(1)*X(4)
         HESS(7) = HESS(7) + LAM(1) * X(2)*X(3)
         HESS(8) = HESS(8) + LAM(1) * X(1)*X(3)
         HESS(9) = HESS(9) + LAM(1) * X(1)*X(2)
C
C     second constraint
C
         HESS(1) = HESS(1) + LAM(2) * 2d0
         HESS(3) = HESS(3) + LAM(2) * 2d0
         HESS(6) = HESS(6) + LAM(2) * 2d0
         HESS(10)= HESS(10)+ LAM(2) * 2d0
      endif
      return
      end
