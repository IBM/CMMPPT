// Copyright (C) 2000, International Business Machines
// Corporation and others.  All Rights Reserved.
/*
nparam     (Input) Number of free variables, i.e., the dimension of x.

nf         (Input) Number of objective functions (nf in the algorithm description, possibly equal to zero). 

nfsr       (Input) Number (possibly zero) of sets of sequentially related objective functions (nfsr in the problem description). 

nineqn     (Input) Number (possibly zero) of nonlinear inequality constraints (ni in the algorithm description). 

nineq      (Input) Total number (possibly equal to nineqn) of inequality constraints (ti in the algorithm description). 

neqn       (Input) Number (possibly zero) of nonlinear equality constraints (ne in the algorithm description). 

neq        (Input) Total number (possibly equal to neqn) of equality constraints (te in the algorithm description). 

ncsrl      (Input) Number (possibly zero) of sets of linear sequentially related constraints (sr in the problem description). 

ncsrn      (Input) Number (possibly zero) of sets of nonlinear sequentially related constraints (nsr in the problem description).

mesh pts   (Input) Pointer to an array of integers of dimension 
              max {1,nfsr + ncsrn + ncsrl }
           indicating the number of objectives/constraints in each speci c set of sequentially related objectives/constraints.
           Elements 0 through nfsr-1 should contain the number of objectives in each sequentially related objective set, 
           the next ncsrn elements should contain the number of constraints in each nonlinear sequentially related constraint set, 
           and the final ncsrl elements should contain the number of constraints in each linear sequentially related constraint set.

mode       (Input) mode = CBA with the following options:

iprint     (Input) Parameter indicating the desired output

miter      (Input) Maximum number of iterations allowed by the user before termination of execution.

inform     (Output) Parameter indicating the status of the execution of CFSQP:

bigbnd     (Input) (see also bl and bu below) Plays the role of infinity.

eps        (Input)

epseqn     (Input) Maximum violation of nonlinear equality constraints allowed by the user at an optimal point

udelta     (Input) The perturbation size the user suggests to use in approximating gra dients by finite difference.

bl         (Input) Array of dimension nparam containing lower bounds for the components of x.

bu         (Input) Array of dimension nparam containing upper bounds for the components of x.

x          (Input) Initial guess. 
           (Output) Iterate at the end of execution.

f          Array of dimension

g          Array of dimension

lambda     Array of dimension

obj        (Input) A pointer to the user-defined function that computes the value of the

constr     (Input) A pointer to the user-defined function that computes the value of the constraints.

gradob     (Input) A pointer to the function that computes the gradients of the objective functions.

gradcn     (Input) A pointer to the function that computes the gradients of the constraints.

cd         (Input) A void pointer which may be used by the user to pass "client data" between their main program 
                   and the objective and constraint functions (and their gradients).
*/
#include <memory.h>
#define INF 1.0e20
#define EPS 1.0e-8
#define MITER 5000
#define FP_INIT 2
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define cfsqp_abs(a) (((a)>0.0)?(a):-(a))
#include "cfsqpusr.h"

struct _LinearConstraints {
	int lda;
	int nclin, ncnln;
	int *indicator;
	double *lower, *upper, *a;
};

typedef struct _LinearConstraints LinearConstraints;

LinearConstraints linearConstraints;

//This function produces the value and gradients for the (quadratic) objective.

extern void funobj_(int *mode, int *n, double *x, double *objf, double *objgrd, int *nstate); 

//This function is user provided - to produce values and gradients for the constraints.
extern void funcon_(int *mode, int *ncnln, int *n, int *nrowj, int *needc, double *x, double *c, double *cjac, int *nstate);
// This function provided in dfotest.f (can be changed or removed by the user if desired) prints out message
// describing the information returned by the optimizer CFSQP)
extern void mntout_(int *inform);

// declaration for common variable block from fortran routines
extern struct  frtcom {
                     int iout;
                     int  iprint;
                     double  mcheps;
                     double  cnstol;
                   } dfocm_;

//Prototypes for CFSQP callbacks.

void obj(int nparam, int j, double *x, double *fj, void *cd);
void gradobj(int nparam,int j,double *x, double *gradfj, void (*dummy)(int,int,double *,double *,void *), void *cd);
void constr(int nparam,int j, double *x,double *gj, void *cd);
void gradcn(int nparam,int j,double *x, double *gradgj, void (*dummy)(int,int,double *,double *,void *), void *cd);

