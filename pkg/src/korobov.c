/* C function for computing a Korobov sequence ********************************/

#include "korobov.h"


/**
 * @title Generate n Points of a d-dimensional Korobov Sequence
 * @param n number of points
 * @param d dimension
 * @param generator vector of generator points
 * @param res pointer to the result matrix
 * @return void
 * @author Marius Hofert based on C. Lemieux's RandQMC
 */
void korobov(int n, int d, int *generator, double *res)
{
	int i, j, ij;
	double *generator_n;
	generator_n = (double *) malloc (d * sizeof(double));

	/* Init */
	for(j=0; j<d; j++){
		generator_n[j] = generator[j] / ((double) n);
		res[j*n] = 0.0;
	}

	/* Generate points */
	for(i=1; i<n; i++){ /* omit i=0 as done in init above */
		for(j=0; j<d; j++){
			ij = j*n + i;
			res[ij] = res[j*n + (i-1)] + generator_n[j];
			if(res[ij] > 1) res[ij] = res[ij] - 1.0;
		}
	}

	/* Clean-up */
	free(generator_n);
}

/**
 * @title R Interface to C for Generating a Korobov Sequence
 * @param n number of points
 * @param d dimension
 * @param generator vector of generator points
 * @return (n, d)-matrix
 * @author Marius Hofert
 */
SEXP korobov_(SEXP n, SEXP d, SEXP generator)
{
    /* Input parameters */
    int n_ = asInteger(n); /* numeric(1) */
    int d_ = asInteger(d); /* numeric(1) */
    int *generator_ = INTEGER(coerceVector(generator, INTSXP)); /* numeric(d) */

    /* Create result object */
    SEXP res = PROTECT(allocMatrix(REALSXP, n_, d_)); /* (n,d)-matrix */
    double *res_ = REAL(res); /* pointer to the values of res */

    /* Main */
    korobov(n_, d_, generator_, res_);

    /* Return */
    UNPROTECT(1); /* clean-up */
    return res;
}
