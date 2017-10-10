using System;
using System.Collections.Generic;
using System.Text;

namespace NetDocGeneric
{
    public class SampleClass
    {
        public K GenMethod&lt;K&gt;(K arg) 
        { 
            // set type of arg to K
            return (arg); 
        }        
        public K GenMethodWithMixedArgs&lt;K&gt;(K arg1, K arg2, bool tf)
        { 
            // if true, return arg1
            // if false, return arg2
            return (tf ? arg1 : arg2); 
        }
        public static K GenStaticMethod&lt;K&gt;(K arg) 
        { 
            return (arg); 
        }
        public static K GenStaticMethodWithMixedArgs&lt;K&gt;
            (K arg1, K arg2, bool tf) 
        { 
            return (tf ? arg1 : arg2); 
        }
    }

    public class SampleGenericClass&lt;T&gt;
    {
        public string ParameterizedGenMethod&lt;K&gt;(T clsParam, K arg)
        { 
            return (clsParam.GetType().Name + ", " + 
                arg.GetType().Name); 
        }
        public T GenMethod&lt;T&gt;(T arg) 
        { 
            return (arg); 
        }
        public static K GenStaticMethod&lt;K&gt;(K arg) 
        { 
            return (arg); 
        }
        public static K GenStaticMethodWithMixedArgs&lt;K&gt;
            (K arg1, K arg2, bool tf) 
        { 
            return (tf ? arg1 : arg2); 
        }
        public static string ParameterizedStaticGenMethod&lt;K&gt;
            (T clsParam, K arg) 
        { 
            return (clsParam.GetType().Name + ", " + 
                arg.GetType().Name); 
        }
    }
}
