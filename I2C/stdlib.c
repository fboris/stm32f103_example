#include <stddef.h>
#include <stdarg.h>
#include <ctype.h>

#include "string.h"
//#include "stdlib.h"



char *itoa(int value, char *str)
{
	int base = 10;
	int divideNum = base;
	int i = 0;

	if (value < 0) {
		str[0] = '-';
		i++;
		value *= -1;
	}

	while (value / divideNum > 0) {
		divideNum *= base;
	}



	while (divideNum / base > 0) {
		divideNum /= base;
		str[i++] = value / divideNum + 48;
		value %= divideNum;
	}

	str[i] = '\0';
	return str;

}

#define LEN 25
void ltoa(char *buf, unsigned long i, int base)
{
	char *s;
	int rem;
	char rev[LEN + 1];

	if (i == 0)
		s = "0";
	else {
		rev[LEN] = 0;
		s = &rev[LEN];

		while (i) {
			rem = i % base;

			if (rem < 10)
				*--s = rem + '0';
			else if (base == 16)
				*--s = "abcdef"[rem - 10];

			i /= base;
		}
	}

	strcpy(buf, s);
}

typedef union {
	long        L;
	float        F;
}         LF_t;

char *ftoa(float f) //, int *status)
{
	long mantissa, int_part, frac_part;
	short exp2;
	LF_t x;
	char *p;
	static char outbuf[15];

	//*status = 0;
	if (f == 0.0) {
		outbuf[0] = '0';
		outbuf[1] = '.';
		outbuf[2] = '0';
		outbuf[3] = 0;
		return outbuf;
	}

	x.F = f;

	exp2 = (unsigned char)(x.L >> 23) - 127;
	mantissa = (x.L & 0xFFFFFF) | 0x800000;
	frac_part = 0;
	int_part = 0;

	if (exp2 >= 31) {
		//*status = _FTOA_TOO_LARGE;
		return 0;

	} else if (exp2 < -23) {
		//*status = _FTOA_TOO_SMALL;
		return 0;

	} else if (exp2 >= 23)
		int_part = mantissa << (exp2 - 23);
	else if (exp2 >= 0) {
		int_part = mantissa >> (23 - exp2);
		frac_part = (mantissa << (exp2 + 1)) & 0xFFFFFF;

	} else /* if (exp2 < 0) */
		frac_part = (mantissa & 0xFFFFFF) >> -(exp2 + 1);

	p = outbuf;

	if (x.L < 0)
		*p++ = '-';

	if (int_part == 0)
		*p++ = '0';
	else {
		ltoa(p, int_part, 10);

		while (*p)
			p++;
	}

	*p++ = '.';

	if (frac_part == 0)
		*p++ = '0';
	else {
		char m, max;

		max = sizeof(outbuf) - (p - outbuf) - 1;

		if (max > 7)
			max = 7;

		/* print BCD */
		for (m = 0; m < max; m++) {
			/* frac_part *= 10;        */
			frac_part = (frac_part << 3) + (frac_part << 1);

			*p++ = (frac_part >> 24) + '0';
			frac_part &= 0xFFFFFF;
		}

		/* delete ending zeroes */
		for (--p; p[0] == '0' && p[-1] != '.'; --p)
			;

		++p;
	}

	*p = 0;

	return outbuf;
}

double atof(const char *s)
{
	int sign = 1;
	int i = 0;
	for( i=0; isspace((unsigned char)s[i]); i++ );
	
	sign = (s[i] == '-')? -1:1;
	
	if( s[i] == '+' || s[i] == '-' )
		i++;
		
	double num = 0.0;
	double pow = 1.0;
	//integer
	for( ;isdigit((unsigned char)s[i]); i++ )
		num = num*10 + (s[i]-'0');
		
	for( i++; isdigit((unsigned char)s[i]); i++ )
	{
		num = num*10 + (s[i]-'0');
		pow *= 10;
	}
	
	return sign * (num/pow);
}

int sprintf(char *str, const char *format, ...)
{
	va_list para;
	va_start(para, format);
	int curr_pos = 0;
	char ch[] = {'0', '\0'};
	char integer[11];
	str[0] = '\0';

	while (format[curr_pos] != '\0') {
		if (format[curr_pos] != '%') {
			ch[0] = format[curr_pos];
			strcat(str, ch);

		} else {
			switch (format[++curr_pos]) {
			case 's':
				strcat(str, va_arg(para, char *));
				break;

			case 'c':
				ch[0] = (char)va_arg(para, int);
				strcat(str, ch);
				break;

			case 'i':
			case 'f':
				strcat(str, ftoa(va_arg(para, double)));
				break;

			case 'd':
				strcat(str, itoa(va_arg(para, int), integer));
				break;

			case 'u':
				strcat(str, itoa(va_arg(para, unsigned), integer));
				break;

			default:
				break;
			}
		}

		curr_pos++;
	}

	va_end(para);
	return strlen(str);
}
