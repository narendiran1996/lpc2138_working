#include<LPC213x.h>
#define X_CPU 12000000L
#define C_CLK 12000000L // since multiplier in PLL0 is 1
#define P_CLK 3000000L // since vpd_div is made 000 - repressing C_CLK/4 (default)
void custom_delay()
{
	int i,j;
	for(i=0;i<1234;i++)
	{
		for(j=0;j<123;j++)
		{
		}
	}
}
int main()
{
	IODIR0 = IODIR0 | (1<<16);
	
	while(1)
	{
		IOSET0 = IOSET0 | (1<<16);
		custom_delay();
		IOCLR0 = IOCLR0 | (1<<16);
		custom_delay();
	}
	return 0;
}
