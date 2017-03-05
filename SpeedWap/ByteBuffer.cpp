// ByteBuffer.cpp: implementation of the CByteBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ByteBuffer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define SIZE_BYTE (1)
#define SIZE_SHORT (2)
#define SIZE_INT	(4)
#define SIZE_LONG	(4)

CByteBuffer::CByteBuffer(BYTE* data,int len){
	length = 0;
	buffer = NULL;
	zero	= new BYTE[SIZE_BYTE]    ;
    zero[0] = 0;
	SetBuffer(data,len);
}

CByteBuffer::CByteBuffer()
{
	length = 0;
	buffer = NULL;
	zero	= new BYTE[SIZE_BYTE]    ;
    zero[0] = 0;

}

CByteBuffer::~CByteBuffer()
{
	if(zero) delete zero;
	zero	= NULL;
	if(buffer && (length > 0))	delete buffer;
	buffer = NULL;
}

void CByteBuffer::AppendBytes0(BYTE* data, int count)
{
	
    BYTE* newBuf = new BYTE[length + count];
	memset(newBuf,0,length + count) ;
    if (length > 0) {
        memcpy(newBuf,buffer,length);//hien tai dan co length bytes
    }
    //Copy them vao newBuf
	memcpy(&newBuf[length],data,count);//copy them vao 
		
    SetBuffer(newBuf,length + count);
	delete newBuf;
}

void CByteBuffer::SetBuffer(BYTE* newbuffer, int len)
{
	if(newbuffer == NULL || len == 0){ 
		delete buffer;
		length = 0;
		buffer = NULL;
		return;
	}
	if(buffer ){ 
		delete buffer;
		buffer = NULL;
		length = 0;
	}

    buffer = new BYTE [len] ;
	memset(buffer,0,len);
	memcpy(buffer,newbuffer,len);
	length = len;
}

int CByteBuffer::GetLength()
{
	return length;
}

void CByteBuffer::AppendString0(char *string, BOOL isCString, CString encoding)
{
	if(string != NULL && strlen(string)>0){
		int len = strlen(string);
		BYTE * strToAppend = new BYTE [len] ;
		memset((void*)strToAppend,0,len);
		memcpy((void*)strToAppend,(void*)string,len);
		
		AppendBytes0(strToAppend,len);
		delete strToAppend;
		//Chua xu li Encoding
	}//string != NULL
    
  	if(isCString ){
		AppendBytes0(zero,1);			
	}
    
}

void CByteBuffer::AppendBytes(CByteBuffer buffer, int count)
{
	if(buffer.GetLength()  < count ) return;
    try{
        BYTE* pData = buffer.ReadBytes( count);
	    AppendBytes0(pData,count);
        delete pData; pData = NULL;
    }catch(CNotEnoughDataInByteBufferException *pEx){
        pEx->Delete();
    }
	
}

/*
*	Clone the buffer
*/
BYTE * CByteBuffer::GetBuffer()
{
    if ( length  == 0 ) return NULL;
	BYTE * data = new BYTE[length];
	memset(data,0,length);
	memcpy(data,buffer,length);
	return data;
}

void CByteBuffer::AppendBytes(BYTE *byteArray, int count)
{
	AppendBytes0(byteArray,count);
}

void CByteBuffer::AppendByte(BYTE byte)
{
    BYTE* byteBuf = new BYTE[SIZE_BYTE];
    byteBuf[0] = byte;
    AppendBytes0(byteBuf, SIZE_BYTE);
	delete byteBuf;
}

void CByteBuffer::AppendCString(char *string)
{
	AppendString0(string,TRUE,"ASCII");
}

void CByteBuffer::AppendInt(int data){
	BYTE* intBuf = new BYTE[SIZE_INT];
    intBuf[3] = (BYTE)(data & 0xff);
    intBuf[2] = (BYTE)((data >> 8) & 0xff);
    intBuf[1] = (BYTE)((data >> 16) & 0xff);
    intBuf[0] = (BYTE)((data >> 24) & 0xff);
    AppendBytes0(intBuf, SIZE_INT);
	delete intBuf;
}

