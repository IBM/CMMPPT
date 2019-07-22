/*
  Copyright (C) 2002, Carnegie Mellon University and others.
  All Rights Reserved.
  This code is published under the Common Public License.
*/
/*****************************************************************************
 $Id: ipoptAMPL.c,v 1.7 2003/01/27 22:17:24 andreasw Exp $
     AMPL interface ipopt
*****************************************************************************/
/****************************************************************************
Authors: 05/01/02  Arvind Raghunatan, Andreas Waechter
                   Release as Version IPOPT 2.0.1

 We thank Marcelo Marazzi and Sven Leyffer for providing example routines.
****************************************************************************/

#include "asl.h"
#include "getstub.h"
#define asl cur_ASL

/* On AIX there are no underscores appended to names for Fortran subroutines */
#ifdef _AIX
#define check_flagfile_ check_flagfile
#define initparams_ initparams
#define timer_ timer
#define ipopt_ ipopt
#define idamax_ idamax
#define ipopt_getdata_ ipopt_getdata
#define dcopy_ dcopy
#define dscal_ dscal

#define eval_a_ eval_a
#define eval_c_ eval_c
#define eval_hesscon_v_ eval_hesscon_v
#define eval_g_ eval_g
#define eval_f_ eval_f
#define eval_hesslag_v_ eval_hesslag_v
#define eval_hessobj_v_ eval_hessobj_v
#define eval_h_ eval_h
#endif

/* Problem specific variables required in subroutine */
fint numEq, numIneq ;
#ifdef INCLUDE_CC
fint numCcon, numVineq ;
#endif

/* Store the Lower and Upper bounds for constraints and variables */
real *conL, *conU ;

/* Variables used in inequality handling */
fint *ineqnos ;
/* Store the Lower and Upper bounds for constraints and variables */
real *conL, *conU ;

/* Variables used in inequality handling */
fint *ineqnos ;
real *slacks ;

/* Variables used in Complementarity handling */ 
#ifdef INCLUDE_CC
fint *cvar1nos, *cvar2nos ; /* holds indices of variables bearing 
			       complementarity relation */
fint *vineqvar ; /* hold the variable number in the complementarity relation
		    that is unbounded - handle for variational inequalities. 
		    Otherwise, holds zero. */

real *cvar1LBnds , *cvar1UBnds , *cvar2LBnds , *cvar2UBnds ; /* Store the 
								bounds */
real *cslacks ; /* slacks introduced for each of non-negative variable that is 
                   introduced for the unbounded complementarity variable */
real *rslacks ; /* slacks introduced for each of the complementarity constraint
                   that IPOPT sees. */
#endif

/* Variables used thruout the file */
static fint nerror = 0 ;
static fint nobj = 0 ;
static fint nnzH = 0 ; /* number of nonzeros in Hessian */

enum evalhcall {EVINIT, EVH, EVLAG, EVOBJ, EVCON };
static int lasthcall = EVINIT;

/* Constants pertaining MAXREAL etc. */

static real maxREAL = 1e+20 ; /* largest real number recognised in the RHS */
static real minREAL = -1e+20 ; /* smallest real number recognised in the RHS */
static real myInf = 1e+30 ; /* representation of infinity used in bounds 
			       representation - innocuous*/
static real myNegInf = -1e+30 ; /* --------------"------------ */ 

/* For number of gradient and jacobian evaluations */
fint nfgrad = 0 ;
fint ncjac = 0 ;

/* IPOPT default options */

#define FEWOPTIONS  /* Define this macro, if only the most important
		       IPOPT options should be available */

static fint
  opICNR = 41,
  opIPRINT = 0,
  opIOUTPUT = 0,
  opIMAXITER = 1000,
  opIMUINIT = 1,
#ifdef NO_MA27
  opISYMSOLV = 0,
#else
  opISYMSOLV = 1,
#endif
  opISCALE = 0,
  opISCALERR = 1,
  opIMERIT = 4,
  opISOC = 1,
#ifdef NO_MC35
  opITRONHESS = 0,
#else
#ifdef NO_MC39
  opITRONHESS = 0,
#else
  opITRONHESS = 1,
#endif
#endif
#ifdef LENGTH_IW
  opIMAX_IW = LENGTH_IW,
#else
  opIMAX_IW = 1000000,
#endif
#ifdef LENGTH_RW
  opIMAX_RW = LENGTH_RW,
#else
  opIMAX_RW = 1000000,
#endif
#ifndef FEWOPTIONS
  opIFULL = 1,
  opICG = 0,
  opIQUASI = 0,
  opIINITB = 0,
  opILMLEN = 6,
  opICORRECT = 3,
  opIDAMP = 0,
  opIRESTO = 2,
  opIALPHA = 2,
  opIITERBLOCKMAX = 4,
  opISELBAS = 0,
  opIHESSVECT = 0,
  opITRON2DERIVS = 1,
#ifdef NO_MC30
#ifdef NO_MC19
  opIKKTSCALE = 0,
#else
  opIKKTSCALE = 1,
#endif
#else
  opIKKTSCALE = 1,
#endif
#endif
#ifdef INCLUDE_CC
  opICCAGGREGATE = 0,
#endif
  opICNRM = 1;

static real
  opDTOL = 1e-8,
  opDFSCALE = 1.0,
  opDNUMIN = 1e-6,
  opDWATCHTOL = 0,
  opDLS_SAFE = 1e20,
  opDMU0 = 0.1,
  opDMULIN = 0.2,
  opDBNDFRAC = 1e-2,
  opDBNDPUSH = 1e-3,
  opDPIVTOL = 1e-8,
  opDPIVTOLMAX = 1e-8,
  opDTRONCGTOL = 1e-3,
#ifndef FEWOPTIONS
  opDSR1TOL = 1.0,
  opDSKIPFACT = 1000.0,
  opDRHO = 1e-1,
  opDMAXCOND = 5e2,
  opDCGTOL = 1e-3,
  opDRESTOKKTRED = 0.999,
  opDS_F = 2.3,
  opDS_THETA = 1.1,
  opDGAMMA_F = 1e-5,
  opDGAMMA_THETA = 1e-5,
  opDTAU = 0.99,
  opDVCORRECTFACT = 1e4,
  opDMUSUPER = 1.5,
  opDERRSUPER = 1.1,
  opDMUERRFAC = 10.0,
  opDMAXERR = 1e3,
#endif
#ifdef INCLUDE_CC
  opDCFACTOR = 1,
#endif
  opDDEPCONDIAG = 1e-8;

static keyword keywds[] = 
{
  KW("dbndfrac"     ,D_val,&opDBNDFRAC,
     "Minimal distance of starting point from closest bound as fraction\n             of bound difference"),
  KW("dbndpush"     ,D_val,&opDBNDPUSH,
     "Absolute minimal distance of starting points from closest bound"),
#ifdef INCLUDE_CC
  KW("dcfactor"     ,D_val,&opDCFACTOR,
     "Coefficient for barrier parameter in the relaxation of complementarity constraints"),
#endif
#ifndef FEWOPTIONS
  KW("dcgtol"       ,D_val,&opDCGTOL,
     "Tolerance for conjugate gradient option"),
#endif
  KW("ddepcondiag"  ,D_val,&opDDEPCONDIAG,
     "Value for diagonal element heuristic when constraints are\n             linearly dependent"), 
#ifndef FEWOPTIONS
  KW("derrsuper"    ,D_val,&opDERRSUPER,
     "Exponent for superlinear decrease of barrier error tolerance"),
#endif
  KW("dfscale"      ,D_val,&opDFSCALE,
     "Scaling factor for objective function"),
#ifndef FEWOPTIONS
  KW("dgamma_f"     ,D_val,&opDGAMMA_F,
     "Factor in filter margin (obj. func.)"),
  KW("dgamma_theta" ,D_val,&opDGAMMA_THETA,
     "Factor in filter margin (constraints)"),
#endif
  KW("dls_safe"     ,D_val,&opDLS_SAFE,
     "Error tolerance under which merit function penalty parameter is\n             only increased"),
#ifndef FEWOPTIONS
  KW("dmaxcond"     ,D_val,&opDMAXCOND,
     "When to repartition"),
  KW("dmaxerr"      ,D_val,&opDMAXERR,
     "Upper bound for barrier error tolerance"),
#endif
  KW("dmu0"         ,D_val,&opDMU0,
     "Initial value of barrier parameter"),
#ifndef FEWOPTIONS
  KW("dmuerrfac"    ,D_val,&opDMUERRFAC,
     "Factor between MU and barrier error tolerance"),
#endif
  KW("dmulin"       ,D_val,&opDMULIN,
     "Factor for linear decrease of MU"),
#ifndef FEWOPTIONS
  KW("dmusuper"     ,D_val,&opDMUSUPER,
     "Exponent for superlinear decrease of MU"),
#endif
  KW("dnumin"       ,D_val,&opDNUMIN,
     "Minimal (=Starting) value of merit function penalty parameter"),
  KW("dpivtol"      ,D_val,&opDPIVTOL,
     "Initial pivot tolerance in linear equation solver"),
  KW("dpivtolmax"   ,D_val,&opDPIVTOLMAX,
     "Maximal pivot tolerance in linear equation solver"),
#ifndef FEWOPTIONS
  KW("drestokktred" ,D_val,&opDRESTOKKTRED,
     "factor for sufficient reduction in KKT system based reestoration phase"),
  KW("drho"         ,D_val,&opDRHO,
     "Parameter in update rule for penalty parameter"),
  KW("ds_f"         ,D_val,&opDS_F,
     "Exponent in filter switching rule (obj. func.)"),
  KW("ds_theta"     ,D_val,&opDS_THETA,
     "Exponent in filter switching rule (constraints)"),
  KW("dskipfact"    ,D_val,&opDSKIPFACT,
     "Parameter for BFGS update skip"),
  KW("dsr1tol"      ,D_val,&opDSR1TOL,
     "Parameter for switching from BFGS to SR1"),
  KW("dtau"         ,D_val,&opDTAU,
     "Factor in fraction to the boundary rule"),
#endif
  KW("dtol"         ,D_val,&opDTOL,
     "Desired error tolerance (termination criterion)"),
#ifndef FEWOPTIONS
  KW("dtroncgtol"   ,D_val,&opDTRONCGTOL,
     "Tolerance for CG with TRON"),
  KW("dvcorrectfact",D_val,&opDVCORRECTFACT,
     "Factor for maximum deviation of V from S^{-1}MU"),
#endif
  KW("dwatchtol"    ,D_val,&opDWATCHTOL,
     "When to activate watchdog technique"),
#ifndef FEWOPTIONS
  KW("ialpha"       ,L_val,&opIALPHA,
     "Treatment of primal and dual fraction to the boundary rule"),
#endif
#ifdef INCLUDE_CC
  KW("iccaggregate" ,L_val,&opICCAGGREGATE,
     "Option of treating complementarity constraints inidividually/as aggregated"),
#endif
#ifndef FEWOPTIONS
  KW("icg"          ,L_val,&opICG,
     "CG option and preconditioner choice"),
#endif
  KW("icnr"         ,L_val,&opICNR,
     "To be set to zero in order to avoid output to file IPOPT.OUT"),
  KW("icnrm"        ,L_val,&opICNRM,
     "Type of norm to measure constraint violation during line search"),
#ifndef FEWOPTIONS
  KW("icorrect"     ,L_val,&opICORRECT,
     "Correction strategy for hessian if indefinite"),
  KW("idamp"        ,L_val,&opIDAMP,
     "Option of damping the cross term"),
  KW("ifull"        ,L_val,&opIFULL,
     "Full or reduced space option"),
  KW("ihessvect"    ,L_val,&opIHESSVECT,
     "Choice of computing hessian-vector products"),
  KW("iinitb"       ,L_val,&opIINITB,
     "Initialization of reduced hessian"),
  KW("iiterblockmax",L_val,&opIITERBLOCKMAX,
     "When to delete filter"),
  KW("ikktscale"    ,L_val,&opIKKTSCALE,
     "How to scale the KKT matrix before factorization"),
  KW("ilmlen"       ,L_val,&opILMLEN,
     "number of (s,y) pairs for limited memory BFGS"),
#endif
  KW("imax_iw"      ,L_val,&opIMAX_IW,
     "Size of integer workspace allotted to IPOPT"),
  KW("imax_rw"      ,L_val,&opIMAX_RW,
     "Size of double presision workspace allotted to IPOPT"),
  KW("imaxiter"     ,L_val,&opIMAXITER,
     "Maximum number of iterations"), 
  KW("imerit"       ,L_val,&opIMERIT,
     "Type of line search"),
  KW("imuinit"      ,L_val,&opIMUINIT,
     "Determines how barrier parameter is initialized"),
  KW("ioutput"       ,L_val,&opIOUTPUT,
     "If set to 1, more output on screen per iteration"),
  KW("iprint"       ,L_val,&opIPRINT,
     "Log level for output to file IPOPT.OUT"),
#ifndef FEWOPTIONS
  KW("iquasi"       ,L_val,&opIQUASI,
     "Quasi-Newton approx or Exact information for Hessian"),
  KW("iresto"       ,L_val,&opIRESTO,
     "Choice of filter restoration phase"),
#endif
  KW("iscale"       ,L_val,&opISCALE,
     "Set to 1 if automatic scaling of functions is to be done"),
  KW("iscalerr"       ,L_val,&opISCALERR,
     "Determines whether error estimate is scaled"),
#ifndef FEWOPTIONS
  KW("iselbas"      ,L_val,&opISELBAS,
     "How to select basis partition"),
#endif
  KW("isoc"         ,L_val,&opISOC,
     "Second order correction"),
  KW("isymsolv"     ,L_val,&opISYMSOLV,
     "Solver for symetric system (0: MA47; 1: MA27; 2: MA57)"),
#ifndef FEWOPTIONS
  KW("itron2derivs" ,L_val,&opITRON2DERIVS,
     "Choice of Gauss-Newton approximation in TRON"),
#endif
  KW("itronhess"    ,L_val,&opITRONHESS,
     "Determines how Hessian-vectors products are computed in TRON")
} ;

