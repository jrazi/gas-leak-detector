
#include "utils.h"

int StartsWith(const char *a, const char *b)
{
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   return 0;
}

int EndsWith(const char *a, const char *b)
{
	 const char* a_end;
   int len_b = strlen(b);	
	 int len_a = strlen(a);
	 
	 if (len_a < len_b) return 0;
	  
	 a_end = &a[len_a - len_b];
	
   if(strncmp(a_end, b, strlen(b)) == 0) return 1;
   return 0;
}

