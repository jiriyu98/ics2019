static unsigned long int next = 1;

int rand(void) {
  // RAND_MAX assumed to be 32767
  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed) {
  next = seed;
}

void itoa(char *buf, int val, unsigned radix)
{

	char zm[37]="0123456789abcdefghijklmnopqrstuvwxyz";
	char aa[100]={0};
 
	int sum=val;
	char *cp=buf;
	int i=0;
 
	while(sum>0)
	{
		aa[i++]=zm[sum%radix];
		sum/=radix;
	}
 
	for(int j=i-1;j>=0;j--)
	{
		*cp++=aa[j];
	}
	*cp='\0';
	return;

}