static Option_Info Oinfo = {"ipopt","IPOPT-2.0.1","ipopt_options",keywds,nkeywds} ;

/* Parameter input routine */
Cextern initparams_(fint *IERR, fint *NARGS, real *ARGS, char *CARGS,
		    int lCARGS);

/* IPOPT subroutine */
Cextern ipopt_(fint *N, real *X, fint *M, fint *NLB, fint *ILB, real *BNDS_L, 
               fint *NUB, fint *IUB, real *BNDS_U, real *V_L, real *V_U,
	       real *LAM, fint *LRW, real *RW, fint *LIW, fint *IW, fint *ITER,
	       fint *IERR) ;

/* Routine for obtaining current value of MU */
#ifdef INCLUDE_CC
Cextern get_amplmu_(real *MU);
#endif

/* BLAS routines */
Cextern dcopy_(fint *Nvar, real *X, fint *incX, real *Y, fint *incY) ;

Cextern daxpy_(fint *Nvar, real *alpha, real *X, fint *incX, real *Y,
	       fint *incY) ;

Cextern dscal_(fint *Nvar, real *alpha, real *X,fint *incX) ;

/* Standard error message if memory allocation not successful */
#define MALLOCERR(Varname) {fprintf(Stderr,"Cannot allocate memory for " #Varname ".\n"); exit(-1);}

/* function for copying C-string into Fortran character array */
void str2fstr(char *Cstr, char *Fstr, int Flen)
{
  int i, len;

  len = strlen(Cstr);
  if( len > Flen) len = Flen;
  memcpy(Fstr, Cstr, len);
  for ( i = len; i < Flen; Fstr[i++] = ' ' );
}


/* Initialising the slack variables introduced for inequalities */
void initslacks(real *X, real *slacks, fint *ineqnos)
{
  fint i ;

  xknown(X); /* make sure, that ASL doesn't check for each call of conival
		if poitn X has changed... */
  for ( i = 0 ; i < numIneq ; i++ )
    {
      slacks[i] = conival(ineqnos[i]-1,X,&nerror) ;
      /* Error Handling */
      if ( nerror < 0 )
	{
	  fprintf(Stderr,"Error detected in AMPL evaluation!\n") ;
	  fprintf(Stderr," nerror = %d\n",nerror) ;
	  fprintf(Stderr,"Exiting from subroutine - initslacks\n") ;
	  exit(1) ;
	}
    }
  xunknown();
}

/* Initialising the variables introduced in variational type constraints to 
   convert them into complementarity constratints.                             
   Implemented a very arbitrary initialisation. Need to look into it later */
#ifdef INCLUDE_CC
void initcompslacks(real *X, real *cslacks, real *slacks, fint * ineqnos) 
{
  fint i ;

  for ( i = 0 ; i < (2*numVineq) ; i++ )
    cslacks[i] = 1 ;
}
#endif

/* Initialising the slack variable introduced in each of the relaxed 
   complementarity constraint */
#ifdef INCLUDE_CC
void initrslacks(real *X, real *rslacks)
{
  fint i ;

  if ( opICCAGGREGATE == 0 )
    for ( i = 0 ; i < (numCcon + 2*numVineq) ; i++ )
      rslacks[i] = 1e-1 ;
  else
    *rslacks = 1e-1 ;
}
#endif

/* Objective function evaluation - objfun */   
void eval_f_(fint *N, real *X, real *F)
{ 

  /* Check to deal with the no objective AMPL problem. */
  if ( n_obj == 0 )
    {
      *F = 0 ;
    }
  else
    {
      *F = objval(nobj, X, &nerror) ;
      /* Check if this is a maximization problem */
      if (objtype[nobj])
	*F = -*F ;
    }
  
  /* Error handling */
  if ( nerror < 0 )
    {
      fprintf(Stderr,"Error detected in AMPL evaluation!\n") ;
      fprintf(Stderr," nerror = %d\n",nerror) ;
      fprintf(Stderr,"Exiting from subroutine - eval_f_\n") ;
      exit(1) ;
    }
  else if ( nerror > 0 )
    {
      *F = exp(1000000.);
      return;
    }
  
}

/* Objective gradient evaluation - objgrd */
void eval_g_(fint *N, real *X, real *G)
{
  fint n_varF , totsize ;
  fint incX = 0 , incY = 1 ; 
  real ZERO = 0 , max2min = -1 ;

  /* Check to deal with the no objective AMPL problem. */
  if ( n_obj == 0 )
    {
      if ( n_cc == 0 )
	totsize = n_var + numIneq ;
#ifdef INCLUDE_CC
      else if ( opICCAGGREGATE == 0 )
	totsize = n_var + numIneq + 2*numVineq + numCcon + 2*numVineq ;
      else if ( opICCAGGREGATE == 1 )
	totsize = n_var + numIneq + 2*numVineq + 1 ;
#endif
      dcopy_(&totsize,&ZERO,&incX,&G[0],&incY) ;
    }
  else
    {
      nfgrad++ ;
      objgrd(nobj, X, G, &nerror) ;

      /* Check for maximization problems */
      if (objtype[nobj])
	{
	  n_varF = n_var ;
	  dscal_(&n_varF,&max2min,&G[0],&incY) ;
	}

      /* Error handling */
      if ( nerror < 0 )
	{
	  fprintf(Stderr,"Error detected in AMPL evaluation!\n") ;
  fprintf(Stderr," nerror = %d\n",nerror) ;
	  fprintf(Stderr,"Exiting from subroutine - eval_g_\n") ;
	  exit(1) ;
	}
      else if( nerror > 0 )
	{
	  G[0] = exp(1000000.);
	  return;
	}
      if ( numIneq > 0 )
	dcopy_(&numIneq,&ZERO,&incX,&G[n_var],&incY) ;  
#ifdef INCLUDE_CC
      if ( numVineq > 0 )
	{
	  totsize = 2*numVineq ;
	  dcopy_(&totsize,&ZERO,&incX,&G[n_var+numIneq],&incY) ;
	}
      if ( n_cc > 0 )
	if ( opICCAGGREGATE == 0 )
	  {
	    totsize = numCcon + 2*numVineq ;
	    dcopy_(&totsize,&ZERO,&incX,&G[n_var+numIneq+2*numVineq],&incY) ;
	  }
	else
	  G[n_var + numIneq + 2*numVineq] = 0 ;
#endif
    }
  
}

