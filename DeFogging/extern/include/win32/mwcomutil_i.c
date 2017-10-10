

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Thu Dec 29 13:47:07 2011
 */
/* Compiler settings for win32\mwcomutil.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IMWUtil,0xC47EA90E,0x56D1,0x11d5,0xB1,0x59,0x00,0xD0,0xB7,0xBA,0x75,0x44);


MIDL_DEFINE_GUID(IID, LIBID_MWComUtil,0xF1F1B168,0xB891,0x4B26,0x98,0xCF,0x24,0x90,0x6D,0x43,0xEE,0x92);


MIDL_DEFINE_GUID(CLSID, CLSID_MWField,0xD2A5A1FD,0xEF77,0x48B6,0x99,0x30,0x0A,0xA7,0x61,0xB6,0xDF,0x8A);


MIDL_DEFINE_GUID(CLSID, CLSID_MWStruct,0xCE0EED2B,0x873C,0x4FB3,0xB5,0x70,0x0F,0x8B,0x7D,0xA5,0x0A,0x14);


MIDL_DEFINE_GUID(CLSID, CLSID_MWComplex,0x55F2AD91,0x3EBF,0x4CDA,0xA0,0xCA,0xC8,0x06,0xE9,0x4D,0xE6,0x78);


MIDL_DEFINE_GUID(CLSID, CLSID_MWSparse,0x5BDF2A5B,0x0043,0x4E63,0xA9,0xA1,0xDB,0xAF,0x13,0xC8,0xF7,0x77);


MIDL_DEFINE_GUID(CLSID, CLSID_MWArg,0x64869C09,0xCD85,0x4B36,0x91,0x2E,0xC7,0x20,0x41,0x7C,0x5F,0x89);


MIDL_DEFINE_GUID(CLSID, CLSID_MWArrayFormatFlags,0xBF8ED559,0xD62C,0x422D,0x83,0x4B,0x85,0x00,0xF5,0xA3,0x36,0x05);


MIDL_DEFINE_GUID(CLSID, CLSID_MWDataConversionFlags,0xE9F39BE7,0x3AB3,0x41FB,0x96,0x22,0x61,0x38,0x28,0xC8,0xCC,0x07);


MIDL_DEFINE_GUID(CLSID, CLSID_MWUtil,0x4CD60BF8,0x2A60,0x4850,0xA1,0x64,0xF0,0x3D,0xEF,0x0D,0x74,0xF9);


MIDL_DEFINE_GUID(CLSID, CLSID_MWFlags,0xC6D04A5F,0x4F86,0x4182,0xAD,0xB2,0x4C,0xEB,0xCC,0x3B,0xC0,0x8B);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



