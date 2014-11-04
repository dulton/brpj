

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __FaceLoginOCXidl_h__
#define __FaceLoginOCXidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DFaceLoginOCX_FWD_DEFINED__
#define ___DFaceLoginOCX_FWD_DEFINED__
typedef interface _DFaceLoginOCX _DFaceLoginOCX;
#endif 	/* ___DFaceLoginOCX_FWD_DEFINED__ */


#ifndef ___DFaceLoginOCXEvents_FWD_DEFINED__
#define ___DFaceLoginOCXEvents_FWD_DEFINED__
typedef interface _DFaceLoginOCXEvents _DFaceLoginOCXEvents;
#endif 	/* ___DFaceLoginOCXEvents_FWD_DEFINED__ */


#ifndef __FaceLoginOCX_FWD_DEFINED__
#define __FaceLoginOCX_FWD_DEFINED__

#ifdef __cplusplus
typedef class FaceLoginOCX FaceLoginOCX;
#else
typedef struct FaceLoginOCX FaceLoginOCX;
#endif /* __cplusplus */

#endif 	/* __FaceLoginOCX_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __FaceLoginOCXLib_LIBRARY_DEFINED__
#define __FaceLoginOCXLib_LIBRARY_DEFINED__

/* library FaceLoginOCXLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_FaceLoginOCXLib;

#ifndef ___DFaceLoginOCX_DISPINTERFACE_DEFINED__
#define ___DFaceLoginOCX_DISPINTERFACE_DEFINED__

/* dispinterface _DFaceLoginOCX */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DFaceLoginOCX;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("BB80CCC6-8390-477C-91D5-2CF335489402")
    _DFaceLoginOCX : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DFaceLoginOCXVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DFaceLoginOCX * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DFaceLoginOCX * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DFaceLoginOCX * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DFaceLoginOCX * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DFaceLoginOCX * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DFaceLoginOCX * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DFaceLoginOCX * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DFaceLoginOCXVtbl;

    interface _DFaceLoginOCX
    {
        CONST_VTBL struct _DFaceLoginOCXVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DFaceLoginOCX_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DFaceLoginOCX_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DFaceLoginOCX_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DFaceLoginOCX_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DFaceLoginOCX_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DFaceLoginOCX_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DFaceLoginOCX_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DFaceLoginOCX_DISPINTERFACE_DEFINED__ */


#ifndef ___DFaceLoginOCXEvents_DISPINTERFACE_DEFINED__
#define ___DFaceLoginOCXEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DFaceLoginOCXEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DFaceLoginOCXEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("F73D1D70-33B6-4CBD-B870-A7D143A1A9AD")
    _DFaceLoginOCXEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DFaceLoginOCXEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DFaceLoginOCXEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DFaceLoginOCXEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DFaceLoginOCXEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DFaceLoginOCXEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DFaceLoginOCXEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DFaceLoginOCXEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DFaceLoginOCXEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DFaceLoginOCXEventsVtbl;

    interface _DFaceLoginOCXEvents
    {
        CONST_VTBL struct _DFaceLoginOCXEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DFaceLoginOCXEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DFaceLoginOCXEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DFaceLoginOCXEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DFaceLoginOCXEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DFaceLoginOCXEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DFaceLoginOCXEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DFaceLoginOCXEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DFaceLoginOCXEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_FaceLoginOCX;

#ifdef __cplusplus

class DECLSPEC_UUID("AEFA1F7C-F5A9-44B3-86B8-53D586CAAFAB")
FaceLoginOCX;
#endif
#endif /* __FaceLoginOCXLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


