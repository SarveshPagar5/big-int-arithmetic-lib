#include <stdio.h>
#define MAX_LEN 310

typedef struct
{
    char num[MAX_LEN];
    int len;
    int sign;
}BigInt;

int strlen(const char *str)
{
    int length=0;
    while(str[length]!='\0')
    {
        length++;
    }
    return length;
}
int check_for_maxValue(BigInt* s)
{
    if(s->len<308)
    {
        return 1;
    }
    else if(s->len>308)
    {
        return 0;
    }

    char Max_val[308]="89884656743115795386465259539451236680898848947115328636715040578866337902750481566354238661203768010560056939935696678829394884407208311246423715319737062188883946712432742638151109800623047059726541476042502884419075341171231440736956555270413618581675255342293149119973622969239858152417678164812112068607";

    char Min_val[308]="89884656743115795386465259539451236680898848947115328636715040578866337902750481566354238661203768010560056939935696678829394884407208311246423715319737062188883946712432742638151109800623047059726541476042502884419075341171231440736956555270413618581675255342293149119973622969239858152417678164812112068608";

    int flag = 1;
    int flag2 = 1;   
    if(s->sign == -1)
    {
        for(int i=307;i>=0 && flag2;i--)
        {
            int a=s->num[i];
            int b=Min_val[307-i]-'0';
            if(a!=b)
            {
                if(a>b) 
                {
                    flag = 0;
                }
                flag2=0;
            }

        }
    }
    else
    {
        for(int i=307;i>=0 && flag2;i--){
            int a =s->num[i];
            int b = Max_val[307-i]-'0';
            if(a!=b)
            {
                if(a>b) flag=0;
                flag2=0;
            }
        }
    }
    return flag;
}

void init(BigInt *a)
{
    for(int i=0;i<MAX_LEN;i++)
    {
        a->num[i]=0;
    }
    a->len=0;
    a->sign=1;
}

int set(BigInt *a, char *s)
{
    init(a);
    int start=0;
                    
    //handle sign
    if(s[0]=='-')
    {
        a->sign=-1;
        start=1;
    }
    //skip initial zeros
    while(s[start]=='0')
    {
        start++;
    }

    // Convert string to digits
    int j=0;
    for (int i=strlen(s)-1;i>=start;i--)
    {
        if(s[i]<48 || s[i]>57)
        {
            printf("\nPLEASE ENTER A VALID NUMBER");
            return 0;
        }
        a->num[j++]=s[i]-'0';
    }

    a->len=j;
    if(check_for_maxValue(a)==0)
    {
        printf("\n\nTHE NUMBER PROVIDED IS OUT OF BOUNDS OVEFLOW MAY OCCUR \n\n");
        return 0;
    }
    return 1;
}

void print(BigInt *a)
{
    if (a->sign==-1)
    {
        printf("-");
    }
    // Skip leading zeros
    int i;
    for (i=a->len-1;i>=0 && a->num[i]==0;i--);
    
    if(i<0)
    {
        printf("0");
        return;
    }   
    for (;i>=0;i--)
    {
        printf("%d",a->num[i]);
    }
}

void abs_add(BigInt *a, BigInt *b, BigInt *result)
{
    init(result);
    
    int carry=0;
    int maxLen;
    maxLen=(a->len>b->len) ? a->len : b->len;
    
    for (int i=0;i<maxLen+1 || carry;i++)
    {
        int digitA=(i<a->len) ? a->num[i] : 0;
        int digitB=(i<b->len) ? b->num[i] : 0;
        
        int sum=digitA+digitB+carry;
        result->num[i]=sum % 10;
        carry=sum/10;
    }  
    result->len=maxLen+1;
    while(result->len>0 && result->num[result->len-1]==0) //this is to set length of result
    {
        result->len--;
    }
}

int abs_compare(BigInt *a, BigInt *b)
{
    if(a->len!=b->len)
    {
        return a->len-b->len;
    }
    for(int i=a->len-1;i>=0;i--)
    {
        if(a->num[i]!=b->num[i]) 
        {
            return a->num[i]-b->num[i];
        }
    }
    return 0;
}

void abs_sub(BigInt *a, BigInt *b, BigInt *result)
{
    init(result);
    
    // Ensure a is larger
    if (abs_compare(a, b)<0)
    {
        BigInt *temp=a;
        a=b;
        b=temp;
        result->sign=-1;
    }
    
    int borrow=0;
    for (int i=0; i<a->len;i++)
    {
        int digitA=a->num[i];
        int digitB=(i < b->len) ? b->num[i] : 0;
        
        int diff=digitA-digitB-borrow;
        if(diff<0)   //taking a carry
        {
            diff+=10;
            borrow=1;
        }
        else        // no need of carry
        {
            borrow=0;
        }
        
        result->num[i]=diff;
    }
    
    result->len=a->len;
    while (result->len>0 && result->num[result->len-1]==0)
    {
        result->len--;
    }
}

void abs_mul(BigInt *a, BigInt *b, BigInt *result)
{
    init(result);
    
    for (int i=0;i<a->len;i++) 
    {
        int carry=0;
        for (int j=0;j<b->len || carry;j++)
        {
            int digitB=(j<b->len) ? b->num[j] : 0;
            int prod=result->num[i+j]+a->num[i]*digitB+carry;
            result->num[i+j]=prod % 10;
            carry=prod/10;
        }
    }
    
    result->len=a->len+b->len;
    while(result->len>0 && result->num[result->len-1]==0)
    {
        result->len--;
    }
}

void add(BigInt *a, BigInt *b, BigInt *result)
{
    if(a->sign==b->sign)
    {
        abs_add(a,b,result);
        result->sign=a->sign;
    }
    else
    {
        if(abs_compare(a,b)>=0)
        {
            abs_sub(a,b,result);
            result->sign=a->sign;
        }
        else
        {
            abs_sub(b,a,result);
            result->sign=b->sign;
        }
    }
}

void sub(BigInt *a,BigInt *b,BigInt *result) 
{
    BigInt neg_b=*b;
    neg_b.sign *=-1;
    add(a,&neg_b,result);
}

void mul(BigInt *a, BigInt *b, BigInt *result) 
{
    abs_mul(a,b,result);
    result->sign=a->sign*b->sign;
}

int main() 
{
    BigInt a, b, result;
    char string1[310];
    printf("ENTER THE FIRST NUMBER : ");
    scanf("%s",string1);
    if(set(&a, string1)==0)
    {
        return 0;
    }
    printf("ENTER THE SECOND NUMBER : ");
    scanf("%s",string1);
    if(set(&b, string1)==0)
    {
        return 0;
    }

    printf("The Addition is : ");
    add(&a, &b, &result);
    if(check_for_maxValue(&result)==0)
    {
        printf("\n\nTHE RESULT IS OUT OF BOUNDS \n\n");
        return 0;
    }
    print(&result);
    printf("\n");

    printf("The Subtraction is : ");
    sub(&a, &b, &result);
    if(check_for_maxValue(&result)==0)
    {
        printf("\n\nTHE RESULT IS OUT OF BOUNDS \n\n");
        return 0;
    }
    print(&result);
    printf("\n");

    printf("The Multiplication is : ");
    mul(&a, &b, &result);
    if(check_for_maxValue(&result)==0)
    {
        printf("\n\nTHE RESULT IS OUT OF BOUNDS \n\n");
        return 0;
    }
    print(&result);
    printf("\n\n\n");
    return 0;
}