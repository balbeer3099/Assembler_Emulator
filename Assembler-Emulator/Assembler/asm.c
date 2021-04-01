// Name: Balbeer Yadav
// Rno: 1801CS13
// BTech CSE 2018-22, IIT Patna
// Declaration of Authorship
// This file is written solely by aforementioned for project under CS321/CS322

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

int data[1000];
int indx=0;

char* trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';
    //printf("trimmed string: %s\n",str);
  return str;
}

void lowerstr(char* ptr)
{
    int i=0;
    while(ptr[i]!='\0')
    {
        ptr[i] = tolower(ptr[i]);
        ++i;
    }
}

struct Opcode               //This node is used for Hashing using Chaining
{
    char name[20];
    char code[35];
    char format[5];
    struct Opcode *next;
};
struct Symbol               //Symbol Table is made using Linked List to save space
{
    char name[50];
    int add;
    struct Symbol *next;
};
typedef struct Opcode Opcode;
typedef struct Symbol Symbol;

Symbol *head=NULL;

Opcode *hash_table = NULL;

/*******************************************************************
    HASH TABLE IS USED TO STORE THE OPCODES BEING READ
*******************************************************************/
void insertIntoHashMap(Opcode *Node)
{
    if(hash_table == NULL) //boundary condition
    {
        hash_table = Node;
        Node->next = NULL;
    }
    else
    {
        Opcode* temp = hash_table;
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = Node;
        Node->next=NULL;
    }
}
int getAddressCode(char* temp)
{
    Symbol * t = head;
    int val = -1;
    while(t != NULL)
    {
        if(!strcmp(temp,t->name))
        {
            val = t->add;
            break;
        }
        t = t->next;
    }
    return val;
} 

Opcode* getOpcodeNode(char *op)
{
    Opcode* temp = hash_table;
    
    while(temp != NULL)
    {
        if(strcmp(temp->name,op)==0)
            return temp;
        temp=temp->next;
    }
    return NULL;
}
char * getOpcodeFormat(Opcode* temp)
{
    return temp->format;
}