/* Constraint value evaluation - conval */
void eval_c_(fint *N, real *X, fint *M, real *C)
{
  fint i,j,k ;
  fint presize ;
  real mu, sum ;
 
  conval(X, C, &nerror) ;

  /* Error handling */
  if ( nerror < 0 )
    {
      fprintf(Stderr,"Error detected in AMPL evaluation!\n") ;
      fprintf(Stderr," nerror = %d\n",nerror) ;
      fprintf(Stderr,"Exiting from subroutine - eval_c_\n") ;
      exit(1) ;
    }
  else if( nerror > 0 )
    {
      C[0] = exp(1000000.);
      return;
    }

  /* Evaluate the constraints given by AMPL */

  j = 0 ; /* inequality nos index */

  for ( i = 0 ; i < n_con ; i++ )
    {
      if ( ( numIneq > 0 ) && ( i == (ineqnos[j]-1) ) )
	  {
	    C[i] = C[i] - X[n_var+j] ;
	    j++ ;
	  }
      else
	{
	  C[i] = C[i] - conL[i] ; 
	}
    }

  /* Evaluate the complementarity constraints and append them appropriately */
#ifdef INCLUDE_CC  
  j = 0 ; /* constraint index */
  k = 0 ; /* relaxed complementarity slack index */
  presize = n_var + numIneq + 2*numVineq ; /* number of variables without 
					      taking into account the number 
					      of relaxed compl. slacks */
  sum = 0 ; /* sum of the complementarity constraints for the aggregated case 
	     */

  get_amplmu_(&mu);

  if ( n_cc > 0 )
    {
      for ( i = 0 ; i < (numCcon + 2*numVineq) ; i++ )
	{
	  if ( vineqvar[i] > 0 )
	    {
	      C[n_con+j] = X[vineqvar[i]-1] - (X[cvar1nos[i]-1] - X[cvar1nos[i+1]-1]) ;
	      j++ ;
	    }
	  
	  /* Look for different cases and write the complementarity relation */

	  /* l1 <= s <= Infinity complements l2 <= x <= Infinity */
	  if ( (cvar1UBnds[i] > maxREAL) && (cvar2UBnds[i] > maxREAL) )
	    {
	      if ( opICCAGGREGATE == 0 )
		{
		  C[n_con+j] = (X[cvar1nos[i]-1] - cvar1LBnds[i])*(X[cvar2nos[i]-1] - cvar2LBnds[i]) + X[presize+k] - opDCFACTOR*mu ;
		  j++ ;
		  k++ ;
		}
	      else
		sum = sum + (X[cvar1nos[i]-1] - cvar1LBnds[i])*(X[cvar2nos[i]-1] - cvar2LBnds[i]) ;
	    }
	  /* l1 <= s <= Infinity complements -Infinity <= x <= u2 */
	  else if ( (cvar1UBnds[i] > maxREAL) && (cvar2LBnds[i] < minREAL) )
	    {
	      if ( opICCAGGREGATE == 0 )
		{
		  C[n_con+j] = (X[cvar1nos[i]-1] - cvar1LBnds[i])*(cvar2UBnds[i] - X[cvar2nos[i]-1]) + X[presize+k] - opDCFACTOR*mu ;
		  j++ ;
		  k++ ;
		}
	      else
		sum = sum + (X[cvar1nos[i]-1] - cvar1LBnds[i])*(cvar2UBnds[i] - X[cvar2nos[i]-1]) ;
	    }
	  /* -Infinity <= s <= u1 complements l1 <= x <= Infinity */
	  else if ( (cvar1LBnds[i] < minREAL) && (cvar2UBnds[i] > maxREAL) )
	    {
	      if ( opICCAGGREGATE == 0 )
		{
		  C[n_con+j] = (cvar1UBnds[i] - X[cvar1nos[i]-1])*(X[cvar2nos[i]-1] - cvar2LBnds[i]) + X[presize+k] - opDCFACTOR*mu ;
		  j++ ;
		  k++ ;
		}
	      else
		sum = sum + (cvar1UBnds[i] - X[cvar1nos[i]-1])*(X[cvar2nos[i]-1] - cvar2LBnds[i]) ;
	    }
	  /* -Infinity <= s <= u1 complements -Infinity <= x <= u2 */
	  else if ( (cvar1LBnds[i] < minREAL) && (cvar2LBnds[i] < minREAL) )
	    {
	      if ( opICCAGGREGATE == 0 )
		{
		  C[n_con+j] = (cvar1UBnds[i] - X[cvar1nos[i]-1])*(cvar2UBnds[i] - X[cvar2nos[i]-1]) + X[presize+k] - opDCFACTOR*mu ;
		  j++ ;
		  k++ ;
		}
	      else
		sum = sum + (cvar1UBnds[i] - X[cvar1nos[i]-1])*(cvar2UBnds[i] - X[cvar2nos[i]-1]) ;
	    }
	}
      if ( opICCAGGREGATE == 1)
	C[n_con+j] = sum + X[presize+k] - opDCFACTOR*mu ;

    }
#endif       

  /* Need to decide if this is really neceesary: */
  lasthcall = EVINIT;
  
}

/* Constraint gradients evaluation - jacval */
void eval_a_(fint *TASK, fint *N, real *X, fint *NZ, real *A, fint *ACON, 
             fint *AVAR)
{
  real *internalA ;
  cgrad *cg ;
  fint i , j = 0 , k , l , m ;
  fint presize ;

  if ( *TASK == 0 )
    {
      if ( n_cc == 0 )
	*NZ = nzc + numIneq ;
#ifdef INCLUDE_CC
      else if ( opICCAGGREGATE == 0 )
	*NZ = nzc + numIneq + 2*numCcon + 7*numVineq + numCcon + 2*numVineq ;
      else if ( opICCAGGREGATE == 1 )
	*NZ = nzc + numIneq + 2*numCcon + 7*numVineq + 1 ;
#endif
    }
  else if ( *TASK == 1 )
    {
      if( !(internalA = (real *)Malloc(nzc*sizeof(real))) ) MALLOCERR(internalA) ;
  
      jacval(X, internalA, &nerror) ;

      /* Error handling */
      if ( nerror < 0 )
	{
	  fprintf(Stderr,"Error detected in AMPL evaluation!\n") ;
	  fprintf(Stderr," nerror = %d\n",nerror) ;
	  fprintf(Stderr,"Exiting from subroutine - eval_a_\n") ;
	  exit(1) ;
	}
      else if( nerror > 0 )
	{
	  internalA[0] = exp(1000000.);
	  return;
	} 

      ncjac++ ;
      for ( i = 0 ; i < n_con ; i++ )
	for ( cg = Cgrad[i] ; cg ; cg = cg->next )
	  {
	    A[j] = internalA[cg->goff] ;
	    ACON[j] = i + 1 ;
	    AVAR[j] = cg->varno + 1 ;
	    j++ ;
	  }
      /* Jacobian of inequalities */
      for ( i = 0 ; i < numIneq ; i++ )
	{
	  A[j] = -1 ;
	  ACON[j] = ineqnos[i] ;
	  AVAR[j] = n_var+i+1 ;
	  j++ ;
	}

      /* Jacobian of complementarity constraints */
#ifdef INCLUDE_CC
      k = 0 ; /* constraints index */
      l = 0 ; /* relaxed complementarity slack index */
      presize = n_var + numIneq + 2*numVineq ; /* size before adding relaxed
						  compl. slacks */
      
      for ( i = 0 ; i < (numCcon + 2*numVineq) ; i++ )
	{
	  if ( vineqvar[i] > 0 )
	    {
	      A[j] = 1 ;
	      ACON[j] = n_con + k + 1 ;
	      AVAR[j] = vineqvar[i] ;
	      j++ ;

	      A[j] = -1 ;
	      ACON[j] = n_con + k + 1 ;
	      AVAR[j] = cvar1nos[i] ;
	      j++ ;

	      A[j] = 1 ;
	      ACON[j] = n_con + k + 1 ;
	      AVAR[j] = cvar1nos[i+1] ;
	      j++ ;
	      
	      k++ ;
	    }

	  /* Look for different cases and write the complementarity relation */

	  if ( opICCAGGREGATE == 0 )
	    m = n_con + k + 1 ;
	  else
	    m = n_con + numVineq + 1 ;

	  /* l1 <= s <= Infinity complements l2 <= x <= Infinity */
	  if ( (cvar1UBnds[i] > maxREAL) && (cvar2UBnds[i] > maxREAL) )
	    {
	      A[j] = X[cvar2nos[i]-1] - cvar2LBnds[i] ;
	      ACON[j] = m ;
	      AVAR[j] = cvar1nos[i] ;
  	      j++ ;

	      A[j] = X[cvar1nos[i]-1] - cvar1LBnds[i] ;
	      ACON[j] = m ;
	      AVAR[j] = cvar2nos[i] ;
	      j++ ;

	      if ( opICCAGGREGATE == 0 )
		{
		  A[j] = 1 ;
		  ACON[j] = m ;
		  AVAR[j] = presize + l + 1 ;
		  j++ ;
		  
		  k++ ;
		  l++ ;
		}
	    }
	  /* l1 <= s <= Infinity complements -Infinity <= x <= u2 */
	  else if ( (cvar1UBnds[i] > maxREAL) && (cvar2LBnds[i] < minREAL) )
	    {
	      A[j] = cvar2UBnds[i] - X[cvar2nos[i]-1] ;
	      ACON[j] = m ;
	      AVAR[j] = cvar1nos[i] ;
	      j++ ;

	      A[j] = -(X[cvar1nos[i]-1] - cvar1LBnds[i]) ;
	      ACON[j] = m ;
	      AVAR[j] = cvar2nos[i] ;
	      j++ ;

	      if ( opICCAGGREGATE == 0 )
		{
		  A[j] = 1 ;
		  ACON[j] = m ;
		  AVAR[j] = presize + l + 1 ;
		  j++ ;

		  k++ ;
		  l++ ;
		}
	    }
	  /* -Infinity <= s <= u1 complements l1 <= x <= Infinity */
	  else if ( (cvar1LBnds[i] < minREAL) && (cvar2UBnds[i] > maxREAL) )
	    {
	      A[j] = -(X[cvar2nos[i]-1] - cvar2LBnds[i]) ;
	      ACON[j] = m ;
	      AVAR[j] = cvar1nos[i] ;
	      j++ ;

	      A[j] = cvar1UBnds[i] - X[cvar1nos[i]-1] ;
	      ACON[j] = m ;
	      AVAR[j] = cvar2nos[i] ;
	      j++ ;

	      if ( opICCAGGREGATE == 0 )
		{
		  A[j] = 1 ;
		  ACON[j] = m ;
		  AVAR[j] = presize + l + 1 ;
		  j++ ;

		  k++ ;
		  l++ ;
		}
	    }
	  /* -Infinity <= s <= u1 complements -Infinity <= x <= u2 */
	  else if ( (cvar1LBnds[i] < minREAL) && (cvar2LBnds[i] < minREAL) )
	    {
	      A[j] = -(cvar2UBnds[i] - X[cvar2nos[i]-1]) ;
	      ACON[j] = m ;
	      AVAR[j] = cvar1nos[i] ;	      
	      j++ ;

	      A[j] = -(cvar1UBnds[i] - X[cvar1nos[i]-1]) ;
	      ACON[j] = m ;
	      AVAR[j] = cvar2nos[i] ;	    
	      j++ ;

	      if ( opICCAGGREGATE == 0 )
		{
		  A[j] = 1 ;
		  ACON[j] = m ;
		  AVAR[j] = presize + l + 1 ;
		  j++ ;

		  k++ ;
		  l++ ;
		}
	    }

	}
      if ( opICCAGGREGATE == 1 )
	{
	  A[j] = 1 ;
	  ACON[j] = n_con + numVineq + 1 ;
	  AVAR[j] = presize + 1 ;
	}
#endif

      free(internalA) ;
    }

}

