#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include "mem.h"



typedef struct{
	unsigned char set;
}Bit;

int checkIfContiguous(Bit *temp,int two_bit_count,int size);
void setBitsToUsed(Bit *temp,int two_bit_count,int size);
void display(Bit *temp);
int resetBits(int count);

Bit *head=NULL;
int regionsize=0;
int freeSpace=0;


int Mem_Init(int sizeOfRegion,int debug)
{
	if(head!=NULL || sizeOfRegion<=0)
	{
		return -1;
	}
	if(sizeOfRegion%getpagesize()!=0)
	{
		// printf("Aligning page  %d\n",getpagesize() );
		sizeOfRegion=sizeOfRegion+getpagesize() - (sizeOfRegion % getpagesize() );
	}
	int fd = open("/dev/zero", O_RDWR);
	void* ptr = mmap(NULL,sizeOfRegion, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (ptr == MAP_FAILED) { printf("Mapping failed\n"); return 1; }



	head=(Bit *)ptr;
	regionsize=sizeOfRegion;//-sizeof(head);
	int bitmap_size=((regionsize*2)/8);
	if(bitmap_size%8!=0)
	bitmap_size=bitmap_size+8 - (bitmap_size%8);

	freeSpace=regionsize-bitmap_size;
	// printf("freeSpace-bitmap is %d  %d\n",freeSpace,bitmap_size );

	return 0;
} //Mem_Init

void *Mem_Alloc(int size)
{
	int bitmap_size=((regionsize*2)/8);
	if(bitmap_size%8!=0)
	bitmap_size=bitmap_size+8 - (bitmap_size%8);

	if((size<1) || (head==NULL))
	{
		fprintf(stderr, "%d\n",E_BAD_ARGS);
		return NULL;
	}
	if(size>freeSpace)
	{
		// printf("size > fs %d %d\n",size,freeSpace );
		fprintf(stderr, "%d\n",E_NO_SPACE);
		return NULL;
	}

	//check which position is free and check for contiguous_chunk from that position
	int byte_count=0; //shd traverse less than freeSpace
	int offset=0; //inc byte when done reading 8 bit
	int contiguous_chunk=0;
	Bit *temp=head;

	while(byte_count<bitmap_size)
	{
		int two_bit_count=0; // 0 1 2 3 -> 4 *2 = 8 bits
		while(two_bit_count<4)
		{

			int bit_1 = (temp->set >>(7-2*two_bit_count)) & 1;
			int bit_2 = (temp->set >>(7-2*two_bit_count-1)) & 1;

			if(bit_1 ==0 && bit_2==0 )
			{
				contiguous_chunk=checkIfContiguous(temp,two_bit_count,size);

				if(contiguous_chunk==1)
				{
					setBitsToUsed(temp,two_bit_count,size);
					freeSpace-=size;
  				temp=head+bitmap_size;
         // 	printf("free size Available to allocate_1	 %d\n",freeSpace);
					return temp+offset;
				}
			}
			offset++;
			two_bit_count++;
		} //two_bit_count check

		temp++;
		byte_count++;
	}	// byte_count check
	// printf("free size Available to allocate_2	 %d\n",freeSpace);
	return NULL;
}

int checkIfContiguous(Bit *temp,int two_bit_count,int size)
{
	Bit *chunk_temp=temp;
	int i=1;
	int next_byte_flag=0;

	while(i<=size)
	{
		while(two_bit_count<4 && i<=size)
		{
			int bit_1 = (chunk_temp->set >>(7-2*two_bit_count)) & 1;
			int bit_2 = (chunk_temp->set >>(7-2*two_bit_count-1)) & 1;

			if( bit_1 !=0 || bit_2!=0)
			{
				return 0;
			}
			two_bit_count++;
			i++;
			if(two_bit_count==4)
			{

				next_byte_flag=1;
			}
		}

		if(next_byte_flag)
		{
			chunk_temp++;
			two_bit_count=0;
		}

		if(i==size)
		break;

		i++;
	}

	return 1;
}


void setBitsToUsed(Bit *temp,int two_bit_count,int size)
{
	Bit *chunk_temp=temp;
	int i=1;
	chunk_temp->set|=1<<(7-(2*two_bit_count));
	chunk_temp->set|=1<<((7-(2*two_bit_count)-1));

	two_bit_count++;
	i++;
	while(i<=size)
	{

		while(two_bit_count<4 && i<=size)
		{
			chunk_temp->set |=1 <<(7-(2*two_bit_count));
			chunk_temp->set |=0 <<(7-(2*two_bit_count)-1);
			two_bit_count++;
			i++;
		}
		if(two_bit_count==4)
		{
			chunk_temp++;
			two_bit_count=0;
		}
		else
		{
			i++;
		}
	}

}

void display(Bit *temp)
{
	int count=0;
	int two_bit_count=0;

	while(count<20)
	{
		if(two_bit_count==4)
		{
			temp++;
			two_bit_count=0;
		}

		printf(" %d%d \t",( (temp->set >>( 7-(2*two_bit_count)) ) &1)
		,
		((temp->set >>(7-(2*two_bit_count)-1)) &1) );

		two_bit_count++;
		count++;
	}
	// printf("\n");
}

int Mem_Free(void *ptr)
{

	Bit *temp=head+((regionsize*2)/8);
	int count=1;

	while(temp!=ptr)
	{
		temp++;
		count++;
	}
	if(count>regionsize)
	{

		return 1;
	}
	int rc=resetBits(count);
	if(rc!=0)
	{
		return 1;
	}
return 0;
}

int resetBits(int count)
{

	Bit *temp=head;
	int total_count=1;
	int two_bit_count=0;

	while(total_count<=count)
	{
		while(two_bit_count<4 && total_count<count)
		{
			two_bit_count++;
			total_count++;
			if(two_bit_count==4)
			{
				two_bit_count=0;
				temp++;
			}
		}

		if(total_count==count)
		{
			//reached free ptr start..
			int bit_1 = (temp->set >>(7-2*two_bit_count)) & 1;
			int bit_2 = (temp->set >>(7-(2*two_bit_count)-1) ) & 1;

			if(bit_1!=1 || bit_2!=1)
			{
				return 1; //header check . Returns if no header.
			}



			temp->set ^=(1 <<(7-(2*two_bit_count)));	//setting header to zero and incrment two_bit_count
			temp->set ^=(1 <<(7-(2*two_bit_count)-1));

			bit_1 = (temp->set >>(7-2*two_bit_count)) & 1;
			bit_2 = (temp->set >>(7-(2*two_bit_count)-1) ) & 1;

			freeSpace+=1;

			two_bit_count++;

			if(two_bit_count==4)
			{
				two_bit_count=0;
				temp++;

			}
			bit_1=(temp->set >>(7-2*two_bit_count)) & 1;
			bit_2=(temp->set >>(7-(2*two_bit_count)-1) ) & 1;

			while(bit_1==1 && bit_2==0)
			{
				freeSpace+=1;
				temp->set &=~(1 <<(7-(2*two_bit_count)));
				two_bit_count++;
				if(two_bit_count>3)
				{
					two_bit_count=0;
					temp++;
				}

				bit_1=(temp->set >>(7-2*two_bit_count)) & 1;
				bit_2=(temp->set >>(7-(2*two_bit_count)-1) ) & 1;
			}

			return 0;
		}

	}
	return 1;
}

void Mem_Dump()
{
	display(head);
}
