

#include "utils.h"

/*
 * Function Name		: itoa
 * Function Description : This function convert integer to other value with desired base number.
 * Function Remarks		: None
 * Function Arguments	: N 				 integer value
 * 						  str				 string to store the value
 * 						  base				 radix
 * Function Return		: None
 * Function Example		: itoa (-100, buf, 10); //buf = -100
 */

char *ltoa(long N, char *str, int base)
{
      register int i = 2;
      long uarg;
      char *tail, *head = str, buf[BUFSIZE];

      if (36 < base || 2 > base)
            base = 10;                    /* can only use 0-9, A-Z        */
      tail = &buf[BUFSIZE - 1];           /* last character position      */
      *tail-- = '\0';

      if (10 == base && N < 0L)
      {
            *head++ = '-';
            uarg    = -N;
      }
      else  uarg = N;

      if (uarg)
      {
            for (i = 1; uarg; ++i)
            {
                  register ldiv_t r;

                  r       = ldiv(uarg, base);
                  *tail-- = (char)(r.rem + ((9L < r.rem) ?
                                  ('A' - 10L) : '0'));
                  uarg    = r.quot;
            }
      }
      else  *tail-- = '0';

      memcpy(head, ++tail, i);
      return str;
}
