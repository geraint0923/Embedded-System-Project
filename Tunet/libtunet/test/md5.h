#ifndef _RPCENG_MD5_H
#define _RPCENG_MD5_H

#include <string.h>

typedef struct 
{
	unsigned int state[4];      
	unsigned int count[2];      
	unsigned char buffer[64];      
} MD5Context;

void MD5_Init(MD5Context * context);

void MD5_Update(MD5Context * context, unsigned char * buf, int len);

void MD5_Final(MD5Context * context, unsigned char digest[16]);


#endif /* end of _RPCENG_MD5_H */
