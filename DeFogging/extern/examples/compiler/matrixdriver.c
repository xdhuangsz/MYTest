/*=================================================================
 *
 * MATRIXDRIVER.C	Sample driver code that calls the shared
 *	        library created using MATLAB Compiler. Refer to the 
 *          documentation of MATLAB Compiler for more information on
 *          this
 *
 * This is the wrapper C code to call a shared library created 
 * using MATLAB Compiler.
 *
 * Copyright 1984-2007 The MathWorks, Inc.
 *
 *=================================================================*/

#include <stdio.h>

/* Include the MCR header file and the library specific header file 
 * as generated by MATLAB Compiler */
#include "libmatrix.h"

/* This function is used to display a double matrix stored in an mxArray */
void display(const mxArray* in);

int run_main(int argc, char **argv)
{
    mxArray *in1, *in2; /* Define input parameters */
    mxArray *out = NULL;/* and output parameters to be passed to the library functions */
    
    double data[] = {1,2,3,4,5,6,7,8,9};

    /* Call the mclInitializeApplication routine. Make sure that the application
     * was initialized properly by checking the return status. This initialization
     * has to be done before calling any MATLAB API's or MATLAB Compiler generated
     * shared library functions.  */
    if( !mclInitializeApplication(NULL,0) )
    {
        fprintf(stderr, "Could not initialize the application.\n");
    	return -1;
    }
    
    /* Create the input data */
    in1 = mxCreateDoubleMatrix(3,3,mxREAL);
    in2 = mxCreateDoubleMatrix(3,3,mxREAL);
    memcpy(mxGetPr(in1), data, 9*sizeof(double));
    memcpy(mxGetPr(in2), data, 9*sizeof(double));
    
    /* Call the library intialization routine and make sure that the
     * library was initialized properly. */
    if (!libmatrixInitialize()){
        fprintf(stderr,"Could not initialize the library.\n");
        return -2;
    }
    else
    {
        /* Call the library function */
        mlfAddmatrix(1, &out, in1, in2);
    /* Display the return value of the library function */
        printf("The value of added matrix is:\n");
        display(out);
    /* Destroy the return value since this variable will be reused in
     * the next function call. Since we are going to reuse the variable,
     * we have to set it to NULL. Refer to MATLAB Compiler documentation
     * for more information on this. */
        mxDestroyArray(out); out=0;
        mlfMultiplymatrix(1, &out, in1, in2);
        printf("The value of the multiplied matrix is:\n");
        display(out);
        mxDestroyArray(out); out=0;
        mlfEigmatrix(1, &out, in1);
        printf("The eigenvalues of the first matrix are:\n");
        display(out);
        mxDestroyArray(out); out=0;
        
    /* Call the library termination routine */
        libmatrixTerminate();
        
    /* Free the memory created */
        mxDestroyArray(in1); in1=0;
        mxDestroyArray(in2); in2 = 0;
    }

/* Note that you should call mclTerminate application at the end of
 * your application.
 */
    mclTerminateApplication();
    return 0;
}


/*DISPLAY This function will display the double matrix stored in an mxArray.
 * This function assumes that the mxArray passed as input contains double
 * array.
 */
void display(const mxArray* in)
{
    int i=0, j=0; /* loop index variables */
    int r=0, c=0; /* variables to store the row and column length of the matrix */
    double *data; /* variable to point to the double data stored within the mxArray */

    /* Get the size of the matrix */
    r = mxGetM(in);
    c = mxGetN(in);
    /* Get a pointer to the double data in mxArray */
    data = mxGetPr(in);
    
    /* Loop through the data and display the same in matrix format */
    for( i = 0; i < c; i++ ){
        for( j = 0; j < r; j++){
            printf("%4.2f\t",data[j*c+i]);
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    mclmcrInitialize();
    return mclRunMain((mclMainFcnType)run_main,0,NULL);
}
