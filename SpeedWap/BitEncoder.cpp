/*--------------------------------------------------------------------------*
 | Copyright (C) 2009 Viettel Technologies., JSC. All rights reserved
 |--------------------------------------------------------------------------*
 | CBitEncoder.cpp
 |
 | Description: This class is a implementation of CBitEncoder, which provides
 |				manipulation methods with bit and byte.
 |
 | Modification Logs:
 | DATE          AUTHOR     DESCRIPTION
 | ------------------------------------------------
 | March 12, 2009   HungDM     Created
 *--------------------------------------------------------------------------*/

#include "stdafx.h"
#include "BitEncoder.h"
#include <string>
//#include <stdint.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static u8 ascii_to_default_alphabet[0x80]= {
	//0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f 
	//0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 32, 10, 0 , 13, 0 , 0 , 0 , 
	'@' , 0xa3 , '$' , 0xa5 , 0xe8 , 0xe9 , 0xf9 , 0xec , 0xf2 , 32, 10, 0 , 13, 0 , 0 , 0 , // 0x00 
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 32, 0 , 0 , 0 , 0 , // 0x10 
		' ','!','"','#', 2 ,'%','&', 39,'(',')','*','+',',','-','.','/', // 0x20 
		'0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?', // 0x30 
		0 ,'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O', // 0x40 
		'P','Q','R','S','T','U','V','W','X','Y','Z', 0xc4 , 0xd6 , 0xd1 , 0xa7 , 0xdc , // 0x50 
		0 ,'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o', // 0x60 
		'p','q','r','s','t','u','v','w','x','y','z', 0xe4 , 0xf6 , 0xf1 , 0xfc , 0xe0   // 0x70 
};
#define ASCII2DEF(ascii)   (ascii_to_default_alphabet[ascii])

int CBitEncoder::text2pdu(char* text, int length, char* pdu, char* udh)
{
    char tmp[500];
    char octett[10];
    int pdubitposition;
    int pdubyteposition = 0;
    int character;
    int bit;
    int pdubitnr;
    int counted_characters = 0;
    int udh_size_octets;   // size of the udh in octets, should equal to the first byte + 1
    int udh_size_septets;  // size of udh in septets (7bit text characters)
    int fillbits;          // number of filler bits between udh and ud.
    int counter;
	int max_pdu = 160;
    pdu[0]=0;
    // Check the udh
    if (udh)
    {
        udh_size_octets = (strlen(udh) + 2) / 3;
        udh_size_septets = ((udh_size_octets) * 8 + 6) / 7;
        fillbits = 7 - (udh_size_octets % 7);
        if (fillbits == 7)
            fillbits = 0;

        // copy udh to pdu, skipping the space characters
        for (counter = 0; counter < udh_size_octets; counter++)
        {
            pdu[counter * 2] = udh[counter * 3];
            pdu[counter * 2 + 1] = udh[counter * 3 + 1];
        }
        pdu[counter * 2] = 0;
    } 
    else
    {
        udh_size_octets = 0;
        udh_size_septets = 0; 
        fillbits = 0;
    }
    // limit size of the message to maximum allowed size
    if (length > max_pdu - udh_size_septets)
    length = max_pdu - udh_size_septets;
    //clear the tmp buffer
    for (character = 0; (size_t)character < sizeof(tmp); character++)
        tmp[character] = 0;
    // Convert 8bit text stream to 7bit stream
    for (character = 0; character < length; character++)
    {
        counted_characters++;
        for (bit = 0; bit < 7; bit++)
        {
            pdubitnr= 7 * character + bit + fillbits;
            pdubyteposition = pdubitnr / 8;
            pdubitposition = pdubitnr % 8;
            if (text[character] & (1 << bit))
                tmp[pdubyteposition] = tmp[pdubyteposition] | (1 << pdubitposition);
            else
                tmp[pdubyteposition] = tmp[pdubyteposition] & ~(1 << pdubitposition);
        }
    }
    tmp[pdubyteposition + 1] = 0;
    // convert 7bit stream to hex-dump
    for (character = 0; character <= pdubyteposition; character++)
    {
        sprintf(octett, "%02X", (unsigned char)tmp[character]);
        strcat(pdu, octett);
    }
    return counted_characters + udh_size_septets;
}


CBitEncoder::CBitEncoder()
{

}


CBitEncoder::~CBitEncoder()
{

}


//Kiem tra xem bit thu postion trong BYTE co dat khong 
BYTE CBitEncoder::GetBit(BYTE number,BYTE Position)
{
   BYTE tempNumber = (BYTE)number;  
   BYTE mask = (BYTE)1;
   mask = (BYTE)(mask << Position);
   if((mask & tempNumber)==mask) return (BYTE)1;
   else return ( BYTE) 0;
}