//C prototype for trust region minimization call.
void mintr_(int *_n, 
		   double *x0,
		   double *mval,
		   double *delta,
		   double *lwrbnd,
		   double *uprbnd,
		   double *a,
		   int *_lda,
		   int *_ldcj ,
		   int *nclin, 
		   int *ncnln,
		   double *wrk,
		   int *lwrk, 
		   int *_iwrk, 
		   int *_liwrk, 
		   int *_inform);

void mintr_(int *_n, 
		   double *x0,
		   double *mval,
		   double *delta,
		   double *lwrbnd,
		   double *uprbnd,
		   double *a,
		   int *_lda,
		   int *_ldcj ,
		   int *_nclin, 
		   int *_ncnln,
		   double *wrk,
		   int *_lwrk, 
		   int *iwrk, 
		   int *_liwrk, 
		   int *_inform)
{
	//Constants held across invocations.
	static int nconslin, nconsnln, nconsleq, nconsneq;
	static double *f=NULL;
	static double *g=NULL;
	static double *lambda=NULL;
	static int *indicator=NULL; //holds index (+/-) of original constraint: + => upper bound is finite; - => lower bound is finite.
	static double *bl, *bu;
	int n=*_n, lda=*_lda, ldcj=*_ldcj, nclin=*_nclin, ncnln=*_ncnln, lwrk=*_lwrk, liwrk=*_liwrk;
	int inform;
	int mesh[]={0};
	int nf=1;
	int nineqn;
	int nineq;
	int neq;
	int neqn;
	int nparam;
	int nfsr;
	int i;
	//Constraints need to be reordered.
	//MINTR variable bounds; linear constraint bounds; nonlinear constraint bounds.
	//CFSQP variable bounds; ( all nonlinear constraint bounds and  linear constraint bounds are assumed to be zero)
//	fpregs(FP_INIT,NULL);
	if(!indicator) {
		double *l=lwrbnd+n;
		double *u=uprbnd+n;
		// indicator[j] is the number and nature of the constraint in MINTR coordinate space (1 based indexing)
		nconslin=0; //linear inequalities 				   
		nconsleq=0; //linear equalities				   
		nconsnln=0; //nonlinear inequalities				   
		nconsneq=0; //nonlinear equalities				   
		for(i=0;i<nclin;++i) {
			if(cfsqp_abs(l[i]-u[i])<dfocm_.cnstol) ++nconsleq;
			else if(l[i]>-INF) ++nconslin;
			else if(u[i]<INF) ++nconslin;
		}
		for(i=0;i<ncnln;++i) {
			if(cfsqp_abs(l[i+nclin]-u[i+nclin])<dfocm_.cnstol) ++nconsneq;
			else if(l[i+nclin]>-INF) ++nconsnln; 
			else if(u[i+nclin]<INF) ++nconsnln;
		}
		indicator=(int *)malloc((nconsleq+nconslin+nconsnln+nconsneq)*sizeof(int));
		nconslin=0; //linear inequalities				   
		nconsleq=0; //linear equalities				   
		nconsnln=0; //nonlinear inequalities				   
		nconsneq=0; //nonlinear equalities
		// collect the constraint indices in the proper order corresponding
		// to CFSQP order				   
		for(i=0;i<ncnln;++i) {
			if(cfsqp_abs(l[i+nclin]-u[i+nclin])<dfocm_.cnstol) continue;
			if(l[i+nclin]>-INF) {
				indicator[nconsnln]=-(i+nclin+1);
				++nconsnln;
			}
			if(u[i+nclin]<INF) {
				indicator[nconsnln]=(i+nclin+1);
				++nconsnln;
			}
		}
		for(i=0;i<nclin;++i) {
			if(cfsqp_abs(l[i]-u[i])<dfocm_.cnstol) continue;
			if(l[i]>-INF) {
				indicator[nconsnln+nconslin]=-(i+1);
				++nconslin;
			}
			if(u[i]<INF) {
				indicator[nconsnln+nconslin]=(i+1);
				++nconslin;
			}
		}
		for(i=0;i<ncnln;++i) {
			if(cfsqp_abs(l[i+nclin]-u[i+nclin])<dfocm_.cnstol) {
				indicator[nconsnln+nconslin+nconsneq]=(i+nclin+1);
				++nconsneq;
			}
		}
		for(i=0;i<nclin;++i) {
			if(cfsqp_abs(l[i]-u[i])<dfocm_.cnstol) {
				indicator[nconsnln+nconsneq+nconslin+nconsleq]=(i+1);
				++nconsleq;
			}
		}
	}
	nineqn=nconsnln;
	nineq=nconslin+nconsnln;
	neqn=nconsneq;
	neq=nconsneq+nconsleq;
	nparam=n;
	nfsr=0;
	if(!f) {
		f=(double *)calloc(nf,sizeof(double));
		g=(double *)calloc(nineq+neq,sizeof(double));
		lambda=(double *)calloc(nineq+neq+nf+nparam,sizeof(double));
		bl=(double *)calloc(nparam,sizeof(double));
		bu=(double *)calloc(nparam,sizeof(double));
	}	
	linearConstraints.lda=lda;
	linearConstraints.a=a;
	linearConstraints.nclin=nclin;
	linearConstraints.ncnln=ncnln;
	linearConstraints.indicator=indicator;
	linearConstraints.lower=lwrbnd;
	linearConstraints.upper=uprbnd;
//
//  Construct the trust region.
//
	for(i=0;i<nparam;++i) {
		bl[i]=max(lwrbnd[i],x0[i]-*delta);
		bu[i]=min(uprbnd[i],x0[i]+*delta);
	}
   	cfsqp(nparam,
		  nf,      //number of objective functions
		  nfsr,    //number of sequentially related objective functions
		  nineqn,  //Number (possibly zero) of nonlinear inequality constraints
		  nineq,   //Total number (possibly equal to nineqn) of inequality constraints (ti in the algorithm description). 
		  neqn,    //Number (possibly zero) of nonlinear equality constraints 
		  neq,     //Total number (possibly equal to neqn) of equality constraints (te in the algorithm description). 
		  0,       //Number (possibly zero) of sets of linear sequentially related constraints (sr in the problem description). 
		  0,       //Number (possibly zero) of sets of nonlinear sequentially related constraints (nsr in the problem description).
		  mesh,    //Pointer to an array of integers of dimension max {1,nfsr + ncsrn + ncsrl }
		  100,     //mode
		  0,       //iprint
		  MITER,     //miter
		  &inform, //output status
		  INF,     //infinity
		  EPS,     //eps
		  dfocm_.cnstol, //epseqn,
		  0.0,     //udelta
		  bl,
		  bu,
		  x0,
		  f,
		  g,
		  lambda,
		  obj,
		  constr,
		  gradobj,
		  gradcn,
		  &linearConstraints);
//
//Map CFSQP output status to output status understood by DFO.
//
        mntout_(&inform);
	switch(inform) {
	case 0:
		*_inform=0;
		break;
	case 1:
		*_inform=2;
		break;
	case 2:
		*_inform=2;
		break;
	case 3:
		*_inform=0;
		break;
	case 4:
		*_inform=0;
		break;
	case 5:
		*_inform=2;
		break;
	case 6:
		*_inform=2;
		break;
	case 7:
		*_inform=1;
		break;
	case 8:
		*_inform=0;
		break;
	case 9:
		*_inform=0;
		break;
	default:
		fprintf(stderr,"mintr: unexpected value of inform: %d\n", inform);
	}
        if ( inform == 3 && inform == 4 &&  inform == 8 && inform == 9 )
          {
              for(i=0;i<nineq;++i) {
                if ( g[i] > dfocm_.cnstol ) *_inform=2;
              }
              for(i=0;i<neq;++i) {
                if ( cfsqp_abs(g[nineq+i]) > dfocm_.cnstol ) *_inform=2;
              }
          }          
			
	*mval=*f;
//	fpregs(FP_INIT,NULL);
}


