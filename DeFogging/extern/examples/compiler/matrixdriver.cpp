/*==============================================================
 *
 * MATRIXDRIVER.CPP
 * Sample driver code that calls a C++ shared library created using
 * the MATLAB Compiler. Refer to the MATLAB Compiler documentation
 * for more information on this
 *
 * This is the wrapper CPP code to call a shared library created 
 * using the MATLAB Compiler.
 *
 * Copyright 1984-2007 The MathWorks, Inc.
 *
 *============================================================*/

// Include the library specific header file as generated by the 
// MATLAB Compiler
#include "libmatrixp.h"

int run_main(int argc, char **argv)
{
    // Call application and library initialization. Perform this 
    // initialization before calling any API functions or
    // Compiler-generated libraries.
    if (!mclInitializeApplication(NULL,0)) 
    {
        std::cerr << "could not initialize the application properly"
                   << std::endl;
    	return -1;
    }
    if( !libmatrixpInitialize() )
    {
        std::cerr << "could not initialize the library properly"
                   << std::endl;
	return -1;
    }
    else
    {
        try
        {
            // Create input data
            double data[] = {1,2,3,4,5,6,7,8,9};
            mwArray in1(3, 3, mxDOUBLE_CLASS, mxREAL);
            mwArray in2(3, 3, mxDOUBLE_CLASS, mxREAL);
            in1.SetData(data, 9);
            in2.SetData(data, 9);
            
            // Create output array
            mwArray out;
            
            // Call the library function
            addmatrix(1, out, in1, in2);
            
            // Display the return value of the library function
            std::cout << "The value of added matrix is:" << std::endl;
            std::cout << out << std::endl;
            
            multiplymatrix(1, out, in1, in2);
            std::cout << "The value of the multiplied matrix is:" 
                      << std::endl;
            std::cout << out << std::endl;
            
            eigmatrix(1, out, in1);
            std::cout << "The eigenvalues of the first matrix are:"
                      << std::endl;
            std::cout << out << std::endl;
        }
        catch (const mwException& e)
        {
          std::cerr << e.what() << std::endl;
          return -2;
        }
        catch (...)
        {
          std::cerr << "Unexpected error thrown" << std::endl;
          return -3;
        }     
        // Call the application and library termination routine
        libmatrixpTerminate();
    }
/* Note that you should call mclTerminate application at the end of
 * your application. 
 */
    mclTerminateApplication();
    return 0;
}

int main()
{
    mclmcrInitialize();
    return mclRunMain((mclMainFcnType)run_main,0,NULL);
}