/* Hessian of Lagrangian evaluation */
void eval_h_(fint *TASK, fint *N, real *X, fint *M, real *LAM, fint *NNZH, 
             real *HESS, fint *IRNH, fint *ICNH)
{
  fint ow , y , i , j , k , uptri ;
  real OW , *H , temp , *cdummy ;

  if ( *TASK == 0 )
    {
      /* Check to deal with the no objective AMPL problem. */
      if ( n_obj == 0 )
	{
	  ow = 0 ; /* coefficient of objective function in the Lagrangian */
	  y = 1 ; /* mulitpliers will be supplied */
	  uptri = 1 ; /* need only the upper triangular part */
	  
	  nnzH = sphsetup(nobj,ow,y,uptri) ;
	}
      else
	{
	  ow = 1 ; /* coefficient of objective function in the Lagrangian */
	  y = 1 ; /* multipliers will be supplied */ 
	  uptri = 1 ; /* need only the upper triangular part */
	  
	  nnzH = sphsetup(-1,ow,y,uptri) ;
	}
      *NNZH = nnzH ;
      /* The Hessain does not return the complementarity derivatives. The 
	 one below is necessary as AMPL provides complementarity relation 
	 between a variable and a constraint which translates to a AMPL 
	 variable and an internal slack - so no duplication would occur. */
#ifdef INCLUDE_CC
      if (n_cc > 0)
	*NNZH = *NNZH + numCcon + 2*numVineq ;
#endif
    }
  else if ( *TASK == 1 ) 
    {
  
      if( !(H = (real *)Malloc(nnzH*sizeof(real))) ) MALLOCERR(H) ;

      if( 0 ) {
      /* Innocuous call to make sure we are evaluating Hessian at the given
         point */
      if (n_obj != 0)
	temp = objval(nobj,X,&nerror) ;
 
      /* Error handling */
      if ( nerror < 0 )
	{
	  fprintf(Stderr,"Error detected in AMPL evaluation!\n") ;
	  fprintf(Stderr," nerror = %d\n",nerror) ;
	  fprintf(Stderr,"Exiting from subroutine - eval_h_\n") ;
	  exit(1) ;
	}                                                                      
                                                               
      /* Calls to evaluate constraint values */

      if( !(cdummy = (real *)Malloc(nzc*sizeof(real))) ) MALLOCERR(cdummy) ;
      conval(X, cdummy, &nerror) ;
      /* Error handling */
      if ( nerror < 0 )
	{
	  fprintf(Stderr,"Error detected in AMPL evaluation!\n") ;
	  fprintf(Stderr," nerror = %d\n",nerror) ;
	  fprintf(Stderr,"Exiting from subroutine - eval_h_\n") ;
	  exit(1) ;
	}
      free(cdummy);
      }

      if (n_obj > 0)
	if (objtype[nobj])
	  OW = -1 ;
	else
	  OW = 1 ;
    
      if ( n_obj > 0 )
	{
	  sphes(H,-1,&OW,LAM) ;
	}
      else
	{
	  sphes(H,nobj,NULL,LAM) ;
	}
      lasthcall = EVH;

      k = 0 ;
      for ( i = 0 ; i < n_var ; i++ )
	for ( j = sputinfo->hcolstarts[i] ; j < sputinfo->hcolstarts[i+1] ; j++ )
	  {
	    HESS[k] = H[j] ;
	    ICNH[k] = i + 1 ;
	    IRNH[k] = sputinfo->hrownos[j] + 1 ;
	    k++ ;
	  }
      j = n_con ;

#ifdef INCLUDE_CC
      if ( n_cc > 0 )
	for ( i = 0 ; i < (numCcon + 2*numVineq) ; i++ )
	  {
	    if ( opICCAGGREGATE == 0 )
	      {
		HESS[k] = (vineqvar[i] > 0) ? LAM[++j] : LAM[j] ;
		j++ ;
	      }
	    else
	      HESS[k] = LAM[n_con+numVineq] ;
	    if (cvar1nos[i] > cvar2nos[i])
	      {
		ICNH[k] = cvar2nos[i] ;
		IRNH[k] = cvar1nos[i] ;
	      }
	    else
	      {
		ICNH[k] = cvar1nos[i] ;
		IRNH[k] = cvar2nos[i] ;
	      }
	    k++ ;
	  }
#endif

      free(H) ;
    }

}
  
/**************Computation of Hessian-Vector products****************/

/***************Hessian of Lagrangian times vector********************/

void eval_hesslag_v_(fint *TASK, fint *N, real *X, fint *M, real *LAM, real *VIN, real *VOUT)
{
  fint incX = 0 , incY = 1 ;
  real OW , temp, *cdummy ;
  real ZERO = 0 ; 
#ifdef INCLUDE_CC
  fint i, totsize ;
#endif

  /* Dummy call to ensure evaluation of Hessian at X */
  if (n_obj > 0)
    temp = objval(nobj, X, &nerror) ;
  
  /* Error handling */
  if ( nerror < 0 )
    {
      fprintf(Stderr,"Error detected in AMPL evaluation!\n") ;
      fprintf(Stderr," nerror = %d\n",nerror) ;
      fprintf(Stderr,"Exiting from subroutine - eval_hesslag_v_\n") ;
      exit(1) ;
    }

  /* Calls to evaluate constraint values */

  if( !(cdummy = (real *)Malloc(nzc*sizeof(real))) ) MALLOCERR(cdummy) ;
  conval(X, cdummy, &nerror) ;
  /* Error handling */
  if ( nerror < 0 )
    {
      fprintf(Stderr,"Error detected in AMPL evaluation!\n") ;
      fprintf(Stderr," nerror = %d\n",nerror) ;
      fprintf(Stderr,"Exiting from subroutine - eval_hesslag_v_\n") ;
      exit(1) ;
    }
  free(cdummy);

  if (n_obj > 0)
    if (objtype[nobj])
      OW = -1 ;
    else
      OW = 1 ;

  if( lasthcall == EVINIT || lasthcall != EVLAG )
    hvinit(-1,&OW,LAM) ;
  hvcomp(VOUT,VIN,-1,&OW,LAM) ;
  lasthcall = EVLAG;

  /* Copy zeros into the remaining part of the vector */
  if ( numIneq > 0 )
    dcopy_(&numIneq,&ZERO,&incX,&VOUT[n_var],&incY) ;

#ifdef INCLUDE_CC
  if ( n_cc > 0 )
    {
      if ( numVineq > 0 )
	{
	  totsize = 2*numVineq ;
	  dcopy_(&totsize,&ZERO,&incX,&VOUT[n_var+numIneq],&incY) ;
	}
      if ( opICCAGGREGATE == 0 )
	{
	  totsize = numCcon + 2*numVineq ;
	  dcopy_(&totsize,&ZERO,&incX,&VOUT[n_var+numIneq+2*numVineq],&incY) ;
	}
      else
	VOUT[n_var+numIneq+2*numVineq] = 0 ;

      for ( i = 0 ; i < (numCcon + 2*numVineq) ; i++ )
	{
	  if ( opICCAGGREGATE == 0 )
	    {	  
	      VOUT[cvar1nos[i]-1] += LAM[n_con+numVineq+i]*VIN[cvar2nos[i]-1] ;
	      VOUT[cvar2nos[i]-1] += LAM[n_con+numVineq+i]*VIN[cvar1nos[i]-1] ;
	    }
	  else
	    {
	      VOUT[cvar1nos[i]-1] += LAM[n_con+numVineq]*VIN[cvar2nos[i]-1] ;
	      VOUT[cvar2nos[i]-1] += LAM[n_con+numVineq]*VIN[cvar1nos[i]-1] ;
	    }
	}
    }
#endif
}

/********************Hessian of objective times vector**********************/

void eval_hessobj_v_(fint *TASK, fint *N, real *X, fint *M, real *VIN, real *VOUT)
{
  fint incX = 0, incY = 1 ;
  real OW , temp , *cdummy ;
  real ZERO = 0 ;
#ifdef INCLUDE_CC
  fint totsize ;
#endif

  /* Dummy call to ensure evaluation of Hessian at X */
  if (n_obj > 0)
    temp = objval(nobj, X, &nerror) ;
  
  /* Error handling */
  if ( nerror < 0 )
    {
      fprintf(Stderr,"Error detected in AMPL evaluation!\n") ;
      fprintf(Stderr," nerror = %d\n",nerror) ;
      fprintf(Stderr,"Exiting from subroutine - eval_hessobj_v_\n") ;
      exit(1) ;
    }

  /* Calls to evaluate constraint values */

  if( !(cdummy = (real *)Malloc(nzc*sizeof(real))) ) MALLOCERR(cdummy) ;
  conval(X, cdummy, &nerror) ;
  /* Error handling */
  if ( nerror < 0 )
    {
      fprintf(Stderr,"Error detected in AMPL evaluation!\n") ;
      fprintf(Stderr," nerror = %d\n",nerror) ;
      fprintf(Stderr,"Exiting from subroutine - eval_hesslag_v_\n") ;
      exit(1) ;
    }
  free(cdummy);

  if (n_obj > 0)
    if (objtype[nobj])
      OW = -1 ;
    else
      OW = 1 ;
  
  if( lasthcall == EVINIT || lasthcall != EVOBJ )
    hvinit(-1,&OW,NULL) ;
  hvcomp(VOUT,VIN,-1,&OW,NULL) ;
  lasthcall = EVOBJ;
  
  /* Copy zeros into the remaining part of the vector */
  if ( numIneq > 0 )
    dcopy_(&numIneq,&ZERO,&incX,&VOUT[n_var],&incY) ;
  
#ifdef INCLUDE_CC
  if ( n_cc > 0 )
    {
      if ( numVineq > 0 )
	{
	  totsize = 2*numVineq ;
	  dcopy_(&totsize,&ZERO,&incX,&VOUT[n_var+numIneq],&incY) ;
	}
      if ( opICCAGGREGATE == 0 )
	{
	  totsize = numCcon + 2*numVineq ;
	  dcopy_(&totsize,&ZERO,&incX,&VOUT[n_var+numIneq+2*numVineq],&incY) ;
	}
      else
	VOUT[n_var+numIneq+2*numVineq] = 0 ;
    }
#endif
}

