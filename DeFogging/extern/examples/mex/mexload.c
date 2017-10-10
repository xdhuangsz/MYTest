#include <string.h>
#include "mex.h"
#include "mat.h"

/*
 * mexload.c : Example MEX-file code emulating the functionality of the
 *             MATLAB command LOAD
 *
 * Copyright 1984-2011 The MathWorks, Inc.
 * $Revision: 1.7.4.6 $
 */

void
mexFunction( int nlhs, mxArray *plhs[],
	     int nrhs, const mxArray *prhs[] )
{
  MATFile *ph;

  char *matfile;
  char *varnamelist[100];
  
  int   counter=0;

  (void) nlhs;      /* unused parameters */
  (void) plhs;
  
  if(nrhs==0) {
    /*
     * no arguments; so load by default from 'matlab.mat'
     */
    matfile=(char *)mxCalloc(11,sizeof(char));
    matfile=strcat(matfile,"matlab.mat");
  }
  else {
    /* 
     * parse the argument list for the
     * filename and any variable names 
     *
     * note: the first argument must be a filename.
     *       therefore you can't request specific
     *       variables from 'matlab.mat' without
     *       passing the filename in
     */
    
    /*
     * we're adding 5 to the size of the string.
     * 1 for '\0' and 4 in case the '.mat' was
     * forgotten
     */
    size_t buflen=mxGetN(prhs[0])+5;
    int index;
    
    /* filename */
    matfile=(char *)mxCalloc((mwSize)buflen,sizeof(char));
    mxGetString(prhs[0],matfile,(mwSize)buflen);
    
    /* 
     * make sure '.mat' suffix exists
     */
    {
        const char *lastdot = strrchr( matfile, '.' );
        const char *lastslash = strrchr( matfile, '/' );
        const char *lastbslash = strrchr( lastslash ? lastslash : matfile, '\\');
        lastslash = lastbslash ? lastbslash : lastslash;
 
        if(!lastdot || (!lastslash || lastdot < lastslash))
            matfile=strcat(matfile,".mat");
    }

    /* variable name list */
    for(index=1;index<nrhs;index++) {
      
      size_t varlength=mxGetN(prhs[index])+1;
      
      varnamelist[counter]=(char *)mxCalloc(varlength,sizeof(char));
      mxGetString(prhs[index],varnamelist[counter],(mwSize)varlength);
      
      counter++;
    }
  }

  /* open the MAT-File to read from */
  ph=matOpen(matfile,"r");

  if(ph<(MATFile *)3) {
    char errmsg[]="Failed to open '";
    
    strcat(errmsg,matfile);
    strcat(errmsg,"'");

    mxFree(matfile);
    mexErrMsgIdAndTxt( "MATLAB:mexload:openFailed", errmsg);
  }
  
  if(counter>0) {
    /* there where variables in the argument list */
    mxArray *var;
    int      index;
    
    for(index=0;index<counter;index++) {
      var=matGetVariable(ph,varnamelist[index]);
      mexPutVariable("caller", varnamelist[index],var);
      
      mxFree(varnamelist[index]);
    }
  }
  else {
    /* 
     * the variable argument list 
     * was empty so get everything 
     */
    mxArray *var;
    const char *var_name;
    
    while((var=matGetNextVariable(ph, &var_name))!=NULL) {
      mexPutVariable("caller", var_name, var);
    }
  }
  
  matClose(ph);
  mxFree(matfile);
}
