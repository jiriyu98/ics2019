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
    char   *p;             
    char   *firstdig;      
    char   temp;           
    unsigned   digval;     
    p = buf;
    if(val <0)
    {
        *p++ = '-';
        val = (unsigned long)(-(long)val);
    }
    firstdig = p; 
    do{
        digval = (unsigned)(val % radix);
        val /= radix;
       
        if  (digval > 9)
            *p++ = (char)(digval - 10 + 'a'); 
        else
            *p++ = (char)(digval + '0');      
    }while(val > 0);
   
    *p-- = '\0 ';         
    do{
        temp = *p;
        *p = *firstdig;
        *firstdig = temp;
        --p;
        ++firstdig;        
    }while(firstdig < p);  
    return;
}
