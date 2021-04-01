/*Author: Balbeer Yadav   	Roll: 1801CS13 (IIT Patna)
I hereby declare that I, Balbeer Yadav from IIT Patna, have authored this source code independently
that I have not used other than the declared sources/resources, and that I have explicitly marked
all material which has been quoted either literally or by content from the used sources.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_OF_OPCODES 18

int A,B;
int PC,SP;
int *Data_Segment;

void ldc(const void* value){
	// Load accumulator with the value specified 
	B = A;
	A = *(int*)value;
}

void adc(const void* value){
	// Add to the accumulator
	A += *(int*)value;
}

void ldl(const void* offset){
	// Load local 
	B = A;
	A = Data_Segment[SP + *(int*)offset];
}

void stl(const void* offset){
	// Store local 
	Data_Segment[SP + *(int*)offset] = A;
	A = B;
}

void ldnl(const void* offset){
	// Load non-local
	A = Data_Segment[A + *(int*)offset];
}

void stnl(const void* offset){
	// Store non-local 
	Data_Segment[A + *(int*)offset] = B;
}

void add(const void* off){
	// Addition 
	A = B+A;
}

void sub(const void* off){
	// Subtraction 
	A = B-A;
}

void shl(const void* off){
	// Shift left 
	A = B<<A;
}

void shr(const void* off){
	// Shift right 
	A = B>>A;
}

void adj(const void* value){
	// Adjust SP 
	SP = SP + *(int*)value;
}

void a2sp(const void* off){
	// Transfer A to SP; 
	SP = A;
	A = B;
}

void sp2a(const void* off){
	// Transfer SP to A 
	B = A;
	A = SP;
}

void call(const void* offset){
	// Call procedure 
	B = A;
	A = PC;
	PC = PC + *(int*)offset;
}

void ret(const void* offset){
	// Return from procedure
	PC = A;
	A = B;
}

void brz(const void* offset){
	// If accumulator is zero, branch to
	// specified offset 
	if(A == 0)
		PC = PC + *(int*)offset;
}

void brlz(const void* offset){
	// If accumulator is less than zero,
	// branch to specified offset 
	if(A < 0)
		PC = PC + *(int*)offset;
}

void br(const void* offset){
	// Branch to specified offset 
	PC = PC + *(int*)offset;
}

void (*asm_func[])(const void*) = {ldc,adc,ldl,stl,ldnl,stnl,add,sub,shl,shr,adj,a2sp,sp2a,call,ret,brz,brlz,br};
char mnemonics[][7] = {"ldc","adc","ldl","stl","ldnl","stnl","add","sub","shl","shr","adj","a2sp","sp2a","call","return","brz","brlz","br","HALT"};

#define TRACE if(trace)fprintf(logfile,"PC=%08X, SP=%08X, A=%08X, B=%08X",PC,SP,A,B)

int main(int argc,char* argv[]){
	PC = SP = A = B = 0;
	if(argc < 3){
		printf("Pass the object file and Use flags--\n");
		printf("-t trace registers each instruction\n");
		printf("-b memory dump before execution\n");
		printf("-a memory dump after execution\n");
		return 0;
	}
	if(argc != 3){
		printf("Use format= ./emu -[flag(s)] [object-file]\n Eg: ./emu -ab test.o for before and after dump\n");
		return 0;
	}
	int fname_len = strlen(argv[2]);
	if(fname_len <= 2 || strcmp(argv[2]+fname_len-2,".o")){
		printf("Pass only .o object files\n");
		return 0;
	}
	int trace = 0,before = 0,after = 0;
	for(int i=1;i<strlen(argv[1]);++i){
		if('t' == argv[1][i])
			trace = 1;
		if('b' == argv[1][i])
			before = 1;
		if('a' == argv[1][i])
			after = 1;
	}

    char log_file[50];
    strcpy(log_file,argv[2]);
    log_file[fname_len-2]='\0';
    strcat(log_file,".txt");
    FILE* logfile = fopen(log_file,"w");

	int size;
	FILE *fptr = fopen(argv[2],"rb");

	fread(&size,sizeof(int),1,fptr);

	Data_Segment = (int*)malloc(size*sizeof(int));
 
	fread(Data_Segment,sizeof(int),size,fptr);

	if(before){
		fprintf(logfile,"\nData Segment before execution: ");
		for(int i=0;i<size;++i){
			if(i%4==0)
				fprintf(logfile,"\n%08X",i);
			fprintf(logfile," %08X",Data_Segment[i]);
		}
		fprintf(logfile,"\n");
	}
	fprintf(logfile,"\n");
	int counter = 0 , csize = 0;
    int Code[10000];
    int inst;
    while(fread(&inst,sizeof(int),1,fptr))
    {
        Code[csize++] = inst;
    }
    //fprintf(logfile,"no of instructions read = %d\n",csize);
	for(;PC < csize;++PC,++counter){
		TRACE;
		int op = (0xFF & Code[PC]);
		if(trace)fprintf(logfile," %s",mnemonics[op]);
		
		int val = Code[PC]>>8;
		if((1<<31)&Code[PC])
			val = (0xFF<<24)|val;

		if(trace){
			if(op < 6 || (op == 10 || op == 13) || (op >= 15 && op <= 17))
				fprintf(logfile," %08X\n",val);
			else
				fprintf(logfile,"\n");
		}
		if(op == 18)
			break;
		(asm_func[op])(&val);
	}
	fprintf(logfile,"\n%d instruction(s) executed\n",counter);

    fclose(fptr);

	if(after){
		fprintf(logfile,"\nData Segment after execution: ");
		for(int i=0;i<size;++i){
			if(i%4==0)
				fprintf(logfile,"\n%08X",i);
			fprintf(logfile," %08X",Data_Segment[i]);
		}
		fprintf(logfile,"\n");
	}
}