/**************Hessain of the constraints times vector***************/

void eval_hesscon_v_(fint *TASK, fint *N, real *X, fint *M, real *LAM, real *VIN, real *VOUT)
{
  fint incX = 0 , incY = 1 ;
  real temp , *cdummy ;
  real ZERO = 0 ;
#ifdef INCLUDE_CC
  fint i, totsize ;
#endif

  /* Dummy call to ensure evaluation of Hessian at X */
  if (n_obj > 0)
    temp = objval(nobj, X, &nerror) ;
  
  /* Error handling */
  if ( nerror < 0 )
    {
      fprintf(Stderr,"Error detected in AMPL evaluation!\n") ;
      fprintf(Stderr," nerror = %d\n",nerror) ;
      fprintf(Stderr,"Exiting from subroutine - eval_hessobj_v_\n") ;
      exit(1) ;
    }

  /* Calls to evaluate constraint values */

  if( !(cdummy = (real *)Malloc(nzc*sizeof(real))) ) MALLOCERR(cdummy) ;
  conval(X, cdummy, &nerror) ;
  /* Error handling */
  if ( nerror < 0 )
    {
      fprintf(Stderr,"Error detected in AMPL evaluation!\n") ;
      fprintf(Stderr," nerror = %d\n",nerror) ;
      fprintf(Stderr,"Exiting from subroutine - eval_hesslag_v_\n") ;
      exit(1) ;
    }
  free(cdummy);

  if( lasthcall == EVINIT || lasthcall != EVCON )
    hvinit(-1,NULL,LAM) ;
  hvcomp(VOUT,VIN,-1,NULL,LAM) ;
  lasthcall = EVCON;

  /* Copy zeros into the remaining part of the vector */
  if ( numIneq > 0 )
    dcopy_(&numIneq,&ZERO,&incX,&VOUT[n_var],&incY) ;  

#ifdef INCLUDE_CC
  if ( n_cc > 0 )
    {
      if ( numVineq > 0 )
	{
	  totsize = 2*numVineq ;
	  dcopy_(&totsize,&ZERO,&incX,&VOUT[n_var+numIneq],&incY) ;
	}
      if ( opICCAGGREGATE == 0 )
	{
	  totsize = numCcon + 2*numVineq ;
	  dcopy_(&totsize,&ZERO,&incX,&VOUT[n_var+numIneq+2*numVineq],&incY) ;
	}
      else
	VOUT[n_var+numIneq+2*numVineq] = 0 ;

      for ( i = 0 ; i < (numCcon + 2*numVineq) ; i++ )
	{
	  if ( opICCAGGREGATE == 0 )
	    {	  
	      VOUT[cvar1nos[i]-1] += LAM[n_con+numVineq+i]*VIN[cvar2nos[i]-1] ;
	      VOUT[cvar2nos[i]-1] += LAM[n_con+numVineq+i]*VIN[cvar1nos[i]-1] ;
	    }
	  else
	    {
	      VOUT[cvar1nos[i]-1] += LAM[n_con+numVineq]*VIN[cvar2nos[i]-1] ;
	      VOUT[cvar2nos[i]-1] += LAM[n_con+numVineq]*VIN[cvar1nos[i]-1] ;
	    }
	}
    }
#endif
}

/************************evaluating constraints residual**********************/
/*** This is to be evaluated whenver mu is changed by IPOPT ro obtain the  ***/
/*** correct complementarity constraint residuls for original NLP and      ***/
/*** relaxed barrier problem.                                              ***/
#ifdef INCLUDE_CC
void eval_ccerr_(fint *N,fint *M, real *X,real *C, real *MU)
{
  fint i, j, k, presize ;
  real sum ;

  j = 0 ; /* constraint index */
  k = 0 ; /* relaxed complementarity slack index */
  presize = n_var + numIneq + 2*numVineq ; /* number of variables without 
					      taking into account the number 
					      of relaxed compl. slacks */
  sum = 0 ; /* sum of the complementarity constraints for the aggregated case 
	     */

  if ( n_cc > 0 )
    {
      for ( i = 0 ; i < (numCcon + 2*numVineq) ; i++ )
	{
	  if ( vineqvar[i] > 0 )
	    j++ ;
	  
	  /* Look for different cases and write the complementarity relation */

	  /* l1 <= s <= Infinity complements l2 <= x <= Infinity */
	  if ( (cvar1UBnds[i] > maxREAL) && (cvar2UBnds[i] > maxREAL) )
	    {
	      if ( opICCAGGREGATE == 0 )
		{
		  if (*MU > 0)
		    C[n_con+j] = (X[cvar1nos[i]-1] - cvar1LBnds[i])*(X[cvar2nos[i]-1] - cvar2LBnds[i]) + X[presize+k] - opDCFACTOR*(*MU) ;
		  else
		    C[n_con+j] = (X[cvar1nos[i]-1] - cvar1LBnds[i])*(X[cvar2nos[i]-1] - cvar2LBnds[i]) ;
		  j++ ;
		  k++ ;
		}
	      else
		sum = sum + (X[cvar1nos[i]-1] - cvar1LBnds[i])*(X[cvar2nos[i]-1] - cvar2LBnds[i]) ;
	    }
	  /* l1 <= s <= Infinity complements -Infinity <= x <= u2 */
	  else if ( (cvar1UBnds[i] > maxREAL) && (cvar2LBnds[i] < minREAL) )
	    {
	      if ( opICCAGGREGATE == 0 )
		{
		  if (*MU > 0)
		    C[n_con+j] = (X[cvar1nos[i]-1] - cvar1LBnds[i])*(cvar2UBnds[i] - X[cvar2nos[i]-1]) + X[presize+k] - opDCFACTOR*(*MU) ;
		  else
		    C[n_con+j] = (X[cvar1nos[i]-1] - cvar1LBnds[i])*(cvar2UBnds[i] - X[cvar2nos[i]-1]) ;  
		  j++ ;
		  k++ ;
		}
	      else
		sum = sum + (X[cvar1nos[i]-1] - cvar1LBnds[i])*(cvar2UBnds[i] - X[cvar2nos[i]-1]) ;
	    }
	  /* -Infinity <= s <= u1 complements l1 <= x <= Infinity */
	  else if ( (cvar1LBnds[i] < minREAL) && (cvar2UBnds[i] > maxREAL) )
	    {
	      if ( opICCAGGREGATE == 0 )
		{
		  if (*MU > 0)
		    C[n_con+j] = (cvar1UBnds[i] - X[cvar1nos[i]-1])*(X[cvar2nos[i]-1] - cvar2LBnds[i]) + X[presize+k] - opDCFACTOR*(*MU) ;
		  else
		    C[n_con+j] = (cvar1UBnds[i] - X[cvar1nos[i]-1])*(X[cvar2nos[i]-1] - cvar2LBnds[i]) ;
		  j++ ;
		  k++ ;
		}
	      else
		sum = sum + (cvar1UBnds[i] - X[cvar1nos[i]-1])*(X[cvar2nos[i]-1] - cvar2LBnds[i]) ;
	    }
	  /* -Infinity <= s <= u1 complements -Infinity <= x <= u2 */
	  else if ( (cvar1LBnds[i] < minREAL) && (cvar2LBnds[i] < minREAL) )
	    {
	      if ( opICCAGGREGATE == 0 )
		{
		  if (*MU > 0)
		    C[n_con+j] = (cvar1UBnds[i] - X[cvar1nos[i]-1])*(cvar2UBnds[i] - X[cvar2nos[i]-1]) + X[presize+k] - opDCFACTOR*(*MU) ;
		  else
		    C[n_con+j] = (cvar1UBnds[i] - X[cvar1nos[i]-1])*(cvar2UBnds[i] - X[cvar2nos[i]-1]) ;
		  j++ ;
		  k++ ;
		}
	      else
		sum = sum + (cvar1UBnds[i] - X[cvar1nos[i]-1])*(cvar2UBnds[i] - X[cvar2nos[i]-1]) ;
	    }
	}
      if (opICCAGGREGATE == 1)
	if (*MU > 0)
	  C[n_con+j] = sum + X[presize +k] - opDCFACTOR*(*MU) ;
	else
	  C[n_con+j] = sum ;
    }

}
#endif