void CByteBuffer::AppendShort(short data)
{
		BYTE* shortBuf = new byte[SIZE_SHORT];
        shortBuf[1] = (BYTE)(data & 0xff);
        shortBuf[0] = (BYTE)((data >> 8) & 0xff);
        AppendBytes0(shortBuf, SIZE_SHORT);
        delete shortBuf;
}

void CByteBuffer::AppendString(char *string, const char *Encoding)
{
	AppendString0(string,FALSE,Encoding);
}

void CByteBuffer::AppendString(char * string)
{
	AppendString(string,"ASCII");
}

char * CByteBuffer::GetHexDump()
{
/*    char * dump = new char[length + 1];
	memset(dump,0,length  + 1);
    afxDump.HexDump(" ",GetBuffer(),length,10);// Dump cac mang buff, gom length byte moi hang 10 so*/
    return NULL;
}


BYTE* CByteBuffer::ReadBytes(int count)
{
	if(count > length ){	
		throw new CNotEnoughDataInByteBufferException();
		
	}

	BYTE * result = new BYTE [count];
	memcpy(result,buffer,count);
	return result;

}

int CByteBuffer::ReadInt()
{

    int result = 0;
    
    if (length >= SIZE_INT) {
        result |= buffer[0]&0xff;
        result <<= 8;
        result |= buffer[1]&0xff;
        result <<= 8;
        result |= buffer[2]&0xff;
        result <<= 8;
        result |= buffer[3]&0xff;
		
        return result;
	}else{
		
		throw new CNotEnoughDataInByteBufferException();
	}
}

BYTE* CByteBuffer::RemoveBytes(int count)
{
	try{
	
		BYTE* result = ReadBytes(count);
		RemoveBytes0(count);
		return result;
	}catch(CNotEnoughDataInByteBufferException* pEx){
		throw pEx;
	}
}

void CByteBuffer::RemoveBytes0(int count)
{
	
    int lefts = length - count;
    if (lefts >= 0) {
        BYTE * leftBuf = new BYTE[lefts];
        memcpy(leftBuf,&buffer[count],lefts);
        SetBuffer(leftBuf,lefts);
		delete leftBuf;
    } else {
        SetBuffer(NULL,0);
		throw new CNotEnoughDataInByteBufferException();
    }
}

char * CByteBuffer::RemoveCString()
{
	int zeroPos = 0;
	char * result = NULL;
    if( length == 0 ) throw new CNotEnoughDataInByteBufferException();
    while ((zeroPos<length) && (buffer[zeroPos] != 0)) {
        zeroPos++;
    }
    if (zeroPos < length) { // found terminating zero
 
        if (zeroPos > 0) {
            
                
				result = new char[zeroPos + 1];
				memset(result,0,zeroPos + 1);
				memcpy(result,buffer,zeroPos);
            
                // this can't happen as we use ASCII encoding
                // whatever is in the buffer it gets interpreted as ascii
            
        } else {
            result = NULL;
        }
        RemoveBytes0(zeroPos+1);
        return result;
    } else {
		throw new CTerminatingZeroNotFoundException();
        return NULL;// do khong tim thay diem ket thuc xau
    }

}

int CByteBuffer::RemoveInt()
{
	try{
		int result = ReadInt();
		RemoveBytes0(SIZE_INT);
		return result;
	}catch(CNotEnoughDataInByteBufferException* pEx){
		throw pEx;
	}
}

short CByteBuffer::RemoveShort()
{
	short result = 0;
	try{
	
		BYTE* resBuff = RemoveBytes(SIZE_SHORT);
		result |= resBuff[0]&0xff;
		result <<= 8;
		result |= resBuff[1]&0xff;

		delete resBuff;
		return result;
	}catch(CNotEnoughDataInByteBufferException* pEx){
		throw pEx;
	}
}