BYTE CBitEncoder::SetBit(BYTE& number,BYTE Position,char BitValue)
{    
    BYTE NumberFromPosition = (BYTE)(1 <<Position);
    if(BitValue > 0)
	{
    //Bit Value ==1
		if((number & NumberFromPosition)==NumberFromPosition){
      ///Can dat bit  = 1
      // Nhung bit do da dat roi =>[ Khong phai lam gi ca  ]
		}else{
     //Bit chua dat => Hung dat vao cho dung
			number+= NumberFromPosition;//cong dung gia tri can dien bit do
		}
	}
    else{//BitValue = 0
		 if((number & NumberFromPosition)==NumberFromPosition){
      ///Can dat bit  = 0
      // Nhung bit do da dat roi =>[ tru di gia tri do ]
			number -=NumberFromPosition;
		}else{
		//Bit chua dat, can dat la 0 = > [ Khong phai la gi ca]
		}// Kiem tra bit do dat dat chua
	}//BitValue

    return (BYTE)number;
}



char* CBitEncoder::GetChars(BYTE* decoded,int count)
{
	char* result = new char[count + 1];

	memset(result,0,count + 1);
	for(int i = 0;i< count ;i ++)
		result[i] = decoded[i];
	return result;
}

////////////////////////////////////////////////////////
void CBitEncoder::RemoveSpaces(CString &csString){
    int post1 = 0,post2 = 0;
    CString csResult = "";

    post1 = csString.Find(" ",post2);
    while(post1 > 0){
        csResult += csString.Mid(post2,post1-post2);
        post2 = post1 + 1;
        post1 = csString.Find(" ",post2);
    }
    //Lay phan cuoi cung
    CString csRight = csString.Right(csString.GetLength() - post2);
    csResult += csRight;
    csString = csResult;
}


BYTE CBitEncoder::DecodeHexString(CString csHexString)
{
    if(csHexString == "") return 0;
    if( csHexString.GetLength() == 1){
        csHexString = "0" + csHexString;
    }
    BYTE nLow = 0, nHigh = 0;

    CString csLow = "",csHigh = "";
    csLow = csHexString.Right(1);
    csHigh = csHexString.Left(1);
    
    char* temp = csLow.GetBuffer(0);
    if(temp[0] >= '0' && temp[0] <= '9')
        nLow = temp[0] - '0';
    if(temp[0] >= 'a' && temp[0] <= 'z')
        nLow = temp[0] - 'a'+ 10;
    if(temp[0] >= 'A' && temp[0] <= 'Z')
        nLow = temp[0] - 'A' + 10;
    csLow.ReleaseBuffer();

    //Tinh not byte cao 
    temp = csHigh.GetBuffer(0);
    if(temp[0] >= '0' && temp[0] <= '9')
        nHigh = temp[0] - '0';
    if(temp[0] >= 'a' && temp[0] <= 'z')
        nHigh = temp[0] - 'a' + 10;
    if(temp[0] >= 'A' && temp[0] <= 'Z')
        nHigh = temp[0] - 'A' + 10;
    csHigh.ReleaseBuffer();

    //return (nHigh <<4)& 0xf0 + (nLow ) & 0x0f;
    nHigh <<= 4;
    return nHigh + nLow;
}

