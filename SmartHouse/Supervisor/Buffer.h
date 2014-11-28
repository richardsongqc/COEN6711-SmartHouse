#pragma once

#ifndef __BUFFER_H__
#define __BUFFER_H__

//#define  E_BOOL             0x01
//#define  E_U8               0x02
//#define  E_U16              0x03
//#define  E_U32              0x04
//#define  E_S8               0x05
//#define  E_S16              0x06
//#define  E_S32              0x07
//#define  E_R32              0x08
//#define  E_R64              0x09
//#define  E_STRING           0x0A
//#define  E_HYBRID           0x10
//#define  E_NONE             0xFF
//
//
//typedef unsigned char		uchar;
//typedef unsigned char		uint8;
//typedef	  signed char		sint8;
//
//typedef unsigned short		uint16;
//typedef	  signed short		sint16;
//
//typedef unsigned long		uint32;
//typedef	  signed long		sint32;
//
//typedef unsigned __int64	uint64;
//typedef   signed __int64	sint64;


//typedef uchar               U8;
//typedef sint8               S8;
//
//typedef uint16              U16;
//typedef sint16              S16;
//
//typedef uint32              U32;
//typedef sint32              S32;
//
//typedef uint64              U64;
//typedef sint64              S64;
//
//typedef float               R32;
//typedef double              R64;


//class CBuffer;
//
//class CMessage
//{
//public:
//	CMessage(U8* szBuf, U8 uLen);
//    ~CMessage(void);
//
//    CMessage(const CMessage& bi);
//    CMessage& operator =(const CMessage& bi);
//    bool operator ==(const CMessage& bi);
//
//    CMessage& operator -=( CBuffer& buffer);
//    CMessage& operator -=( CMessage& rItem);
//    CMessage& operator +=( CMessage& rItem);
//
//
//    U8          GetLen(void);
//	void        SetLen(U8 uLen);
//
//    U8          GetCmd(void);
//    void        SetCmd(U8 ucTag);
//
//	U8          GetSize(void);
//    U8*         GetBuffer(void);
//    void        Clear(void);
//	U8          GetMaxBufferSize(void);
//private:
//    U8          m_szBuf[256]; 
//};



class CBuffer
{
public:
    //CBuffer(void);
	CBuffer(unsigned char * szBuf, unsigned char uLen);
    ~CBuffer(void);

    CBuffer& operator =(const CBuffer& rBuf);
    CBuffer(const CBuffer& rBuf);
    //CBuffer& operator +=( CMessage& rBufItem);

    //CMessage* GetItem();

	unsigned char*     GetBuffer(void);
	unsigned char      GetSize(void);

	unsigned char      GetMaxBufferSize(void);
    void    Clear(void);

private:
	unsigned char      m_szBuf[256];
    
};

#endif //__BUFFER_H__