BYTE CByteBuffer::RemoveByte()
{
    byte result = 0;
	try{
		BYTE* resBuff = RemoveBytes(SIZE_BYTE);
		result = resBuff[0];
		delete resBuff;
		return result;
	}catch(CNotEnoughDataInByteBufferException* pEx){
		throw pEx;
	}
    
    
}

BYTE CByteBuffer::ReadByte() 
{
	if(length > 0)
		return buffer[0];
	else
		throw new CNotEnoughDataInByteBufferException();
	
	
}

BOOL CByteBuffer::IsEmpty()
{
	return (length == 0);
}

char * CByteBuffer::RemoveString(int count)
{
	if(count > length ) 
		throw new CNotEnoughDataInByteBufferException();
	try{
	
		char *result = new char[count] ;
		memcpy(result,buffer,count);
		RemoveBytes0(count);
		return result;
	}catch(CNotEnoughDataInByteBufferException* pEx){
		throw pEx;
	}
}

void CByteBuffer::ShowHex(BYTE NumsPerRows){
	if(length ==0)return;
	BYTE Rows = 0;
	if(length % NumsPerRows) Rows = length/NumsPerRows + 1;
	else	Rows = length/NumsPerRows ;
	

	for(int i = 0;i< Rows;i++){
		for(int j = 0;j< NumsPerRows;j++){
			if((i*NumsPerRows + j ) > length ) return;
			char* HexString =  GetHexString(buffer[i*NumsPerRows + j]);
			if(HexString){
				printf("%s ",HexString);
				delete HexString;
			}
		}
		printf("\n");
	}
}

char* CByteBuffer::GetHexString(BYTE byte)
{
	char * HexString = new char[3]	;
	BYTE val1 = 0, val2 = 0;
	val1	=	byte/16;
	val2	=	byte % 16;
	switch(val1){
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		HexString[0]	=	'0'	+ val1;
		break;
	case 10:
		HexString[0]	=	'a'	+ val1;
		break;
	case 11:
		HexString[0]	=	'b'	+ val1;
		break;
	case 12:
		HexString[0]	=	'c'	+ val1;
		break;
	case 13:
		HexString[0]	=	'd'	+ val1;
		break;
	case 14:
		HexString[0]	=	'e'	+ val1;
		break;
	case 15:
		HexString[0]	=	'f'	+ val1;
		break;
	}

	///////////////////////////////////////////////////
	switch(val2){
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		HexString[1]	=	'0'	+ val1;
		break;
	case 10:
		HexString[1]	=	'a'	+ val1;
		break;
	case 11:
		HexString[1]	=	'b'	+ val1;
		break;
	case 12:
		HexString[1]	=	'c'	+ val1;
		break;
	case 13:
		HexString[1]	=	'd'	+ val1;
		break;
	case 14:
		HexString[1]	=	'e'	+ val1;
		break;
	case 15:
		HexString[1]	=	'f'	+ val1;
		break;
	}

	HexString[2] = '\0';
	return HexString;
}

CByteBuffer* CByteBuffer::RemoveBuffer(int nSize)
{
    try{
        BYTE* data = RemoveBytes(nSize);
        CByteBuffer* pBuffer = new CByteBuffer(data,nSize);
        delete data;
        return pBuffer;
    }catch(CNotEnoughDataInByteBufferException *pEx){
        throw pEx;    
    }
}

void CByteBuffer::AppendBuffer(CByteBuffer *pBuffer)
{
    if(pBuffer != NULL){
        if(pBuffer->GetLength() > 0){
            BYTE * data = pBuffer->GetBuffer();
            AppendBytes(data,pBuffer->GetLength());
            delete data;
            data = NULL;        
        }
    }
}


