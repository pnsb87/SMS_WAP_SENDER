// BitEncoder.h: interface for the CBitEncoder class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __BITENCODER__H
#define __BITENCODER__H

#include "ByteBuffer.h"


#ifndef u8
#define u8 unsigned char 
#endif

#ifndef u16
#define u16 unsigned short 
#endif

#ifndef u32
#define u32 unsigned long
#endif


class CBitEncoder  
{
public:
	CBitEncoder();
	virtual ~CBitEncoder();

	static BYTE GetBit(BYTE byte,BYTE Position);
	static BYTE SetBit(BYTE& byte,BYTE Position,char BitValue);
	
	static u8 MTU_USSD_str_to_def_alph(char* ascii_str, u8* da_octs, u8* da_len, u8 max_octs);
	static u16 str_to_def_alph(char* ascii_str, u8* da_octs, u8& da_len, u8 max_octs); 
	static int text2pdu(char* text, int length, char* pdu, char* udh); 
	static char* GetChars(BYTE* decoded,int count);
	static BYTE DecodeHexString(CString csHexString);
	static WORD DecodeHexStringWord(CString csHexString);
	static void RemoveSpaces(CString &csString);
	static  u8 bits_from_byte(u8 val, u8 bitoff, u8 nbit);
	static  u8 bit_from_byte(u8 val, u8 bitoff);
	static  void bits_to_byte(u8 *pdest, u8 val, u8 bitoff, u8 nbit);
	static  void bit_to_byte(u8 *pdest, u8 val, u8 bitoff);
	static  void bit_set(u8 *base, u16 index);
	static  void bit_clear(u8 *base, u16 index);
	static  int bit_test(u8 *base,u16 index);
	static unsigned char asctobin(char *hex);
	static void bintoasc(char *hex, unsigned char bin);
	static unsigned char hextobin(char c);
	static void bintohex(char *ptr, unsigned char bin);
	static int CStrToGsm11Enc7bit(CString str,LPBYTE pRecData,int len);
	static char * GetBinaryString(u8 byte);
	static int	  EncodeBCDString(u8* MsgData, char* string);	
	static char * DecodeBCDString(u8 * BCDBuffer,u8 length);
	static char*  EncodeBCDString(u8* msisdn, int len, int& out_len);
	static char * DecodeSS7String(u8 * BCDBuffer,u8 length);
	static char * DecodeSS7StringEven(u8 * BCDBuffer,u8 length);

	
	static void SetErrorType(BYTE *pErrorType, BYTE nValue);
	static BOOL CheckErrorType(BYTE *pErrorType, BYTE nValue);
	static BYTE GetErrorCode(BYTE *pErrorCode, BYTE *pErrorType, BYTE nValue);
	static BYTE GetErrorCode(BYTE *pErrorCode, BYTE nValue);

};



#endif 
