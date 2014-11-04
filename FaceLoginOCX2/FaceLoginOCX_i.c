

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Fri Oct 31 15:55:12 2014
 */
/* Compiler settings for .\FaceLoginOCX.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
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

MIDL_DEFINE_GUID(IID, LIBID_FaceLoginOCXLib,0x3656C95A,0xA6BA,0x41FA,0x89,0x71,0x97,0x9D,0xEB,0xB8,0xD9,0x6E);


MIDL_DEFINE_GUID(IID, DIID__DFaceLoginOCX,0xBB80CCC6,0x8390,0x477C,0x91,0xD5,0x2C,0xF3,0x35,0x48,0x94,0x02);


MIDL_DEFINE_GUID(IID, DIID__DFaceLoginOCXEvents,0xF73D1D70,0x33B6,0x4CBD,0xB8,0x70,0xA7,0xD1,0x43,0xA1,0xA9,0xAD);


MIDL_DEFINE_GUID(CLSID, CLSID_FaceLoginOCX,0xAEFA1F7C,0xF5A9,0x44B3,0x86,0xB8,0x53,0xD5,0x86,0xCA,0xAF,0xAB);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