WORD CBitEncoder::DecodeHexStringWord(CString csHexString)
{
    if(csHexString == "") return 0;
    if(csHexString.GetLength() < 3) csHexString = "0" + csHexString;
    
    CString csLow = "", csHigh = "";
    BYTE nLow = 0 , nHigh = 0;
    csHigh = csHexString.Left(2);
    csLow  = csHexString.Right(2);
    nLow = DecodeHexString(csLow);
    nHigh = DecodeHexString(csHigh);
    WORD result = 0;
    result = nHigh<<8;
    result |= nLow;
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////     UTILITY FUNCTION      ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Function to extract a number of adjacent bits
 * from a given offset in a byte:*/
u8 CBitEncoder::bits_from_byte(u8 val,u8  bitoff,u8 nbit)
  //u8    val;            /* value from which data is extracted */
  //u8    bitoff;         /* offset of LSB to be extracted (0..7) */
  //u8    nbit;           /* number of bits to extract (1..8) */
{
  return( (val>>bitoff) & (0xff>>(8-nbit)) );
}

/** Function to extract a single bit
 * from a given offset in a byte:*/
u8   CBitEncoder::bit_from_byte(u8 val, u8 bitoff)
  //u8    val;            /* value from which bit is extracted */
  //u8    bitoff;         /* offset of bit to be extracted (0..7) */
{
  if (val & (0x01 << bitoff))
    return(1);
  else
    return(0);
}

/** Function to insert a number of bits into
 * a byte at a given offset without changing
 * the remaining bits in the byte.*/
void CBitEncoder::bits_to_byte(u8 *pdest,u8 val,u8 bitoff,u8 nbit)
  //u8    *pdest;         /* pointer to byte to be modified */
  //u8    val;            /* value to be inserted (nbit LSB's used) */
  //u8    bitoff;         /* offset at which to insert LSB (0..7) */
  //u8    nbit;           /* number of bits to insert (1..8) */
{
  u8	mask;

  mask = (u8)(0xff >> (8-nbit));
  *pdest &= ~(mask << bitoff);
  *pdest |= ((val & mask) << bitoff);
}

/** Function to insert a single bit into
 * a byte at a given offset without changing
 * the remaining bits in the byte.*/
void CBitEncoder::bit_to_byte(u8 *pdest, u8 val, u8 bitoff)
  //u8    *pdest;         /* pointer to byte to be modified */
  //u8    val;            /* value to be inserted (LSB used) */
  //u8  bitoff;         /* offset at which to bit (0..7) */
{
  if (val & 0x01)
    *pdest |= (0x01 << bitoff);
  else
    *pdest &= ~(0x01 << bitoff);
}

/** The following functions allow individual bits in an
 * array of bytes to be set, cleared or tested. A pointer
 * to the first byte in the array is passed to the functions
 * along with the bit offset "index" from the start of the
 * array.
 *
 * index = 0 corresponds to the LSB of the first byte,
 * index = 7 corresponds to the MSB of the first byte,
 * index = 8 corresponds to the LSB of the second byte
 * and so on .......**/

void CBitEncoder::bit_set(u8 *base, u16 index)
  //u8    *base;          /* pointer to first entry in byte array */
  //u16   index;          /* offset in bits into array */
{
  *(base + (index >> 3)) |= (0x01 << (index & 0x07));
}

void CBitEncoder::bit_clear(u8 *base, u16 index)
  //u8    *base;          /* pointer to first entry in byte array */
  //u16   index;          /* offset in bits into array */
{
  *(base + (index >> 3)) &= ~(0x01 << (index & 0x07));
}

int CBitEncoder::bit_test(u8 *base, u16 index)
  //u8    *base;          /* pointer to first entry in byte array */
  //u16   index;          /* offset in bits into array */
{
  if ( (*(base + (index >> 3))) & (0x01 << (index & 0x07)) )
    return(1);
  else
    return(0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/** convert ascii hex character to binary - any character other than
 * 0-9, A-F or a-f has value 0*/
unsigned char CBitEncoder::hextobin(char c)
  //char c;
{
	if ((c >= '0') && (c <= '9'))
		c -= '0';
	else if ((c >= 'A') && (c <= 'F'))
		c -= ('A'- 10);
	else if ((c >= 'a') && (c <= 'f'))
		c -= ('a'- 10);
	else
		c = 0;
	return(c);
}

/* convert ascii hex pair to binary*/
unsigned char CBitEncoder::asctobin(char *hex)
  //char *hex;
{
  return((hextobin(*hex)<<4)+hextobin(*(hex+1)));
}

void CBitEncoder::bintohex(char *ptr,unsigned char bin)
  //char *ptr;
  //unsigned char bin;
{
  *ptr = (char)((bin < 10) ? '0'+bin : 'a'-10+bin);
}

void CBitEncoder::bintoasc(char *hex,unsigned char bin)
  //char *hex;
  //unsigned char bin;
{
  unsigned char lsd, msd;

  lsd = bin%16;
  msd = bin/16;
  bintohex(hex, msd);
  bintohex(hex+1, lsd);
}

/////////////////////////////////////////		Decode parameter		///////////////////
char* CBitEncoder::GetBinaryString(u8 byte)
{
	char * result = new char[9];
	memset(result , 0 ,9);
	for(u16 i = 0;i<8;i++)
		result[7-i]	=	'0' + bit_test(&byte,i);
	return result;
}
static bool strContaionNotNumber(CString str)
{
	int len = str.GetLength();
	char c;
	for(int i=0; i<len; i++)
	{
		c = str.GetAt(i);
		if( (c > '9') || (c < '0') )
			return true;
	}
	return false;
}
int CBitEncoder::CStrToGsm11Enc7bit(CString str,LPBYTE pRecData, int len)
{
	int strlen = str.GetLength();
	unsigned char *src = (unsigned char *)str.LockBuffer();
	unsigned char *dst = pRecData;

	unsigned char next7bit;
	unsigned char shift_buf;
	char shift_n;
	int i;
	unsigned char tmp;

	shift_buf = *src++;
	i=1;
	do{
		if( (i > strlen) || ((dst-pRecData) >= len) )
			break;
		shift_n = (i&7);
		next7bit = *src++;
		tmp = next7bit<<(8-shift_n);
		shift_buf |= tmp;
		*dst++ = shift_buf;
		i++;
		if(shift_n == 7)
		{
			shift_buf = *src++;
			i++;
		}else
			shift_buf = (next7bit >> shift_n);
	}while(1);
	str.UnlockBuffer();
	return (dst - pRecData);
}
static inline UCHAR Ascii2Bcd(UCHAR ch)
{
	if(ch > '0' && ch < '9')
		ch -= '0';
	else if(ch == '*')
		ch = 10;
	else if(ch == '#')
		ch = 11;
	return ch;
}

char*  CBitEncoder::EncodeBCDString(u8* msisdn, int len, int& out_len)
{
	char* result = new char[len/2 + 1];
	memset(result, 0, len/2 + 1);
	int index = 0;
	for(int i = 0; i < len; i+=2)
	{
			UCHAR chL = Ascii2Bcd(msisdn[i]);
			UCHAR chH;
			if(i+1 < len)
				chH = Ascii2Bcd(msisdn[i+1]);
			else
				chH = 0x0f;

			result[index++] = ((chH & 0xf) << 4) | (chL&0xf);
	}
	out_len = index;
	return result;
}

char * CBitEncoder::DecodeBCDString(u8 * BCDBuffer,u8 length)
{
	char * result = new char[length * 2 + 1];
	memset(result,0,length * 2 + 1);
	int index  = 0;
	
	for(int i = 0;i<length;i++)
	{
		BYTE Char = 0,NextChar = 0;
		Char	=	BCDBuffer[i] & 0x0f;//Lay 4 bit thap
		NextChar=	BCDBuffer[i] >> 4;//Lay 4 bit cao
		result[index ++]	=	(Char < 10)?(Char + '0'):(Char + 'a' - 10);
		if(NextChar == 0xf	)
			result[index++]	= 0;
		else
			result[index++]	=(NextChar < 10)?(NextChar + '0'):(NextChar + 'a' - 10);
	}
	return result;
}

char * CBitEncoder::DecodeSS7String(u8 * BCDBuffer,u8 length)
{
	char * result = new char[length * 2 + 1];
	memset(result,0,length * 2 + 1);
	int index  = 0;
	for(int i = 0;i<length;i++)
	{
		BYTE Char = 0,NextChar = 0;
		NextChar	=	BCDBuffer[i] & 0x0f;//Lay so sau
		Char = BCDBuffer[i] & 0xf0;//Lay so truoc
		Char = Char >>4;
		result[index ++]	=	(Char < 10)?(Char + '0'):(Char + 'a' - 10);
		if(i==(length-1) &&NextChar == 0x0)
			result[index++] = 0;
		else
			result[index++]	=(NextChar < 10)?(NextChar + '0'):(NextChar + 'a' - 10);
	}
	return result;
}


char * CBitEncoder::DecodeSS7StringEven(u8 * BCDBuffer,u8 length)
{
	char * result = new char[length * 2 + 1];
	memset(result,0,length * 2 + 1);
	int index  = 0;
	for(int i = 0;i<length;i++)
	{
		BYTE Char = 0,NextChar = 0;
		NextChar	=	BCDBuffer[i] & 0x0f;//Lay so sau
		Char = BCDBuffer[i] & 0xf0;//Lay so truoc
		Char = Char >>4;
		result[index ++]	=	(Char < 10)?(Char + '0'):(Char + 'a' - 10);
		result[index++]	=(NextChar < 10)?(NextChar + '0'):(NextChar + 'a' - 10);
	}
	return result;
}

void CBitEncoder::SetErrorType(BYTE *pErrorType, BYTE nValue)
{
	bit_set(pErrorType, nValue);
}

BOOL CBitEncoder::CheckErrorType(BYTE *pErrorType, BYTE nValue)
{
	return bit_test(pErrorType, nValue);
}
	
BYTE CBitEncoder::GetErrorCode(BYTE *pErrorCode, BYTE *pErrorType, BYTE nValue) 
{
	if (bit_test(pErrorType, nValue))
		return pErrorCode[nValue];
	else 
		return 0;
}

BYTE CBitEncoder::GetErrorCode(BYTE *pErrorCode, BYTE nValue) 
{
	return pErrorCode[nValue];
}