// interface between CFSQP and FUNOBJ
void obj(int nparam, int j, double *x, double *fj, void *cd)
{
	int	mode=0;
	int n=nparam;
	double *objgrd=NULL;
	int nstate=0;
	if(j!=1) {
		fprintf(stderr,"Oops!\n");
		exit(0);
	}	
//	extern void funobj(int *mode, int *n, double *x, double *objf, double *objgrd, int *nstate); 
	funobj_(&mode, &n, x, fj, objgrd, &nstate);
}

void gradobj(int nparam,int j,double *x, double *gradfj, void (*dummy)(int,int,double *,double *,void *), void *cd)
{
	int	mode=1;
	int n=nparam;
	double *objf=NULL;
	int nstate=0;
	if(j!=1) {
		fprintf(stderr,"Oops!\n");
		exit(0);
	}	
//	extern void funobj(int *mode, int *n, double *x, double *objf, double *objgrd, int *nstate); 
	funobj_(&mode, &n, x, objf, gradfj, &nstate);
}

static int *needc=NULL; //Integer vector used by funcon to determine which values to return.
static double *c;       //Double work vector used by funcon to return constraint values.
static double *cjac;    //Double work matrix used by funcon to return Jacobian.

// Interface between CFSQP and FUNCON (for constraint value computation)
void constr(int nparam, int j, double *x, double *gj, void *cd)
{
	int	mode=0;
	int n=nparam;
	double *objf=NULL;
	int nstate=0;
	LinearConstraints *linearConstraints = (LinearConstraints *)cd;
	int ncnln = linearConstraints->ncnln;
	int jj = linearConstraints->indicator[j-1], jjj=cfsqp_abs(jj)-1;
	if(needc==NULL) {
		needc=(int *)malloc(ncnln*sizeof(int));
		c=(double *)malloc(ncnln*sizeof(double));
		cjac=(double *)malloc(ncnln*nparam*sizeof(double));
	}
	if(jjj<linearConstraints->nclin) {
//
//  Need to compute linear stuff here.
//
		int lda=linearConstraints->lda;
		double objval=0.0, *a=linearConstraints->a;
		int k;
//
//      Get constraint value. (FORTRAN stores data columnwise.)
//
		for(k=0;k<nparam;++k) {
			objval+=a[lda*k+jjj]*x[k];
		}
		if(jj>0) {
			gj[0]=objval-linearConstraints->upper[jjj+nparam];
		}		
		if(jj<0) {
			gj[0]=-objval+linearConstraints->lower[jjj+nparam];
		}
//      printf("%10.6f\n", gj[0]);
		return;
	}
	else {
//  extern void funcon(int *mode, int *ncnln, int *n, int *nrowj, int *needc, double *x, double *c, double *cjac, double *nstate);
		memset(needc,'\0',ncnln*sizeof(int));
		needc[jjj-linearConstraints->nclin]=1;
		funcon_(&mode, &ncnln, &n, &ncnln, needc, x, c, cjac, &nstate);
		if(jj>0) {
			gj[0]=c[jjj-linearConstraints->nclin]-linearConstraints->upper[jjj+nparam];
		}
		if(jj<0) {
			gj[0]=-c[jjj-linearConstraints->nclin]+linearConstraints->lower[jjj+nparam];
		}
	}	
}	