/********************************** main subroutine **************************/
main(int argc, char **argv)
{

#ifdef sgi
#include <sys/fpu.h>
#endif 
    
  /* Variables used in the call to IPOPT. */
  fint N, M, NLB, *ILB, NUB, *IUB, LIW, LRW , *IW, ITER, IERR ;
  real *X, *BNDS_L, *BNDS_U, *V_L, *V_U, *LAM, *RW ;

  fint i , j , k , l , m , clen , vlen , bound, numLBnds ;
  real optObj, conValue, mult, zero ;

  char *stub ;
  char message[100];
  FILE *nl, *soln ;

  /* Array for passing options */
#ifdef FEWOPTIONS
  static fint noptions = 25 ;
#else
  static fint noptions = 55 ;
#endif
  real *optvalues ;
  char *optnames ;

#ifndef SKIP_IPOUT
  fint FFUNCS, CFUNCS, NOCG, NORES, NONEGCURV, ex, fe, nb, one ;
  real cnrm, *optC;
  /* For running using script */
  FILE *script ;
  /*clock_t clkStart , clkEnd ;*/
  real clkStart , clkEnd ;
#endif

#ifdef sgi
  union fpc_csr f;
  f.fc_word = get_fpc_csr();
  f.fc_struct.flush = 0;
  set_fpc_csr(f.fc_word);
#endif 

  if ( argc < 2 )
    {
      printf("usage: %s stub\n", argv[0]) ;
      exit(1) ;
    }

  ASL_alloc(ASL_read_pfgh) ;

  stub = getstub(&argv,&Oinfo) ;

  if (getopts(argv,&Oinfo))
    return 1 ;  
  
  /* Set problem constants and dimensions in .nl file */
  nl = jac0dim(stub, (fint)strlen(stub)) ;

  /* Allocate memory for initial guess. */
  if( !(X0 = (real *)Malloc(n_var*sizeof(real))) ) MALLOCERR(X0) ;

  /* Read the file */
  pfgh_read(nl,ASL_findgroups) ;
  hesset(1, nobj, 1, 0, nlc);

  /* Check if complementarity constraints are present but INCLUDE_CC is undefined */
#ifndef INCLUDE_CC
  if (n_cc > 0)
    {
      printf("Error : Cannot handle complementarity constraints.\n") ;
      exit(1) ;
    }
#endif

  /* Check for incorrect options setting */
  if ( opIMAX_IW <= 0 )
    {
      fprintf(Stderr,"Invalid value for imax_iw: %d\n",opIMAX_IW) ;
      exit(1) ;
    }
  if ( opIMAX_RW <= 0 )
    {
      fprintf(Stderr,"Invalid value for imax_rw: %d\n",opIMAX_RW) ;
      exit(1) ;
    }

  /* Identify the number of equalities, inequalities, number of lower and upper     bounds */

  numEq = 0 ;
  numIneq = 0 ;

  if( !(conL = (real *)Malloc(n_con*sizeof(real))) ) MALLOCERR(conL) ;
  if( !(conU = (real *)Malloc(n_con*sizeof(real))) ) MALLOCERR(conU) ;

  for ( i = 0 ; i < n_con ; i++ )
    {
      conL[i] = LUrhs[2*i] ;
      conU[i] = LUrhs[2*i+1] ;

      if ( conL[i] <= minREAL )
	conL[i] = minREAL ;
      if ( conU[i] >= maxREAL )
	conU[i] = maxREAL ;

      if ( conL[i] == conU[i] )
	numEq = numEq + 1 ;
      else
	numIneq = numIneq + 1 ;

    }		
  
  /* Obtain the number of given Complementarity constraints involved .
     Do not count complementarity relations involving equations */
#ifdef INCLUDE_CC
  numCcon = 0 ;
  numVineq = 0 ;

  if ( n_cc > 0 )
    for ( i = 0 ; i < n_con ; i++ )
      {
	if ( cvar[i] > 0 && cvar[i] <= n_var )
	  {
	    /* Check if any one of them has equal lower and upper bounds */
	    if ( (conL[i] != conU[i]) && (LUv[2*(cvar[i]-1)] != LUv[2*(cvar[i]-1)+1]) )
	      {
		/* Check if any of them is unconstrained */
		if ( (LUrhs[2*i] < minREAL) && (LUrhs[2*i+1] > maxREAL) )
		  numVineq = numVineq + 1 ;
		else if ( (LUv[2*(cvar[i]-1)] < minREAL) && (LUv[2*(cvar[i]-1)+1] > maxREAL) )
		  numVineq = numVineq + 1 ;
		else
		  numCcon = numCcon + 1 ;
	      }
	  }
      }
#endif

  /* Set the values of IPOPT call parameters. */
  if ( n_cc == 0 )
    {
      N = n_var + numIneq ;
      M = n_con ;
    }    
#ifdef INCLUDE_CC
  else if ( opICCAGGREGATE == 0 )
    {
      N = n_var + numIneq + 2*numVineq + numCcon + 2*numVineq ; 
      M = n_con + numCcon + 3*numVineq ;
    }
  else if ( opICCAGGREGATE == 1 )
    {
      N = n_var + numIneq + 2*numVineq + 1 ;
      M = n_con + numVineq + 1 ;
    }
#endif

  /* Identify the inequality constraint numbers */

  if ( numIneq > 0 )
    {
      if( !(slacks = (real *)Malloc(numIneq*sizeof(real))) ) MALLOCERR(slacks) ;
      if( !(ineqnos = (fint *)Malloc(numIneq*sizeof(fint))) ) MALLOCERR(ineqnos) ;
      j = 0 ;
      
      for ( i = 0 ; i < n_con ; i++ )
	{
	  if ( conL[i] != conU[i] )
	    {
	      ineqnos[j] = i+1 ;
	      j++ ;
	    }
	}

    }

  /* Calculate the number of upper and lower bounds of the variables given */
  NLB = 0 ;
  NUB = 0 ;

  for ( i = 0 ; i < n_var ; i++ )
    {
      if ( LUv[2*i] > minREAL )
	NLB = NLB + 1 ;
      if ( LUv[2*i+1] < maxREAL ) 
	NUB = NUB + 1 ;
    }

  for ( i = 0 ; i < numIneq ; i++ )
    {
      if ( LUrhs[2*(ineqnos[i]-1)] > minREAL )
	NLB = NLB + 1 ;
      if ( LUrhs[2*(ineqnos[i]-1)+1] < maxREAL )
	NUB = NUB + 1 ;
    }

  /* Compensate for the additional slacks added for variational inequalities */
#ifdef INCLUDE_CC
  if ( n_cc > 0 )
    {
      if ( opICCAGGREGATE == 0 )
	NLB = NLB + 2*numVineq + numCcon + 2*numVineq ;
      else if ( opICCAGGREGATE == 1 )
	NLB = NLB + 2*numVineq + 1 ;
    }
#endif

  /* Store the variable lower and upper bounds appropriately. */

  if( !(BNDS_L = (real *)Malloc(NLB*sizeof(real))) ) MALLOCERR(BNDS_L) ;
  if( !(ILB = (fint *)Malloc(NLB*sizeof(fint))) ) MALLOCERR(ILB) ;
  if( !(BNDS_U = (real *)Malloc(NUB*sizeof(real))) ) MALLOCERR(BNDS_U) ;
  if( !(IUB = (fint *)Malloc(NUB*sizeof(fint))) ) MALLOCERR(IUB) ;

  j = 0 ; /* index the ILB */
  l = 0 ; /* index the IUB */

  for ( i = 0 ; i < n_var ; i++ )
    {
      if ( LUv[2*i] > minREAL )
	{
	  ILB[j] = i+1 ;
	  BNDS_L[j] = LUv[2*i] ;
	  j++ ;
	}
      if ( LUv[2*i+1] < maxREAL )
	{
	  IUB[l] = i+1 ;
	  BNDS_U[l] = LUv[2*i+1] ;
	  l++ ;
	}
    }

  for ( i = 0 ; i < numIneq ; i++ )
    {
      if ( LUrhs[2*(ineqnos[i]-1)] > minREAL )
	{
	  ILB[j] = n_var+i+1 ;
	  BNDS_L[j] = LUrhs[2*(ineqnos[i]-1)] ;
	  j++ ;
	}
      if ( LUrhs[2*(ineqnos[i]-1)+1] < maxREAL )
	{
	  IUB[l] = n_var+i+1 ;
	  BNDS_U[l] = LUrhs[2*(ineqnos[i]-1)+1] ;
	  l++ ;
	}
    }
  numLBnds = j ;

  /* Store the bounds for the complementarity variables */
#ifdef INCLUDE_CC
  if ( n_cc > 0 )
    {

      if( !(vineqvar = (fint *)Malloc((numCcon + 2*numVineq)*sizeof(fint))) ) MALLOCERR(vineqvar) ;
      if( !(cvar1nos = (fint *)Malloc((numCcon + 2*numVineq)*sizeof(fint))) ) MALLOCERR(cvar1nos) ;
      if( !(cvar2nos = (fint *)Malloc((numCcon + 2*numVineq)*sizeof(fint))) ) MALLOCERR(cvar2nos) ;
      if( !(cvar1LBnds = (real *)Malloc((numCcon + 2*numVineq)*sizeof(real))) ) MALLOCERR(cvar1LBnds) ;
      if( !(cvar1UBnds = (real *)Malloc((numCcon + 2*numVineq)*sizeof(real))) ) MALLOCERR(cvar1UBnds) ;
      if( !(cvar2LBnds = (real *)Malloc((numCcon + 2*numVineq)*sizeof(real))) ) MALLOCERR(cvar2LBnds) ;
      if( !(cvar2UBnds = (real *)Malloc((numCcon + 2*numVineq)*sizeof(real))) ) MALLOCERR(cvar2UBnds) ;
      if( !(cslacks = (real *)Malloc(2*numVineq*sizeof(real))) ) MALLOCERR(cslacks) ;
      if( !(rslacks = (real *)Malloc((numCcon + 2*numVineq)*sizeof(real))) ) MALLOCERR(rslacks) ;

      j = 0 ; /* index the inequality slack variables */
      l = 0 ; /* index the complementarity slack variables */
      k = 0 ; /* index the array varnos and bounds */
      m = 0 ; /* index the bounds to be supplied to IPOPT */
      for ( i = 0 ; i < n_con ; i++ )
	{
	  if ( cvar[i] > 0 & cvar[i] <= n_var )
	    {
	      /* Check for equalities and avoid them */
	      if ( (conL[i] != conU[i]) && (LUv[2*(cvar[i]-1)] != LUv[2*(cvar[i]-1)+1]) )
		{
		  /* Check for unrestricted expression - variational inequality
		   */
		  if ( (LUrhs[2*i] < minREAL) && (LUrhs[2*i+1] > maxREAL) )
		    {
		      /* -Infinity < s < Infinity complements l <= x <= u 
			 Reformulate as,
			        s = s1 - s2
				s1 >= 0 complements x >= l    (C1)
				s2 >= 0 complements x <= u    (C2)        */ 
 
		      /* Complementarity relation - (C1) */
		      vineqvar[k] = n_var+j+1 ; /* Inequality slack is 
						   unbounded */
		      cvar1nos[k] = n_var+numIneq+l+1 ; /* complementarity 
							       slack 1 (+) */
		      cvar1LBnds[k] = 0 ;
		      cvar1UBnds[k] = myInf ;
		      cvar2nos[k] = cvar[i] ; /* complentary variable */
		      cvar2LBnds[k] = LUv[2*(cvar[i]-1)] ;
		      cvar2UBnds[k] = myInf ;
		      ILB[numLBnds+m] = cvar1nos[k] ;
		      BNDS_L[numLBnds+m] = 0 ;
		      l++ ;
		      k++ ;
		      m++ ;

		      /* Complementarity relation - (C2) */
		      vineqvar[k] = 0 ;
		      cvar1nos[k] = n_var+numIneq+l+1 ; /* complementarity
							       slack 2 (-) */
		      cvar1LBnds[k] = 0 ;
		      cvar1UBnds[k] = myInf ;
		      cvar2nos[k] = cvar[i] ; /* complementary variable */
		      cvar2LBnds[k] = myNegInf ;
		      cvar2UBnds[k] = LUv[2*(cvar[i]-1)+1] ;
		      ILB[numLBnds+m] = cvar1nos[k] ;
		      BNDS_L[numLBnds+m] = 0 ;
		      l++ ;
		      k++ ;
		      m++ ;
		      j++ ;

		    }
		  else if ( (LUv[2*(cvar[i]-1)] < minREAL) && (LUv[2*(cvar[i]-1)+1] > maxREAL) )
		    {
		      /* l <= s <= u complements -Infinity < x < Infinity 
			 Reformulate as,
			            x = s1 - s2 
				    s1 >= 0 complements s >= l  (C1)
				    s2 >= 0 complements s <= u  (C2)       */

		      /* Complementarity relation - (C1) */
		      vineqvar[k] = cvar[i] ; /* AMPL variable is unbounded */
		      cvar1nos[k] = n_var+numIneq+l+1 ; /* Complementarity
							   slack 1 (+) */
		      cvar1LBnds[k] = 0 ;
		      cvar1UBnds[k] = myInf ;
		      cvar2nos[k] = n_var+j+1 ; /* Inequality slack */
		      cvar2LBnds[k] = LUrhs[2*i] ;
		      cvar2UBnds[k] = myInf ;
		      ILB[numLBnds+m] = cvar1nos[k] ;
		      BNDS_L[numLBnds+m] = 0 ;
		      l++ ;
		      k++ ;
		      m++ ;

		      /* Complementarity relation - (C2) */
		      vineqvar[k] = 0 ;
		      cvar1nos[k] = n_var+numIneq+l+1 ; /* Complementarity
							   slack 2 (-) */
		      cvar1LBnds[k] = 0 ;
		      cvar1UBnds[k] = myInf ;
		      cvar2nos[k] = n_var+j+1 ; /* Inequality slack */
		      cvar2LBnds[k] = myNegInf ;
		      cvar2UBnds[k] = LUrhs[2*i+1] ;
		      ILB[numLBnds+m] = cvar1nos[k] ;
		      BNDS_L[numLBnds+m] = 0 ;
		      l++ ;
		      k++ ;
		      m++ ;
		      j++ ;
		    }
		  else 
		    {
		      /* It is one of,
			   l1 <= s complements l2 <= x
			   l1 <= s complements u2 >= x
			   u1 >= s complements l2 <= x
			   u1 >= s complements u2 >= x  */
		     
		      vineqvar[k] = 0 ; 
		      cvar1nos[k] = n_var+j+1 ; /* Inequality slack */
		      cvar1LBnds[k] = LUrhs[2*i] ;
		      cvar1UBnds[k] = LUrhs[2*i+1] ;
		      cvar2nos[k] = cvar[i] ;
		      cvar2LBnds[k] = LUv[2*(cvar[i]-1)] ;
		      cvar2UBnds[k] = LUv[2*(cvar[i]-1)+1] ;
		      k++ ;
		      j++ ;
		    }
		}
	    }
	  /* An inequality not involved in a complementarity relation */
	  else if ( ineqnos[j] == i+1 )
	    j++ ;
	}
    }
  numLBnds = numLBnds + m ;
#endif

  /* Bounds for the variables introduced as part of relaxing the 
     complementarity equation */
#ifdef INCLUDE_CC
  if ( n_cc > 0 )
    {
      j = (opICCAGGREGATE == 1) ? 1 : (numCcon + 2*numVineq) ;
      for ( i = 0 ; i < j ; i++ )
	{
	  ILB[numLBnds+i] = n_var + numIneq + 2*numVineq + i + 1 ;
	  BNDS_L[numLBnds+i] = 0 ;
	}
    }
#endif

  /* Initialise the slack variables */
  if ( numIneq > 0 )
    initslacks(X0,slacks,ineqnos) ;

  /* Initialise the slacks for variational inequalities */
#ifdef INCLUDE_CC
  if ( n_cc > 0 )
    initcompslacks(X0,cslacks,slacks,ineqnos) ;
#endif

  /* Initialise the slacks for each relaxed complementarity constraint */
#ifdef INCLUDE_CC
  if ( n_cc > 0 )
    initrslacks(X0,rslacks) ;
#endif

  /* Copy initial point into X */
  if( !(X = (real *)Malloc(N*sizeof(real))) ) MALLOCERR(X) ;
 
  for ( i = 0 ; i < N ; i++ )
    {
      if ( i < n_var )
	X[i] = X0[i] ;
      else if ( i < (n_var + numIneq) )
	X[i] = slacks[i-n_var] ;
#ifdef INCLUDE_CC
      else if ( i < (n_var + numIneq + 2*numVineq) )
	X[i] = cslacks[i-n_var-numIneq] ;      
      else
	X[i] = rslacks[i-n_var-numIneq-2*numVineq] ;
#endif
    }

  /* Allocate memory for the Multipliers */
  if( !(V_L = (real *)Malloc(NLB*sizeof(real))) ) MALLOCERR(V_L) ;
  if( !(V_U = (real *)Malloc(NUB*sizeof(real))) ) MALLOCERR(V_U) ;
  if( !(LAM = (real *)Malloc(M*sizeof(real))) ) MALLOCERR(LAM) ;

  /* Initialize the workspace variables */
  LRW = opIMAX_RW ;
  LIW = opIMAX_IW ;

  if( !(RW = (real *)Malloc(LRW*sizeof(real))) ) MALLOCERR(RW) ;
  if( !(IW = (fint *)Malloc(LIW*sizeof(fint))) ) MALLOCERR(IW) ;

  /* Read variable names */
  vlen = maxcolnamelen ; 
  clen = maxrownamelen ;

#ifndef SKIP_IPOUT
  /* Write ip.out as one-line output for iterlist */
  nb = (NUB>NLB) ? NUB : NLB ;
  check_flagfile_(&ex) ;
  if ( ex )
    {
      script = fopen("ip.out","w") ;
      fprintf(script,"%18s | %5d %5d %5d | %5d/%5d %5dc%5dr%5dn (%3d) %15.8e %10.3e %9.2e",stub,N,M,nb,0,-1,0,0,0,-1,0e0,0e0,-1e0 ) ;
      fclose(script) ;
    }
  else
    {
      script = fopen("ip.out","w") ;
      fprintf(script," | %5d/%5d %5dc%5dr%5dn (%3d) %15.8e %10.3e %9.2e",0,-1,0,0,0,-1,0e0,0e0,-1e0 )
;
      fclose(script) ;
    }
#endif

  /* Write SLACKS.DAT to tell IPOPT about the slack variables */
  if( numIneq > 0 )
    {
      script = fopen("SLACKS.DAT","w") ;
      for( i = n_var+1 ; i <= (n_var + numIneq) ; ++i)
        {
          fprintf(script,"%6d\n",i) ;
        }
      fclose(script) ;
    }
  else
    {
      remove("SLACKS.DAT") ;
    }


  /* Pass parameters to IPOPT */
#define CARGSLEN 40
  if( !(optnames = (char *)Malloc(noptions*CARGSLEN*sizeof(char))) ) MALLOCERR(optnames) ;
  if( !(optvalues = (real *)Malloc(noptions*sizeof(real))) ) MALLOCERR(optvalues) ;
  j = 0;
  i = 0;

#define SetARGS(PARAM) {optvalues[i++] = op ## PARAM; str2fstr(#PARAM,optnames+j,CARGSLEN);j+=CARGSLEN;}

  SetARGS(ICNR);
  SetARGS(IPRINT);
  SetARGS(IOUTPUT);
  SetARGS(IMAXITER);
  SetARGS(IMUINIT);
  SetARGS(ISYMSOLV);
  SetARGS(ISCALE);
  SetARGS(ISCALERR);
  SetARGS(IMERIT);
  SetARGS(ISOC);
  SetARGS(ITRONHESS);
  SetARGS(ICNRM);

  SetARGS(DTOL);
  SetARGS(DFSCALE);
  SetARGS(DNUMIN);
  SetARGS(DWATCHTOL);
  SetARGS(DLS_SAFE);
  SetARGS(DMU0);
  SetARGS(DMULIN);
  SetARGS(DBNDFRAC);
  SetARGS(DBNDPUSH);
  SetARGS(DPIVTOL);
  SetARGS(DPIVTOLMAX);
  SetARGS(DTRONCGTOL);
  SetARGS(DDEPCONDIAG);

#ifndef FEWOPTIONS
  SetARGS(IFULL);
  SetARGS(ICG);
  SetARGS(IQUASI);
  SetARGS(IINITB);
  SetARGS(ILMLEN);
  SetARGS(ICORRECT);
  SetARGS(IDAMP);
  SetARGS(IRESTO);
  SetARGS(IALPHA);
  SetARGS(IITERBLOCKMAX);
  SetARGS(ISELBAS);
  SetARGS(IHESSVECT);
  SetARGS(ITRON2DERIVS);
  SetARGS(IKKTSCALE);
  SetARGS(DSR1TOL);
  SetARGS(DSKIPFACT);
  SetARGS(DRHO);
  SetARGS(DMAXCOND);
  SetARGS(DCGTOL);
  SetARGS(DRESTOKKTRED);
  SetARGS(DS_F);
  SetARGS(DS_THETA);
  SetARGS(DGAMMA_F);
  SetARGS(DGAMMA_THETA);
  SetARGS(DTAU);
  SetARGS(DVCORRECTFACT);
  SetARGS(DMUSUPER);
  SetARGS(DERRSUPER);
  SetARGS(DMUERRFAC);
  SetARGS(DMAXERR);
#endif
#undef SetARGS

  initparams_(&IERR, &noptions, optvalues, optnames, CARGSLEN) ;

  if ( IERR != 0 )
    {
      printf("Check PARAMS.DAT !!\n") ;
      exit(1) ;
    }

  /* clkStart = clock() ; */
  timer_(&clkStart) ;
  /* Call IPOPT to solve the NLP */
  ipopt_(&N, X, &M, &NLB, ILB, BNDS_L, &NUB, IUB, BNDS_U, V_L, V_U, LAM, &LRW, 
	 RW, &LIW, IW, &ITER, &IERR) ;
  /* clkEnd = clock() ; */
  timer_(&clkEnd) ;

#ifndef SKIP_IPOUT
  eval_f_(&N,X,&optObj) ;

  if( M>0 )
    {
      if( !(optC = (real *)Malloc(M * sizeof(real))) ) MALLOCERR(optC) ;
      eval_c_(&N,X,&M,optC) ;
#ifdef INCLUDE_CC
      zero = 0 ;
      if (n_cc > 0)
	eval_ccerr_(&N,&M,X,optC,&zero) ;
#endif
      one = 1;
      i = idamax_(&M,optC,&one);
      cnrm = optC[i-1] ;
      cnrm = (cnrm >0) ? cnrm : -cnrm ;
      free(optC);
    }
  else
    cnrm = 0;

  ipopt_getdata_(&FFUNCS, &CFUNCS, &NOCG, &NORES, &NONEGCURV);
  fe = (FFUNCS>CFUNCS) ? FFUNCS : CFUNCS ;

  /* For running scripts - modification to suit Arvind and Andreas o/p spec */
#ifdef INCLUDE_CC
  if ( n_cc > 0 )
    {
      script = fopen("ipc.out","w") ;
      fprintf(script,"%d\t%d\t%d\t%e\t%d\t%d\t%e\t%d\t%d\t%d\t%s\n",N,M,numCcon,optObj,ITER,fe,cnrm,NORES,NONEGCURV,IERR,stub) ;
      fclose(script) ;
    }
#endif
  check_flagfile_(&ex) ;
  if ( ex )
    {
      script = fopen("ip.out","w") ;
      fprintf(script,"%18s | %5d %5d %5d | %5d/%5d %5dc%5dr%5dn (%3d) %15.8e %10.3e %9.2e",stub,N,M,nb,fe,ITER,NOCG,NORES,NONEGCURV,IERR,optObj,cnrm,(clkEnd-clkStart) ) ;
      fclose(script) ;
    }
  else
    {
      script = fopen("ip.out","w") ;
      fprintf(script," | %5d/%5d %5dc%5dr%5dn (%3d) %15.8e %10.3e %9.2e",fe,ITER,NOCG,NORES,NONEGCURV,IERR,optObj,cnrm,(clkEnd-clkStart) ) ;
      fclose(script) ;
    }

  /* Clean up a little */
  if(  numIneq > 0 )
    remove("SLACKS.DAT") ;
#endif

  /* Display some information about the problem */

  printf("IPOPT variables                       = %d\n",N) ;
  printf("IPOPT constraints                     = %d\n",M) ;
#ifdef INCLUDE_CC
  if (n_cc > 0)
    {
      printf("IPOPT complementarity constraints     = %d\n",(numCcon-2*numVineq)) ;
      printf("IPOPT variational inequalities        = %d\n",numVineq) ;
    }
#endif
  printf("No of objective gradient evaluations  = %d\n",nfgrad) ;
  printf("No of constraint jacobian evaluations = %d\n",ncjac) ;

  /* Process the output of IPOPT and display appropriate results.
     NLP.solution is written only if variable and constraint names are defined 
     in AMPL */
  need_nl = printf("\nIPOPT-2.0.1: ") ;
  need_nl = 0 ;

  if ((clen != 0) && (vlen != 0))
    {
      soln = fopen("NLP.solution", "w") ;
      fprintf(soln,"                  IPOPT-2.0.1 SOLVE SUMMARY \n") ;
      fprintf(soln,"                  ------------------------- \n") ;

      /* Output formatting */
      if ( n_obj == 0 )
	fprintf(soln,"\n                  Finding a Fesible Point\n") ;
      else
	{
	  if (!objtype[nobj])
	    fprintf(soln,"\n                  Minimization Problem\n") ;
	  else
	    fprintf(soln,"\n                  Maximization Problem\n") ;
	  eval_f_(&N,X,&optObj) ;
	  if (!objtype[nobj])
	    fprintf(soln,"Objective value = %-16g\n",optObj) ;
	  else
	    fprintf(soln,"Objective value = %-16g\n",-optObj) ;
	}

      fprintf(soln,"Solve Status : ") ;
      if ( IERR == 0 )
	fprintf(soln,"OPTIMAL SOLUTION FOUND.\n\n") ;
      else
	fprintf(soln,"Solve process aborted. Refer to message on screen.\n\n") ;

      if (vlen != 0)
	{
	  j = 0 ; /* Track of variables with lower bounds */
	  l = 0 ; /* Track of variables with upper bounds */
	  fprintf(soln,"\nVariables\n") ;
	  fprintf(soln,"\nLower          Value         Upper        Multiplier       Name\n") ;

	  for ( i = 0 ; i < n_var ; i++ )
	    {
	      bound = 0 ;
	      if ( ILB[j] == i+1 && IUB[l] == i+1 )
		{
		  mult = (V_L[j] >= V_U[l]) ? V_L[j] : V_U[l] ;
		  j++ ;
		  l++ ;
		  bound = 1 ;
		}
	      else if ( ILB[j] == i+1 )
		{
		  mult = V_L[j] ;
		  j++ ;
		  bound = 1 ;
		}
	      else if ( IUB[l] == i+1 )
		{
		  mult = V_U[l] ;
		  l++ ;
		  bound = 1 ;
		}
	      if (bound == 1)
		fprintf(soln, "\n%+0.2e      %+0.2e     %+0.2e    %+0.2e        %s",LUv[2*i],X[i],LUv[2*i+1],mult,var_name(i)) ;
	      else
		fprintf(soln, "\n%+0.2e      %+0.2e     %+0.2e    .........        %s",LUv[2*i],X[i],LUv[2*i+1],var_name(i)) ;
	    }
	}

      xknown(X);
      if (clen != 0)
	{
	  fprintf(soln,"\n\nConstraints\n") ;
	  fprintf(soln,"\nLower          Value         Upper        Mulitplier       Name\n") ;
 	 
	  for ( i = 0 ; i < n_con ; i++ )
	    {
	      conValue = conival(i,X,&nerror) ;
	      fprintf(soln,"\n%+0.2e      %+0.2e     %+0.2e    %+0.2e        %s",LUrhs[2*i],conValue,LUrhs[2*i+1],LAM[i],con_name(i)) ;     
	    }
	}
      xunknown();

      fclose(soln) ;
    }

  if ( IERR == 0 )
    write_sol("OPTIMAL SOLUTION FOUND", X, 0, 0) ;
  else if ( IERR == 1 )
    write_sol("Maximal number of iterations exceeded", X, 0, 0) ;
  else if ( IERR == 2 )
    write_sol("Line search failure", X, 0, 0) ;
  else if ( IERR == 3 )
    write_sol("Line search failure during active watchdog", X, 0, 0) ;
  else if ( IERR == 4 )
    write_sol("Some algorithmic parameters are wrong or inconsistent", 0, 0, 0) ;
  else if ( IERR == 5 )
    write_sol("Too few degrees of freedom", 0, 0, 0) ;
  else if ( IERR == 6 )
    write_sol("Equality constraints seem to be linearly dependent", X, 0, 0) ;
  else if ( IERR == 7 )
    write_sol("Some of the bounds are inconsistent", 0, 0, 0) ;
  else if ( IERR == 8 )
    write_sol("Error opening or reading some file", 0, 0, 0) ;
  else if ( IERR == 9 )
    write_sol("Step size getting too small - (tolerance too tight?)", X, 0, 0) ;
  else if ( IERR == 10 )
    write_sol("Regularization parameter in indefinite KKT systems\nis becoming too large", X, 0, 0) ;
  else if ( IERR == 11 )
    write_sol("NaN or Inf occured", X, 0, 0) ;
  else if ( IERR == 15 )
    write_sol("Internal Error -> blame Arvind", X, 0, 0) ;
  else if ( IERR == 16 )
    write_sol("Point is (almost) feasible, but IPOPT is in restoration phase", X, 0, 0) ;
  else if ( IERR == 17 )
    write_sol("Convergence to stationary point for infeasibility", X, 0, 0) ;
  else if ( IERR == 18 )
    write_sol("Restoration phase cannot further improve feasibility", X, 0, 0) ;
  else if ( IERR == 19 )
    write_sol("A slack variable becomes too small", X, 0, 0) ;
  else if ( IERR == 97 )
    write_sol("The solver executable has not been compiled for\nthis set of options", 0, 0, 0) ;
  else if ( IERR == 98 )
    write_sol("Size of double precision work space too small", 0, 0, 0) ;
  else if ( IERR == 99 )
    write_sol("Size of integer work space too small", 0, 0, 0) ;
  else
    {
      sprintf(message, "Undocumented error termination, IERR = %d\nCheck output", IERR);
      write_sol(message, 0, 0, 0) ;
    }

  /* Free up the memory allocations. */
  free(ILB) ;
  free(IUB) ;
  free(IW) ;
  free(X0) ;
  free(X) ;
  free(BNDS_L) ;
  free(BNDS_U) ;
  free(V_L) ;
  free(V_U) ;
  free(LAM) ;
  free(RW) ;
  free(conL) ;
  free(conU) ;
  free(optvalues) ;
  free(optnames);

  if ( numIneq > 0 )
    {
      free(ineqnos) ;
      free(slacks) ;
    }
#ifdef INCLUDE_CC
  if ( n_cc > 0 )
    {
      free(vineqvar) ;
      free(cvar1nos) ;
      free(cvar2nos) ;
      free(cvar1LBnds) ;
      free(cvar1UBnds) ;
      free(cvar2LBnds) ;
      free(cvar2UBnds) ;
      free(cslacks) ;
      free(rslacks) ;
    }
#endif

  return 0 ;

}
  
