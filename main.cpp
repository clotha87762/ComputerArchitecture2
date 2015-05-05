#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "simulator.h"
#include "decoder.h"
#include <cstring>
using namespace std;
void boolClear();
simulator sim;
int main()
{  /*unsigned short s= 0xFFFF;
    int t = (int) s;
    printf("0x%08X",t);
    system("Pause");*/


    /*short x = -1;
    int t = -1;
     unsigned short tt = (unsigned short) x;
    unsigned int mask_temp = (unsigned int)tt;
    int a = mask_temp<<16  ;
    printf("0x%08X\n",a);


    system("Pause");*/



    int type = 1;
    int temp;

   /* if(type==1){
        FILE* W = fopen("iimage.bin","wb");
        unsigned int pp[20] = {0x00000000,0x0000000D,0x30080000,0x30090000,0x292A0003,0x11400003,0x01094020,0x21290001,0x08000002,
        0xAC080000,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0x00000400,0x00000003,0x12345678,0x9ABCDEF0,0x13572468};

        for(int i =0;i<15;i++){
            fwrite(&pp[i],sizeof(int),1,W);
        }

        fclose(W);

        W= fopen("dimage","wb");

        for(int i=15;i<20;i++)
           fwrite(&pp[i],sizeof(int),1,W);


        fclose(W);

    }
    else{
        FILE* W = fopen("test","rb");
        int status = fread(&temp,sizeof(int),1,W);
        printf("%x \n",temp);
    }
*/







    FILE* iimage =  fopen("iimage.bin","rb");
    FILE* dimage =  fopen("dimage.bin","rb");
   /* for(int i=0;i<4;i++){
        fread(&temp,sizeof(int),1,iimage);
         printf("%x \n",temp);
         fread(&temp,sizeof(int),1,dimage);
         printf("%x \n",temp);
    }*/




    decodeInstructions(iimage);
    decodeData(dimage);





   /*cout<<hex<<"PC "<<sim.PC<<endl;
    cout<<sim.I_number<<endl;
    cout<<sim.D_number<<endl;

   for(int i=0;i<32;i++){
       // cout<<"reg"<<dec<<i<<" "<<hex<<sim.reg[i]<<endl;
       printf("reg%d 0x%08x\n",i,sim.reg[i]);
    }
    for(int i=sim.PC;i<sim.PC+sim.I_number_byte;i+=4){
        unsigned int instruction = (sim.I_mem_byte[i]<<24)|(sim.I_mem_byte[i+1]<<16)| (sim.I_mem_byte[i+2]<<8) | (sim.I_mem_byte[i+3]);
         printf("Instruction: 0x%08x\n",instruction);
        int h;

        h=sim.decodeInstruction(instruction);

        if(sim.nowType==Rtype){
            cout<<"op "<<hex<<sim.r_instruction.opcode<<endl;
            cout<<"rs "<<hex<<sim.r_instruction.rs<<endl;
            cout<<"rt "<<hex<<sim.r_instruction.rt<<endl;
            cout<<"rd "<<hex<<sim.r_instruction.rd<<endl;
            cout<<"shamt "<<hex<<sim.r_instruction.shamt<<endl;
            cout<<"funct "<<hex<<sim.r_instruction.funct<<endl;
            cout<<"nowOp "<<sim.nowOp<<endl;
        }
        else if(sim.nowType==Itype){
            cout<<"op "<<hex<<sim.i_instruction.opcode<<endl;
            cout<<"rs "<<hex<<sim.i_instruction.rs<<endl;
            cout<<"rt "<<hex<<sim.i_instruction.rt<<endl;
            cout<<"immediate "<<sim.i_instruction.immediate<<endl;

        }else{
            cout<<"op "<<hex<<sim.j_instruction.opcode<<endl;
            cout<<"add "<<hex<<sim.j_instruction.address<<endl;

        }
    }
    for(int i=0;i<sim.D_number_byte;i+=4){
        unsigned int data4 = (((int)sim.D_mem_byte[i])<<24 )| (((int)sim.D_mem_byte[i+1])<<16) | (((int)sim.D_mem_byte[i+2])<<8) | (((int)sim.D_mem_byte[i+3]));

        cout<<hex<<"Data"<<i<<" "<<data4<<endl;
    }

 system("Pause");*/


   /* sim.reg[8] = 8;
    sim.reg[1] = 1;
    sim.nowOp = SRL;
    sim.nowType=Rtype;
*/
    /*
    sim.i_instruction.immediate = -1;
    sim.i_instruction.rt = 0;
    sim.i_instruction.rs = 8;
    sim.i_instruction.opcode = 0x23;*/
/*
    sim.r_instruction.opcode=0x00;
    sim.r_instruction.rs = 8;
    sim.r_instruction.rt = 8;
     sim.r_instruction.rd = 3;
    sim.r_instruction.funct = 0x02;
    sim.r_instruction.shamt = 1;*/


   FILE* f = fopen("snapshot2.rpt", "w");
   FILE* E = fopen("error_dump2.rpt","w");
   bool errorHalt;
   unsigned int fetched_ins;
   unsigned int before_branch;
   while(1){

       /* if(sim.IFflush){
            fetched_ins = before_branch;
        }
        else{*/
            fetched_ins = (sim.I_mem_byte[sim.PC]<<24)|(sim.I_mem_byte[(sim.PC)+1]<<16)| (sim.I_mem_byte[(sim.PC)+2]<<8) | (sim.I_mem_byte[(sim.PC)+3]);
        //}
        sim.PrintReg(f,fetched_ins);




        if(fetched_ins==0xFFFFFFFF){
             bool toHalt=false;
            int h;
            for(h =0;h<4;h++){
             if(sim.ppName[h]!=HALT)break;
             if(h==3) toHalt=true;
            }
            if(toHalt)break;

        }

        if(sim.branch||sim.IFstall||sim.IFflush)sim.PC-=4;

        sim.cycle++;


        errorHalt=sim.printError(E);
        if(errorHalt)break;


        sim.shiftPipeline();

        if(!sim.IFstall&&!sim.IDstall&&!sim.IFflush){
            sim.writeTo0[0]=false;
            sim.numberOverflow[0]=false;
            sim.addressOverflow[0]=false;
            sim.misalignment[0]=false;
            sim.decodeInstruction();
            sim.ExecuteInstruction(E);

        }




        if(sim.IFflush){

            sim.PC = sim.PCtemp-4;


        }

        boolClear();

       // unsigned int instruction_fetched = (I_mem_byte[PC]<<24)|(I_mem_byte[PC+1]<<16)| (I_mem_byte[PC+2]<<8) | (I_mem_byte[PC+3]);
       // system("Pause");
        sim.stallDetect();

        //if((!sim.IFstall)&&(!sim.IDstall))
        sim.forwardDetect();



        if((!sim.IFstall)&&(!sim.IDstall)){

            if(sim.branch){
                sim.IFflush=true;
                before_branch=fetched_ins;
                sim.branch=0;
            }

        }



            sim.PC+=4;



    }


    return 0;


}


void boolClear(){
    sim.EX_FWD_EXMEM_rs=false;
    sim.IFstall=false;
    sim.IDstall=false;
    sim.IFflush=false;
    sim.ID_FWD_rs=false;
    sim.ID_FWD_rt=false;
    sim.EX_FWD_EXMEM_rs=false;
    sim.EX_FWD_MEMWB_rs=false;
    sim.EX_FWD_EXMEM_rt=false;
    sim.EX_FWD_MEMWB_rt=false;

}