int main(int argc,char* argv[])
{
    // File naming 
    char filename[50];
    for(int i=0;argv[1][i]!='\0' && argv[1][i]!='.';++i)
    {
        filename[i]=argv[1][i];
        filename[i+1]='\0';
    }
    char listingfile[50];
    char logfile[50];
    char objectfile[50];
    strcpy(listingfile,filename);
    strcat(listingfile,".l");
    strcpy(logfile,filename);
    strcat(logfile,".log");
    strcpy(objectfile,filename);
    strcat(objectfile,".o");
    //printf("%s %s %s %s created\n",filename,logfile,listingfile,objectfile);
    // File naming ends

    FILE *input_opcode;
    FILE *output_machine_code;
    FILE *input_instructions;
    FILE *object_file;
    FILE *log_file;
    int ilc=0;  //Instruction Location Counter
    int base = 0;
    char c,c2,c3,temp;
    char opcode[100];
    char machine_code[100];
    char format[5];
  
    
    input_opcode = fopen("input_opcode.txt","r+");  //input_opcode contains a list of opcodes followed by their format and mac.code
    log_file = fopen(logfile,"w+");  

    if (input_opcode == NULL)
        fprintf(log_file,"FILE OPENING PROBLEM");
    char test1[50];
    do
    {
        c = fscanf(input_opcode,"%s",opcode);//Assuming to get opcode as a string in opcode array
        c2= fscanf(input_opcode,"%s",machine_code);//Assuming to get a the integer as a string in machine_code array
        c3= fscanf(input_opcode,"%s",format);
        //now we will create node of each string
        struct Opcode* Node = (struct Opcode *) malloc(sizeof(Opcode));

        strcpy(Node->name,opcode);
        //Name of the opcode is fed
        strcpy(Node->code,machine_code);
        //Machine code of the opcode is fed
        strcpy(Node->format,format);
        //Format of the opcode is fed
      //  fprintf(log_file,"BEFORE INSERTING NAME:: %s ,CODE:: %s and format",Node->name,Node->code,Node->format);
        insertIntoHashMap(Node);


    }while(fgets(test1, sizeof test1, input_opcode)!=NULL);

        //At this point we have a hash-map of Opcodes

   fprintf(log_file,"Hash-map Created Successfully!\n");
    /*TEST:: PRINTING HASHTABLE with hashcode*/

    Opcode* temp1 = hash_table;
    while(temp1!=NULL)
    {
       fprintf(log_file,"NAME:: %s and CODE:: %s and format:: %s \n",temp1->name,temp1->code,temp1->format);
        temp1 = temp1->next;
    }
 
   fprintf(log_file,"Now reading Opcodes and Converting them to machine codes\n\n");

    input_instructions = fopen(argv[1],"r+");
    output_machine_code = fopen(listingfile,"w+");
    char k;
    char opt[100];

    /***********************************************************************************************/
	/*First pass for generation of symbol table*/
	
    while ( fgets ( opt, sizeof opt, input_instructions ) != NULL ) /* read a line */
    {
        char* op = trimwhitespace(opt);
        if(op[0]==';')
            continue;
        char t1[100];
        op=strtok(op,";");
        op = trimwhitespace(op);
        lowerstr(op);
        strcpy(t1,op);
        char* token = strtok(t1,":");
        if(op[strlen(t1)]==':')
        {
            //printf("label found%sEnd\n",t1);
            if(isalpha(t1[0])==0)
            {
               fprintf(log_file,"Error: invalid label : %s\n",t1);
            }
            else
            {    
                int len = strlen(t1);   len++;
                token  = trimwhitespace(t1);
                if(getAddressCode(token)!=-1)
                {
                   fprintf(log_file,"Error: repeated label : %s\n",t1);
                }
                else
                {
                    strcpy(t1,token);
                    char fun[20];
                    int x;
                    struct Symbol *temp = (struct Symbol*) malloc(sizeof(Symbol)); //dynamic memory allocation for a node of symbol
                    sscanf((op+len),"%s%n",fun,&x); x+=len;
                    if(strcmp(fun,"set")==0)
                    {
                        int x1;
                        sscanf(token+x,"%s%n",fun,&x1);
                        errno=0;  char* endptr;
                        int val = (int)strtol(fun,&endptr,0);
                        if(token[x+x1]!='\0')
                        {
                           fprintf(log_file,"Error: invalid argument format in : %s\n",token);
                        }
                        else if( (fun == endptr || errno!=0 || (errno==0 && fun && *endptr!=0)) && val==0)
                        {
                           fprintf(log_file,"Error: invalid value of SET command in line : %s\n",token);
                        }
                        else
                        {
                            strcpy(temp->name , t1);
                            temp->add = val;
                            temp->next = NULL;
                            if(head == NULL) //boundary condition for implementing symbol table using linked list
                                head = temp;
                            else  //adding new symbol node to existing table of nodes
                            {
                                Symbol *t = head;
                                while(t->next!=NULL)
                                    t= t->next;
                                t->next = temp;
                            }
                        }                
                    }
                    else if(strcmp(fun,"data")==0)
                    {
                        int x1;
                        sscanf(token+x,"%s%n",fun,&x1);
                        errno=0;  char* endptr;
                        int val = (int)strtol(fun,&endptr,0);
                        if(token[x+x1]!='\0')
                        {
                           fprintf(log_file,"Error: invalid argument format in : %s\n",token);
                        }
                        else if( (fun == endptr || errno!=0 || (errno==0 && fun && *endptr!=0)) && val==0)
                        {
                           fprintf(log_file,"Error: invalid value of DATA command in line : %s\n",token);
                        }
                        else
                        {
                            //printf("data found %s\n",opt);
                            indx++;
                            data[indx] = val;
                            data[0]=indx;
                        }
                        strcpy(temp->name , t1);
                        temp->add = ilc;
                        temp->next = NULL;
                        if(head == NULL) //boundary condition for implementing symbol table using linked list
                            head = temp;
                        else  //adding new symbol node to existing table of nodes
                        {
                            Symbol *t = head;
                            while(t->next!=NULL)
                                t= t->next;
                            t->next = temp;
                        }
                        if(strlen(fun)==0)
                            continue;                        
                    }
                    else
                    {
                        strcpy(temp->name , t1);
                        temp->add = ilc;
                        temp->next = NULL;
                        if(head == NULL) //boundary condition for implementing symbol table using linked list
                            head = temp;
                        else  //adding new symbol node to existing table of nodes
                        {
                            Symbol *t = head;
                            while(t->next!=NULL)
                                t= t->next;
                            t->next = temp;
                        }
                        if(strlen(fun)==0)
                            continue;
                    }
                }
            }  
        }        
        else
        {
            if(isalpha(op[0])==0)
            {
               fprintf(log_file,"Error: invalid mnemonic in line : %s\n",op);
            }
            else
            {
                char fun[20];
                int x;
                sscanf(op,"%s%n",fun,&x);
                if(strcmp(fun,"data")==0)
                {
                    int x1;
                    sscanf(op+x,"%s%n",fun,&x1);
                    errno=0;  char* endptr;
                    int val = (int)strtol(fun,&endptr,0);
                    if(op[x+x1]!='\0')
                    {
                       fprintf(log_file,"Error: invalid argument format in : %s\n",op);
                    }
                    else if( (fun == endptr || errno!=0 || (errno==0 && fun && *endptr!=0)) && val==0)
                    {
                       fprintf(log_file,"Error: invalid value of DATA command in line : %s\n",op);
                    }
                    else
                    {
                       fprintf(log_file,"data found %s\n",opt);
                        indx++;
                        data[indx] = val;
                        data[0]=indx;
                    }       
                }
            }
            
        }      
     ilc++;
 }
   fclose(input_instructions);
  fprintf(log_file,"\npass 1 finished\n\n");
   // PASS - 2
   input_instructions = fopen(argv[1],"r+");
   int mac_code[1000];
   int mcnt=0;

// *********************** DATA segment **************//

fprintf(output_machine_code,"DATA Segment:\nNumber of data items = %08X\n\n",indx);
if(indx==0)
{
    mac_code[mcnt++]=0; 
}
for(int in = 1 ; in <= indx ; ++in)
{
    fprintf(output_machine_code,"\t%08X\n",data[in]);
    mac_code[mcnt++] = data[in];
}
fprintf(output_machine_code,"\nCODE Segment:\n\n");


// ********** data seg ends ************************//


ilc = 0;
while ( fgets ( opt, sizeof opt, input_instructions ) != NULL ) /* read a line */
    {
        //printf("read: %s\n",opt);
        char* op = trimwhitespace(opt);
        if(op[0]==';')
        {
            fprintf(output_machine_code,"\t\t\t%s\n",opt);
            continue;
        }    
        char t1[100];
        op=strtok(op,";");
        op = trimwhitespace(op);
        lowerstr(op);
        strcpy(t1,op);
        char* token = strtok(t1,":");
        int x = 0;
        if(op[strlen(t1)]==':')
        {
            x = strlen(t1)+1;
        }
        if(op[x]=='\0')
        {
            //printf("label only\n");
            fprintf(output_machine_code,"%04X %s\n",ilc,opt);
            continue;
        }
        char fun[20];
        int x1;
        sscanf(op+x,"%s%n",fun,&x1);    x+=x1;
        if(strcmp(fun,"set")==0)
        {
            fprintf(output_machine_code,"%04X %s\n",ilc,opt);
        }
        else if(strcmp(fun,"data")==0)
        {
            fprintf(output_machine_code,"%04X %s\n",ilc,opt);
        }
        else if(getOpcodeNode(fun)!=NULL)
        {
            int inst = 0;
            Opcode* current_node = getOpcodeNode(fun);
            char* fo = getOpcodeFormat(current_node);
            if(strcmp(fo,"z")==0)
            {
                if(op[x]!='\0')
                {
                   fprintf(log_file,"Error: extra argument on line: %s\n",opt);
                }
                else
                {
                    inst = (int)strtol((current_node->code),NULL,0);
                    fprintf(output_machine_code,"%04X %08X %s\n",ilc,inst,opt);
                    mac_code[mcnt++] = inst;
                }                
            }
            else if(op[x]=='\0')
            {
               fprintf(log_file,"Error: missing argument on line : %s\n",opt);
            }
            else if(strcmp(fo,"v")==0)
            {
                sscanf(op+x,"%s%n",fun,&x1);    x+=x1;
                if(op[x]!='\0')
                {
                   fprintf(log_file,"Error: invalid argument format in : %s\n",opt);
                }
                else if(isdigit(fun[0]) || fun[0]=='-' || fun[0]=='+')
                {
                    errno=0;  char* endptr;
                    int val = (int)strtol(fun,&endptr,0);
                    //printf("val = %d errno = %d\n",val,errno);
                    if( (fun == endptr || errno!=0 || (errno==0 && fun && *endptr!=0)) && val==0)
                    {
                       fprintf(log_file,"Error: invalid value for command in line : %s\n",op);
                    }
                    else
                    {
                        inst = (val<<8);
                        val = (int)strtol((current_node->code),NULL,0);
                        inst = inst|val;
                        fprintf(output_machine_code,"%04X %08X %s\n",ilc,inst,opt);
                        mac_code[mcnt++] = inst;
                    }
                }
                else
                {
                    int val = getAddressCode(fun);
                    if(val==-1)
                    {
                       fprintf(log_file,"Error: invalid value for command in line : %s\n",op);
                    }
                    else
                    {
                        inst = (val<<8);
                        val = (int)strtol((current_node->code),NULL,0);
                        inst = inst|val;
                        fprintf(output_machine_code,"%04X %08X %s\n",ilc,inst,opt);
                        mac_code[mcnt++] = inst;
                    }
                    
                }
                
            }
            else if(strcmp(fo,"a")==0)
            {
                sscanf(op+x,"%s%n",fun,&x1);    x+=x1;
                if(op[x]!='\0')
                {
                   fprintf(log_file,"Error: invalid argument format in : %s\n",opt);
                }
                else if(getAddressCode(fun)!=-1)
                {
                    int inst = getAddressCode(fun);
                    inst = inst - ilc - 2 ;
                    int val = (int)strtol((current_node->code),NULL,0);
                    inst = (inst<<8)|val;
                    fprintf(output_machine_code,"%04X %08X %s\n",ilc,inst,opt);
                    mac_code[mcnt++] = inst;
                }
                else
                {
                   fprintf(log_file,"Error: invalid target label/symbol in line : %s\n",op);
                }
            } 
            else
            {
               fprintf(log_file,"Error: wrong mnemonic on line : %s\n",opt);
            }      
        }
        else
        {
           fprintf(log_file,"Error: wrong mnemonic on line : %s\n",opt);
        }    
     ilc++;
    }  
    fclose(input_instructions);
    fclose(output_machine_code);
    fclose(input_opcode);
   fprintf(log_file,"\npass 2 finished\n\n");

    // instructions in object file

    object_file = fopen(objectfile,"wb");
    fwrite(mac_code,sizeof(int),mcnt,object_file);
    fclose(object_file);
   fprintf(log_file,"\nobject file created\n");
    /*PRINT SYMBOL TABLE*/
    Symbol *p;
    p=head;
   fprintf(log_file,"\n\nSymbol Table\n\n");
    FILE *f = fopen("symbol_table.txt","w+");
    while(p!=NULL)
    {
       fprintf(log_file,"%s :: ",p->name);
        fprintf(f,"%s :: ",p->name);
       fprintf(log_file,"%d\n",p->add);
        fprintf(f,"%d\n",p->add);
        p = p->next;
    }
    fclose(log_file);
    return 0;
}