// Interface between CFSQP and FUNCON (for gradient computation)

void gradcn(int nparam,int j, double *x, double *gradgj, void (*dummy)(int,int,double *,double *,void *), void *cd)
{
	int	mode=1;
	int n=nparam;
	double *objf=NULL;
	int nstate=0;
	LinearConstraints *linearConstraints = (LinearConstraints *)cd;
	int ncnln = linearConstraints->ncnln;
	int jj = linearConstraints->indicator[j-1], jjj=cfsqp_abs(jj)-1, k;
	if(needc==NULL) {
		needc=(int *)malloc(ncnln*sizeof(int));
		c=(double *)malloc(ncnln*sizeof(double));
		cjac=(double *)malloc(ncnln*nparam*sizeof(double));
	}
	if(jjj<linearConstraints->nclin) {
//
//  Need to compute linear stuff here.
//
		int lda=linearConstraints->lda;
		double objval=0.0, *a=linearConstraints->a;
//
//      Get constraint coefficients. (FORTRAN stores data columnwise.)
//
		if(jj>0) {
			for(k=0;k<nparam;++k) {
				gradgj[k]=a[lda*k+jjj];
			}
		}		
		if(jj<0) {
			for(k=0;k<nparam;++k) {
				gradgj[k]=-a[lda*k+jjj];
			}
		}		
		return;
	}
	else {
//  extern void funcon(int *mode, int *ncnln, int *n, int *nrowj, int *needc, double *x, double *c, double *cjac, double *nstate);
		memset(needc,'\0',ncnln*sizeof(int));
		needc[jjj-linearConstraints->nclin]=1;
		funcon_(&mode, &ncnln, &n, &ncnln, needc, x, c, cjac, &nstate);
		if(jj>0) {
			for(k=0;k<nparam;++k) {
				gradgj[k]=cjac[k*ncnln+jjj-linearConstraints->nclin];
			}
		}
		if(jj<0) {
			for(k=0;k<nparam;++k) {
				gradgj[k]=-cjac[k*ncnln+jjj-linearConstraints->nclin];
			}
		}
	}	
}