CString CByteBuffer::DumpHex(int NumsPerRow)
{
	CByteBuffer * pBuffer = new CByteBuffer();
    if(length ==0)return "No data to dump";
	BYTE Rows = 0;
	if(length % NumsPerRow) Rows = length/NumsPerRow + 1;
	else	Rows = length/NumsPerRow ;
	
    pBuffer->AppendByte(13);
    pBuffer->AppendByte(10);

	for(int i = 0;i< Rows;i++){
		for(int j = 0;j< NumsPerRow;j++){
            if((i*NumsPerRow + j ) < length ){
			    //char* HexString =  GetHexString(buffer[i*NumsPerRow + j]);
                CString csHexString ;
                csHexString.Format("%.2x ",buffer[i*NumsPerRow + j]);
			    pBuffer->AppendString(csHexString.GetBuffer(0));
                csHexString.ReleaseBuffer();
            }
		}
		//printf("\n");
        pBuffer->AppendByte(13);
        pBuffer->AppendByte(10);
	}
    
    pBuffer->AppendByte(0);//Cuoi xau
    char * result  = pBuffer->RemoveCString();
    CString csResult = _T(result);
    delete result;
    delete pBuffer;
    return csResult;
}

CByteBuffer* CByteBuffer::ReadBuffer(int nSize)
{
    BYTE* pData = NULL;
    try{
        pData = ReadBytes(nSize);
        CByteBuffer * pResult = new CByteBuffer((BYTE*)pData,nSize);
        delete pData;
        return pResult;
    }catch(CNotEnoughDataInByteBufferException *pEx){
        throw pEx;
    }
}

void CByteBuffer::AppendCString(CString csString)
{
    

    char * c  = csString.GetBuffer(0);
    this->AppendCString(c);
    csString.ReleaseBuffer();
}

char * CByteBuffer::RemoveLine()
{
    int endLinePos = 0;
	char * result = NULL;
    if( length == 0 ) throw new CNotEnoughDataInByteBufferException();
    while ((endLinePos<length) && (buffer[endLinePos] != 0x0a)) {
        endLinePos++;
    }
    if (endLinePos < length) { // found terminating text line
 
        if (endLinePos > 0) {
            
				if(buffer[endLinePos - 1] == 0x0d){
					result = new char[endLinePos +10];
				    memset(result,0,endLinePos );
				    memcpy(result,buffer,endLinePos -1);// Khong copy thang 0x0d
                }
                else{
                    result = new char[endLinePos + 10];
				    memset(result,0,endLinePos  + 10);
                    memcpy(result,buffer,endLinePos);// File nay khong dung 0xd 0xa ma chi co 0xa
                }
                // this can't happen as we use ASCII encoding
                // whatever is in the buffer it gets interpreted as ascii
            
        } else {
            result = NULL;
        }
        RemoveBytes0(endLinePos+1 );//Remove index + 1 byte
        
        //Khong hieu sao lai khong lay duoc ki tu 0xd ma chi lay duoc 0x0a
        return result;
    } else {
		throw new CEndlineNotFoundException();
        //return NULL;// do khong tim thay diem ket thuc xau
    }

}

void CByteBuffer::AppendEndline()
{
    AppendByte(0x0d);
    AppendByte(0x0a);
}

void CByteBuffer::ResetBuffer()
{
    if(buffer){
        delete buffer;
        buffer = NULL;
        length = 0;
    }
}

CByteBuffer* CByteBuffer::Clone()
{
    int nSize = GetLength();
    if(nSize > 0){
        try{
            return ReadBuffer(nSize);
        }
        catch(CNotEnoughDataInByteBufferException*pEx){
            throw pEx;
        }
    }
    return NULL;
}

CString CByteBuffer::DumpWithNoFormat()
{
	if(length ==0) return "";
	CByteBuffer bytebuffer;
	CString csHexString ;
	for(int i = 0;i < length;i++){		
		csHexString.Format("%.2x",buffer[i]);
		bytebuffer.AppendString((LPTSTR)(LPCTSTR)csHexString);
		csHexString = "";
	}

	bytebuffer.AppendByte(0);//Cuoi xau
	char * result = bytebuffer.RemoveCString();
	CString csResult = _T(result);
	delete result;	

	return csResult;
}


BYTE CByteBuffer::ValueAt(int index)
{
	BYTE nValue = 0;
	if(index < length){
		nValue = buffer[index];
	}else{
		throw new CNotEnoughDataInByteBufferException();
	}
	
	return nValue;
}
