#include <stdio.h>
#include <string.h>
#include <stdlib.h>

long addr5; //int
long addr6; //int

long foo(); //int
void point_at(void *p);
void foo1();
void foo2();

int main (int argc, char** argv){
    int addr2;
    int addr3;
    char* yos="ree";
    int * addr4 = (int*)(malloc(50));
    printf("- &addr2: %p\n",&addr2); //address of addr2 (on stack) - local area variable
    printf("- &addr3: %p\n",&addr3); //address of addr3 (on stack) - local area variable
    printf("- foo: %p\n",foo); //address of foo (on stack) - global area function
    printf("- &addr5: %p\n",&addr5); //address of addr5 (on stack) - global area variable
    
	point_at(&addr5); //details in the function
	
    printf("- &addr6: %p\n",&addr6); //address of addr6 (on stack) - global area variable
    printf("- yos: %p\n",yos); //address of a yos pointer to the first char r in the char array ree on the heap (on heap)
    printf("- addr4: %p\n",addr4); //address of addr4 pointer to a new int with size of 50
    printf("- &addr4: %p\n",&addr4);//address of addr4 garbage value of the pointer
    
    printf("- &foo1: %p\n" ,&foo1); //address of foo1 - global area function (on stack)
    printf("- &foo1: %p\n" ,&foo2); //address of foo2 - global area function (on stack)
    printf("- &foo2 - &foo1: %ld\n" ,&foo2 - &foo1); //distance between the address of foo2 and foo1 in decimal
    return 0;
}

long foo(){ //int
    return -1;
}

void point_at(void *p){ //p = addr5
    long local; //int
	static int addr0 = 2;
    static int addr1;


    long dist1 = (size_t)&addr6 - (size_t)p;
    long dist2 = (size_t)&local - (size_t)p;
    long dist3 = (size_t)&foo - (size_t)p;
    
    printf("dist1: (size_t)&addr6 - (size_t)p: %ld\n",dist1); //distance between the address of addr6 and addr5(p) in decimal
    printf("dist2: (size_t)&local - (size_t)p: %ld\n",dist2); //distance between the address of local and addr5(p) in decimal
    printf("dist3: (size_t)&foo - (size_t)p:  %ld\n",dist3); //distance between the address of foo and addr5(p) in decimal
	//long enough for dist (address difference) variables
	printf("- addr0: %p\n", & addr0); //address of addr0 local variable (on stack)
    printf("- addr1: %p\n",&addr1); //address of addr1 local variable (on stack)
}

void foo1 (){    
    printf("foo1\n"); 
}

void foo2 (){    
    printf("foo2\n");    
}
