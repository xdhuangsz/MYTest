//
// MATLAB Compiler: 4.17 (R2012a)
// Date: Mon Oct 09 22:03:12 2017
// Arguments: "-B" "macro_default" "-W" "cpplib:PWdehanze" "-T" "link:lib"
// "PWdehaze.m" 
//

#ifndef __PWdehanze_h
#define __PWdehanze_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_PWdehanze
#define PUBLIC_PWdehanze_C_API __global
#else
#define PUBLIC_PWdehanze_C_API /* No import statement needed. */
#endif

#define LIB_PWdehanze_C_API PUBLIC_PWdehanze_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_PWdehanze
#define PUBLIC_PWdehanze_C_API __declspec(dllexport)
#else
#define PUBLIC_PWdehanze_C_API __declspec(dllimport)
#endif

#define LIB_PWdehanze_C_API PUBLIC_PWdehanze_C_API


#else

#define LIB_PWdehanze_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_PWdehanze_C_API 
#define LIB_PWdehanze_C_API /* No special import/export declaration */
#endif

extern LIB_PWdehanze_C_API 
bool MW_CALL_CONV PWdehanzeInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_PWdehanze_C_API 
bool MW_CALL_CONV PWdehanzeInitialize(void);

extern LIB_PWdehanze_C_API 
void MW_CALL_CONV PWdehanzeTerminate(void);



extern LIB_PWdehanze_C_API 
void MW_CALL_CONV PWdehanzePrintStackTrace(void);

extern LIB_PWdehanze_C_API 
bool MW_CALL_CONV mlxPWdehaze(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_PWdehanze
#define PUBLIC_PWdehanze_CPP_API __declspec(dllexport)
#else
#define PUBLIC_PWdehanze_CPP_API __declspec(dllimport)
#endif

#define LIB_PWdehanze_CPP_API PUBLIC_PWdehanze_CPP_API

#else

#if !defined(LIB_PWdehanze_CPP_API)
#if defined(LIB_PWdehanze_C_API)
#define LIB_PWdehanze_CPP_API LIB_PWdehanze_C_API
#else
#define LIB_PWdehanze_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_PWdehanze_CPP_API void MW_CALL_CONV PWdehaze(int nargout, mwArray& resultr, mwArray& resultg, mwArray& resultb, const mwArray& maxr, const mwArray& maxg, const mwArray& maxb, const mwArray& minr, const mwArray& ming, const mwArray& minb, const mwArray& rectpoint, const mwArray& ratio);

#endif
#endif
