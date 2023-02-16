

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Jun 01 17:54:46 2020
 */
/* Compiler settings for IPS.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

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


#ifndef __IPS_h_h__
#define __IPS_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IIPS_FWD_DEFINED__
#define __IIPS_FWD_DEFINED__
typedef interface IIPS IIPS;
#endif 	/* __IIPS_FWD_DEFINED__ */


#ifndef __IPS_FWD_DEFINED__
#define __IPS_FWD_DEFINED__

#ifdef __cplusplus
typedef class IPS IPS;
#else
typedef struct IPS IPS;
#endif /* __cplusplus */

#endif 	/* __IPS_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __IPS_LIBRARY_DEFINED__
#define __IPS_LIBRARY_DEFINED__

/* library IPS */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_IPS;

#ifndef __IIPS_DISPINTERFACE_DEFINED__
#define __IIPS_DISPINTERFACE_DEFINED__

/* dispinterface IIPS */
/* [uuid] */ 


EXTERN_C const IID DIID_IIPS;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("86D01A7C-EFC9-4F93-A988-138496A89BFD")
    IIPS : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IIPSVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IIPS * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IIPS * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IIPS * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IIPS * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IIPS * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IIPS * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IIPS * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IIPSVtbl;

    interface IIPS
    {
        CONST_VTBL struct IIPSVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IIPS_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IIPS_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IIPS_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IIPS_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IIPS_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IIPS_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IIPS_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IIPS_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_IPS;

#ifdef __cplusplus

class DECLSPEC_UUID("B1B48768-FEC3-4C97-AF9D-4B6BFD8FD7C4")
IPS;
#endif
#endif /* __IPS_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


