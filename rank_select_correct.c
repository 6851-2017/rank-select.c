#include <stdio.h>
#include <stdlib.h>

#define logK 3
#define K (1 << logK)
#define N (2 << K)
#define logN (K+1)
#define logSqN (logN*logN)
#define loglogN (logK+1)

static int table [K][logK];


void fill_table(){
	for (int i=0; i<K; i++){
		for (int j=0; j < logK; j++){
			int value = i >> j;
			int count = __builtin_popcount(value);
			table[i][logK-j-1] =  count; 
		}
	}
}

// return how many one bits are left of this position 
int rank(int pos, int vector){
	return 0;
}

void print_bits(int *bit_string, int len) {
	for (int i = 0; i < len; i++) {
		for (int j = 31; j >= 0; j--) {
			printf("%d", (bit_string[i]>>j)&1);
		}
	}
	printf("\n");
}

int main(){
	int bitString [N/32] = {0};
	bitString[0] = (1<<20)+1;
	bitString[2] = (1<<20)+1;

	// bits in chunk -> logSqN
	// these chunks can be smaller
	int chunks [(N/logSqN)+1]; 
	int subchunk_size = 32;
	int chunk_size = logSqN;
	int subchunks[(N/logSqN)+1][32];
	//int subchunks[64][K];
	int count = 0;
	printf("chunk size = %d, subchunk size = %d\n", chunk_size, subchunk_size);

	// filling chunks
	for(int i=0; i<N; i+=logSqN){
		
		int j = i;
		if(j%32 !=0){
			int partial = bitString[j/32];
			partial = partial << (j%32);
			count += __builtin_popcount(partial);
			j += (32-(j%32));
		}

		while(j+32 <(i+logSqN) && (j<N)){
			count+=__builtin_popcount(bitString[j/32]);
			j+=32;
		}
		if(j<(i+logSqN) && (j<N)){
			int partial = bitString[j/32];
			partial = partial >> (32-((i+logSqN)%32));
			count += __builtin_popcount(partial);
			j+=((i+logSqN)%32);
		}
		chunks[i/logSqN] = count;
	}

	// subchunks
	for(int x=0; x<(N/logSqN)+1; x++){
		int subchunk_count = 0;
		printf("new chunk\n");
		for(int y=0; y<=logSqN/32; y+=1){

			int start_bit_pos = 32*((x*logSqN+y*32)/32)+((x*logSqN+y*32)%32);
			int end_bit_pos = start_bit_pos+32;
			if (end_bit_pos > (x+1)*logSqN) {
				end_bit_pos = (x+1)*logSqN;
			}

			int subchunk_start_index = start_bit_pos/32;
			int subchunk_end_index = end_bit_pos/32;

			int start = bitString[subchunk_start_index];
			unsigned int end = bitString[subchunk_end_index];

			int first_part = start << (subchunk_start_index%32);
			unsigned int second_part = end >> (32-(subchunk_end_index%32));
			
			//int first_part = bitString[((x*logSqN+y*32)/32)] << ((x*logSqN+y*32)%32);
			//int second_part = bitString[((x*logSqN+y*32)/32)+1] >> (32-((x*logSqN+y*32)%32));
			subchunk_count += __builtin_popcount(first_part);
			subchunk_count += __builtin_popcount(second_part);
			subchunks[x][y]=subchunk_count;
		}
	}
	print_bits(bitString, N/32);
	for(int x=0; x<(N/logSqN)+1; x++){
		printf("chunk size = %d\n", chunks[x]);
		for(int y=0; y<logSqN/32; y+=1){
			printf("subchunk size = %d\n",subchunks[x][y]);
		}
	}

}


