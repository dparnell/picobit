#ifndef _BITS_H_
#define _BITS_H_

typedef volatile unsigned int   reg;
typedef volatile unsigned short reg16;

#define CONFIG_AREA(type, name, addr) \
			static struct type* const name = (struct type*) addr;

#define BIT(n)		(1 << (n))
#define MASK(nbits)	((1 << (nbits)) - 1)

#define BITS(x, s, e) (((x) & MASK(e - s + 1)) << s)
#define R_BITS(v, s, e) (((v) & MASK(e - s + 1)) >> s)
#define W_BITS(v, x, s, e) (v) = ((v) & ~(MASK(e - s + 1) << s)) | BITS(x, s, e)

#endif
