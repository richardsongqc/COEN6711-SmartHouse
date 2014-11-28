#include "StdAfx.h"
#include <SerialProtocol.h>
#include "Buffer.h"
#include <stdlib.h>
#include <string>
//#include "AES.h"


#define RADIX_HEX           (16)
#define OFFSET_BYTE         (8U)


//static unsigned char key[] = 
//{
//	0x2b, 0x7e, 0x15, 0x16, 
//	0x28, 0xae, 0xd2, 0xa6, 
//	0xab, 0xf7, 0x15, 0x88, 
//	0x09, 0xcf, 0x4f, 0x3c
//};
//
using namespace std;


//
//CMessage::~CMessage(void)
//{
//}
//
//
//CMessage::CMessage(U8* szBuf, U8 uLen)
//{
//    memset(m_szBuf, 0, sizeof(m_szBuf));
//    memcpy( m_szBuf, szBuf, uLen);
//}
//
//
//CMessage::CMessage(const CMessage& bi)
//{
//    memcpy( m_szBuf, bi.m_szBuf, sizeof(bi.m_szBuf));
//}
//
//
//CMessage& CMessage::operator =(const CMessage& bi)
//{
//    memcpy( m_szBuf, bi.m_szBuf, sizeof(bi.m_szBuf));
//
//    return *this;
//}
//
//
//
//bool CMessage::operator ==(const CMessage& bi)
//{
//    return ( memcmp( m_szBuf, bi.m_szBuf, sizeof(bi.m_szBuf) ) == 0 )? true:false;
//}
//
//CMessage& CMessage::operator -= ( CBuffer& buffer)
//{
//    memset( m_szBuf, 0 , sizeof(m_szBuf));
//
//    U8 uLen = buffer.GetLen();
//    if( uLen == 0 )
//    {
//        return *this;
//    }
//
//    m_ucCmd = *(buffer.GetBuffer()+3);
//    m_uLen = *(buffer.GetBuffer()+4) << OFFSET_BYTE;
//    m_uLen |= *(buffer.GetBuffer()+5) & 0xFF;
//    memcpy( m_szBuf, buffer.GetBuffer()+3, m_uLen + 3 );
//    
//    uLen -= ( m_uLen + 3);
//    buffer.SetLen(uLen);
//
//    if( uLen > 0 )
//    {
//        memcpy( buffer.GetBuffer()+3,  buffer.GetBuffer() + m_uLen + 6, uLen);
//        memset( buffer.GetBuffer() + uLen + 3 , 0, buffer.GetMaxBufferSize() - uLen -3 );
//    }
//    else
//    {
//        memset( buffer.GetBuffer()+3, 0, sizeof(m_szBuf)-3);
//    }
//
//    
//    return *this;
//}
//
//
//CMessage& CMessage::operator -=( CMessage& rItem)
//{
//    memset( m_szBuf, 0 , sizeof(m_szBuf));
//
//    U8 uLen = rItem.GetLen();
//    if( uLen == 0 )
//    {
//        return *this;
//    }
//
//    m_ucCmd = *(rItem.GetBuffer() + 3);
//    m_uLen = *(rItem.GetBuffer() + 4) << OFFSET_BYTE;
//    m_uLen |= *(rItem.GetBuffer() + 5) & 0xFF;
//    memcpy( m_szBuf, rItem.GetBuffer() + 3 , m_uLen + 3 );
//    
//    uLen -= ( m_uLen + 3 );
//    rItem.SetLen(uLen);
//    if( uLen > 0 )
//    {
//        memcpy( rItem.GetBuffer()+3,  rItem.GetBuffer() + m_uLen + 6, uLen);
//        memset( rItem.GetBuffer() + uLen + 3 , 0, rItem.GetMaxBufferSize() - uLen -3 );
//    }
//    else
//    {
//        memset( rItem.GetBuffer()+3, 0, sizeof(m_szBuf)-3);
//    }
//
//    
//    return *this;
//}
//
//
//CMessage& CMessage::operator +=( CMessage& rItem)
//{
//    U8     uLen = GetLen();
//    U8     nItemSize = rItem.GetSize();
//
//    memcpy( m_szBuf + uLen + 3, rItem.GetBuffer(), nItemSize );
//    uLen += nItemSize;
//
//    SetLen(uLen);
//    SetCmd( E_HYBRID );        
//    return *this;
//}
//
//
//U8 CMessage::GetLen(void)
//{
//    U8     uLen = m_szBuf[1] << OFFSET_BYTE;
//    uLen |= m_szBuf[2];
//
//    return ( m_uLen == uLen )? uLen: 0;
//}
//
//
//U8 CMessage::GetCmd(void)
//{
//    return ( this->m_ucCmd == m_szBuf[0] )? m_ucCmd: E_NONE;
//}
//
//
//void CMessage::SetCmd(U8 ucTag)
//{
//    this->m_ucCmd = ucTag;
//    m_szBuf[0] = ucTag;
//}
//
//U8* CMessage::GetBuffer(void)
//{
//    return m_szBuf;
//}

///////////////////////////////////////////////////

//CBuffer::CBuffer(void) 
//{
//    memset( m_szBuf, 0, sizeof(m_szBuf));
//}


CBuffer::CBuffer(unsigned char * szBuf, unsigned char uLen)
{
	memset(m_szBuf, 0, sizeof(m_szBuf));
    memcpy( m_szBuf, szBuf, uLen);
}


CBuffer::~CBuffer(void)
{
}


CBuffer& CBuffer::operator =(const CBuffer& rBuf)
{
    //TODO: insert return statement here
    memcpy( m_szBuf, rBuf.m_szBuf, sizeof(rBuf.m_szBuf));

    return *this;
}


CBuffer::CBuffer(const CBuffer& rBuf)
{
    memcpy( m_szBuf, rBuf.m_szBuf, sizeof(rBuf.m_szBuf));
}



//CBuffer& CBuffer::operator +=( CMessage& rBufItem)
//{
//    //TODO: insert return statement here
//    U8 uLen = this->GetLen();
//    U8 nItemSize = rBufItem.GetSize();
//
//    memcpy( m_szBuf + 3 + uLen, rBufItem.GetBuffer(), nItemSize );
//    uLen += nItemSize;
//
//    this->SetLen(uLen);
//
//    return *this;
//}

//
//U8 CMessage::GetMaxBufferSize(void)
//{
//    return sizeof(m_szBuf);
//}


unsigned char CBuffer::GetMaxBufferSize(void)
{
	return (unsigned char) sizeof(m_szBuf);
}


void CBuffer::Clear(void)
{
    memset( m_szBuf, 0, sizeof(m_szBuf));
}

unsigned char* CBuffer::GetBuffer(void)
{
	return m_szBuf;
}


//void CMessage::Clear(void)
//{
//    memset( m_szBuf, 0, sizeof(m_szBuf));
//}


//U8 CBuffer::Receive(CSocket* pSocket)
//{
//    //char tcBuf[MAX_BUFFER_SIZE*2] = {0};
//    U8 uLen =  pSocket->Receive(m_szBuf, GetMaxBufferSize(), 0);
//
//    if( uLen > 0 )
//    {
//        m_ucCmd = m_szBuf[0];
//
//        m_uLen = m_szBuf[1] << OFFSET_BYTE;
//        m_uLen |= m_szBuf[2];
//    }
//
//    return uLen;
//}
//
//
//U8 CBuffer::Send(CSocket* pSocket)
//{
//    if( NULL == pSocket )
//    {
//        return 0;
//    }
//   
//    return pSocket->Send(m_szBuf, GetSize(), 0);
//}
