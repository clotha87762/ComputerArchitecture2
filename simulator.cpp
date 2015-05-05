#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "simulator.h"
#include "decoder.h"


using namespace std;
#define RRS (r_instruction.rs)
#define RRT (r_instruction.rt)
#define RRD (r_instruction.rd)
#define IRS (i_instruction.rs)
#define IRT (i_instruction.rt)
#define IIM (i_instruction.immediate)
#define JA (j_instruction.address)



simulator::simulator(){
     I_number=0;
     D_number=0;
     I_number_byte=0;
     D_number_byte = 0;
     PC=0;
     cycle = 0;
     skipCycle=false;
     errorHalt=false;
     nowType = Rtype;
     nowOp = NOP;
     for(int i=0;i<5;i++){
       for(int j=0;j<32;j++)
            reg[i][j]=0;

       if(i==4)break;
        ppName[i] = NOP;
        rs[i]=0;
        rt[i]=0;
        rd[i]=0;

     }

     memset(I_mem, 0x00, sizeof(I_mem));
     memset(I_mem_byte, 0x00, sizeof(I_mem_byte));
     memset(D_mem, 0x00, sizeof(D_mem));
     memset(D_mem_byte, 0x00, sizeof(D_mem_byte));
     PC=0;

}

void simulator::decodeInstruction(){
        int halt = 0;

        unsigned int instruction = (I_mem_byte[PC]<<24)|(I_mem_byte[PC+1]<<16)| (I_mem_byte[PC+2]<<8) | (I_mem_byte[PC+3]);//會不會是變數名字的關係????
        int mask;
        //printf("ins 0x%08X\n",instruction);
        mask = 0xFC000000;
        opcode = (instruction&mask)>>26;
        if(opcode==0x00){

            r_instruction.opcode = opcode;
            r_instruction.rs=(instruction<<6)>>27;
            r_instruction.rt=(instruction<<11)>>27;
            r_instruction.rd= (instruction<<16)>>27;
            rd[0]=r_instruction.rd;
            rs[0]=r_instruction.rs;
            rt[0]=r_instruction.rt;

            r_instruction.shamt = (instruction<<21)>>27;
            r_instruction.funct=(instruction<<26)>>26;
           // cout<<"YOOfunct: "<<r_instruction.funct<<endl;
            nowType = Rtype;
            switch(r_instruction.funct){
                case 0x20:
                    nowOp = ADD;
                    ppName[0]=ADD;
                    break;
                case 0x22:
                     nowOp = SUB;
                    ppName[0]=SUB;
                    break;
                case 0x24:
                     nowOp = AND;
                    ppName[0]=AND;
                    break;
                case 0x25:
                     nowOp = OR;
                    ppName[0]=OR;
                    break;
                case 0x26:
                     nowOp = XOR;
                    ppName[0]=XOR;
                    break;
                case 0x27:
                     nowOp = NOR;
                    ppName[0]=NOR;
                    break;
                case 0x28:
                     nowOp = NAND;
                    ppName[0]=NAND;
                    break;
                case 0x2A:
                     nowOp = SLT;
                    ppName[0]=SLT;
                    break;
                case 0x00:
                     if(r_instruction.rd==0&&r_instruction.rt==0&&r_instruction.shamt==0){
                      nowOp =NOP;
                    ppName[0]=NOP;
                     }
                     else{
                     nowOp = SLL;
                     rs[0]=0;
                    ppName[0]=SLL;
                     }
                    break;
                case 0x02:
                     nowOp = SRL;
                     rs[0]=0;
                    ppName[0]=SRL;
                    break;
                case 0x03:
                     nowOp = SRA;
                     rs[0]=0;
                    ppName[0]=SRA;
                    break;
                case 0x08:
                    nowOp = JR;
                    ppName[0]=JR;
                    rd[0]=0;
                    rt[0]=0;
                    break;
                default:
                    break;

            }


        }
        else if(opcode==0x03||opcode==0x02){
            j_instruction.opcode = opcode;
            j_instruction.address = (instruction<<6)>>6;

            nowType = Jtype;
            rs[0]=0;
            rd[0]=0;
            rt[0]=0;
            if(opcode==0x03){
                nowOp = JAL;
                ppName[0]=JAL;
                rd[0]=31;
            }
            else if(opcode==0x02){
                nowOp = J;
                ppName[0]=J;
            }

        }
        else if(opcode==0x3F){
            //Halt

            nowOp = HALT;
            ppName[0]=HALT;
            rs[0]=0;
            rt[0]=0;
            rd[0]=0;


        }
        else if(opcode==0x08||opcode==0x23||opcode==0x21||opcode==0x25||opcode==0x20||opcode==0x24||opcode==0x2B||opcode==0x29||
                opcode==0x28||opcode==0x0F||opcode==0x0D||opcode==0x0E||opcode==0x0C||opcode==0x0A||opcode==0x04||opcode==0x05){

            i_instruction.opcode = opcode;
            i_instruction.rs = (instruction<<6)>>27;
            i_instruction.rt = (instruction<<11)>>27;
            rs[0] = i_instruction.rs;
            rt[0] = 0;
            rd[0] = i_instruction.rt;

            i_instruction.immediate = (signed short)((instruction<<16)>>16);
            //cout<<"yooimmediate: "<<dec<<i_instruction.immediate<<endl;
           /* int signmask=1;
            if(((signmask<<15)&i_instruction.immediate)==1)*/


            nowType = Itype;

            switch(opcode){

                case 0x08:
                    nowOp = ADDI;
                    ppName[0]=ADDI;
                    break;
                case 0x23:
                     nowOp = LW;
                    ppName[0]=LW;
                    break;
                case 0x21:
                     nowOp = LH;
                    ppName[0]=LH;
                    break;
                case 0x25:
                     nowOp = LHU;
                    ppName[0]=LHU;
                    break;
                case 0x20:
                     nowOp = LB;
                    ppName[0]=LB;
                    break;
                case 0x24:
                     nowOp = LBU;
                    ppName[0]=LBU;
                    break;
                case 0x2B:
                    nowOp = SW;
                    ppName[0]=SW;
                    rt[0]=rd[0];
                    rd[0]=0;
                    break;
                case 0x29:
                    nowOp = SH;
                    ppName[0]=SH;
                    rt[0]=rd[0];
                    rd[0]=0;
                    break;
                case 0x28:
                    nowOp = SB;
                    ppName[0]=SB;
                    rt[0]=rd[0];
                    rd[0]=0;
                    break;
                case 0x0F:
                    nowOp = LUI;
                    ppName[0]=LUI;
                    break;
                case 0x0C:
                    nowOp = ANDI;
                    ppName[0]=ANDI;
                    break;
                case 0x0D:
                    nowOp = ORI;
                    ppName[0]=ORI;
                    break;
                case 0x0E:
                    nowOp = NORI;
                    ppName[0]=NORI;
                    break;
                case 0x0A:
                    nowOp = SLTI;
                    ppName[0]=SLTI;
                    break;
                case 0x04:
                    nowOp = BEQ;
                    ppName[0]=BEQ;
                    rt[0]=rd[0];
                    rd[0]=0;
                    break;
                case 0x05:
                    nowOp = BNE;
                    ppName[0]=BNE;
                    rt[0]=rd[0];
                    rd[0]=0;
                    break;
            }

        }



}




void simulator::PrintReg(FILE* F,unsigned int fetched_ins){

    fprintf(F,"cycle %d\n",cycle);
    for(int i=0;i<32;i++){

        if(i<10)fprintf(F,"$0%d: 0x%08X\n",i,reg[4][i]);
        else{fprintf(F,"$%d: 0x%08X\n",i,reg[4][i]); }
    }

    fprintf(F,"PC: 0x%08X\n",PC);

    fprintf(F,"IF: 0x%08X",fetched_ins);
    if(IFstall){
    fprintf(F," to_be_stalled");
    }
    else if(IFflush)
    fprintf(F," to_be_flushed");

    fprintf(F,"\n");

    fprintf(F,"ID: %s",printIns(ppName[0]));
    if(IDstall){
    fprintf(F," to_be_stalled");
    }

    if(ID_FWD_rs){
    fprintf(F," fwd_EX-DM_rs_$%d",rs[0]);
    }
    if(ID_FWD_rt){
    fprintf(F," fwd_EX-DM_rt_$%d",rt[0]);
    }

    fprintf(F,"\n");


    fprintf(F,"EX: %s",printIns(ppName[1]));
    if(EX_FWD_EXMEM_rs){
    fprintf(F," fwd_EX-DM_rs_$%d",rs[1]);
    }
    else if(EX_FWD_MEMWB_rs){
     fprintf(F," fwd_DM-WB_rs_$%d",rs[1]);
    }

    if(EX_FWD_EXMEM_rt){
    fprintf(F," fwd_EX-DM_rt_$%d",rt[1]);
    }
    else if(EX_FWD_MEMWB_rt){
     fprintf(F," fwd_DM-WB_rt_$%d",rt[1]);
    }


    fprintf(F,"\n");

    fprintf(F,"DM: %s\n",printIns(ppName[2]));
    fprintf(F,"WB: %s\n",printIns(ppName[3]));

   /* for(int i=0;i<4;i++)
        fprintf(F,"[%d] rs=%d   rt=%d   rd=%d\n",i,rs[i],rt[i],rd[i]);
    */
    fprintf(F,"\n\n");

}


char* simulator::printIns(instruction_name x){



    char* temp;
        if(x==NOP){
         temp="NOP";
         return temp;
        }
        else if(x==HALT){
            temp="HALT";
            return temp;
        }
        else if(x == ADD){
             temp="ADD";
             return temp;
        }
       else if(x == ADDI){
             temp="ADDI";
             return temp;
       }
       else if(x == AND){
            temp="AND";
             return temp;
       }
       else if(x == ANDI){
            temp="ANDI";
             return temp;
       }
       else if(x == BEQ){
            temp="BEQ";
             return temp;
       }
       else if(x == BNE){
            temp="BNE";
             return temp;
       }
       else if(x == J){
            temp="J";
             return temp;
       }
       else if(x == JAL){
            temp="JAL";
             return temp;
       }
       else if(x == JR){
            temp="JR";
             return temp;
       }
       else if(x == LB){
            temp="LB";
             return temp;
       }
       else if(x == LBU){
            temp="LBU";
             return temp;
       }
       else if(x == LH){
            temp="LH";
             return temp;
       }
       else if(x == LHU){
            temp="LHU";
             return temp;
       }
       else if(x ==LUI){
            temp="LUI";
             return temp;
       }
       else if(x == LW){
            temp="LW";
             return temp;
       }
       else if(x == NAND){
            temp="NAND";
             return temp;
       }
       else if(x == NOR){
            temp="NOR";
             return temp;
       }
       else if(x == NORI){
            temp="NORI";
             return temp;
       }
       else if(x == OR){
            temp="OR";
             return temp;
       }
       else if(x == ORI){
            temp="ORI";
             return temp;
       }
       else if(x == SB){
            temp="SB";
             return temp;
       }
       else if(x == SH){
            temp="SH";
             return temp;
       }
       else if(x == SLL){
           temp="SLL";
             return temp;
       }
       else if(x == SLT){
            temp="SLT";
             return temp;
       }
       else if(x ==SLTI){
            temp="SLTI";
             return temp;
       }
       else if(x == SRA){
            temp="SRA";
             return temp;
       }
       else if(x == SRL){
            temp="SRL";
             return temp;
       }
       else if(x == SUB){
            temp="SUB";
             return temp;
       }
       else if(x == SW){
            temp="SW";
             return temp;
       }
       else if(x == XOR){
           temp="XOR";
            return temp;
       }


}


void  simulator::Deal_WritetoReg0(FILE* E){

        if(nowOp==NOP) return;

        if(nowType==Itype){

            if(i_instruction.rt!=0x00)return;

            if(nowOp==SW||nowOp==SH||nowOp==SB||nowOp==BEQ||nowOp==BNE) return;


            skipCycle = true;
            writeTo0[0] = true;
            return;

        }
        else if(nowType==Rtype){

            if(r_instruction.rd!=0x00) return;

            if(nowOp==SLL&&r_instruction.shamt==0x00&&r_instruction.rt==0x00) return;

            if(nowOp==JR) return;


            writeTo0[0] = true;
            skipCycle = true;
            return;

        }

}

bool isOverflow ;


void overflowCmp(int x,int y){

    int sum = x+y;
    int mask = (1<<31);
    if((x&mask) == (y&mask))
        if((sum&mask) != (x&mask))
            isOverflow=true;

}

void  simulator::Deal_NumberOverflow(FILE* E){


    isOverflow=false;

    if(!(nowOp==ADD||nowOp==ADDI||nowOp==SUB||nowOp==LW||nowOp==LH||nowOp==LHU||nowOp==LB||nowOp==LBU
       ||nowOp==SW||nowOp==SH||nowOp==SB||nowOp==BEQ||nowOp==BNE))
       return;


    if(nowType==Rtype){

        if(nowOp==ADD){


          overflowCmp(reg[0][RRS],reg[0][RRT]);


    }
    else if(nowOp==SUB){


            overflowCmp(reg[0][RRS],-reg[0][RRT]);

        }

    }
    else if(nowType==Itype){

            if(nowOp==BEQ||nowOp==BNE){

                if(PC+4>=0&&i_instruction.immediate>=0&&((int)((PC+4)+4*i_instruction.immediate))<0)
                    isOverflow=true;

            }
            else{


                 overflowCmp(reg[0][IRS],IIM);
            }

    }

    if(isOverflow)
        numberOverflow[0] =true;

}



void  simulator::Deal_MemoryAddOverflow(FILE* E){

    if(nowOp==LW||nowOp==LH||nowOp==LHU||nowOp==LB||nowOp==LBU||nowOp==SW||nowOp==SH||nowOp==SB){

        if(nowOp==LB||nowOp==LBU||nowOp==SB){
            if(reg[0][i_instruction.rs]+i_instruction.immediate>=1024||reg[0][i_instruction.rs]+i_instruction.immediate<0){

            skipCycle = true;
            addressOverflow[0]=true;
            }
        }
        else if(nowOp==LH||nowOp==LHU||nowOp==SH){
            if(reg[0][i_instruction.rs]+i_instruction.immediate>=1023||reg[0][i_instruction.rs]+i_instruction.immediate<0){

            skipCycle = true;
            addressOverflow[0]=true;
            }
        }
        else if(nowOp==LW||nowOp==SW){
            if(reg[0][i_instruction.rs]+i_instruction.immediate>=1021||reg[0][i_instruction.rs]+i_instruction.immediate<0){

            skipCycle = true;
            addressOverflow[0]=true;
            }
        }

    }
}


void  simulator::Deal_DataMisaligned(FILE* E){

    if(nowOp==LW){

        if((reg[0][i_instruction.rs]+i_instruction.immediate)%4!=0){

           skipCycle = true;
           misalignment[0]=true;
        }
    }
    else if(nowOp==LH||nowOp==LHU){

            if((reg[0][i_instruction.rs]+i_instruction.immediate)%2!=0){

            skipCycle = true;
            misalignment[0]=true;
            }
    }
    else if(nowOp==SW){
        if((reg[0][i_instruction.rs]+i_instruction.immediate)%4!=0){

               skipCycle = true;
               misalignment[0]=true;
            }
    }
    else if(nowOp==SH){
            if((reg[0][i_instruction.rs]+i_instruction.immediate)%2!=0){

                skipCycle = true;
                misalignment[0]=true;
             }
    }


}


void simulator::ExecuteInstruction(FILE* E){

        Deal_WritetoReg0(E);
        Deal_NumberOverflow(E);
        Deal_MemoryAddOverflow(E);
        Deal_DataMisaligned(E);


        if(skipCycle){
            skipCycle=false;
            return;
        }



       if(nowOp == ADD)
             add_Instruction();
       else if(nowOp == ADDI)
             addi_Instruction();
       else if(nowOp == AND)
            and_Instruction();
       else if(nowOp == ANDI)
            andi_Instruction();
       else if(nowOp == BEQ)
            beq_Instruction();
       else if(nowOp == BNE)
            bne_Instruction();
       else if(nowOp == J)
            j_Instruction();
       else if(nowOp == JAL)
            jal_Instruction();
       else if(nowOp == JR)
            jr_Instruction();
       else if(nowOp == LB)
            lb_Instruction();
       else if(nowOp == LBU)
            lbu_Instruction();
       else if(nowOp == LH)
            lh_Instruction();
       else if(nowOp == LHU)
            lhu_Instruction();
       else if(nowOp ==LUI)
            lui_Instruction();
       else if(nowOp == LW)
            lw_Instruction();
       else if(nowOp == NAND)
            nand_Instruction();
       else if(nowOp == NOR)
            nor_Instruction();
       else if(nowOp == NORI)
            nori_Instruction();
       else if(nowOp == OR)
            or_Instruction();
       else if(nowOp == ORI)
            ori_Instruction();
       else if(nowOp == SB)
            sb_Instruction();
       else if(nowOp == SH)
            sh_Instruction();
       else if(nowOp == SLL)
            sll_Instruction();
       else if(nowOp == SLT)
            slt_Instruction();
       else if(nowOp ==SLTI)
            slti_Instruction();
       else if(nowOp == SRA)
            sra_Instruction();
       else if(nowOp == SRL)
            srl_Instruction();
       else if(nowOp == SUB)
            sub_Instruction();
       else if(nowOp == SW)
            sw_Instruction();
       else if(nowOp == XOR)
            xor_Instruction();


}

void simulator::add_Instruction(){

    reg[0][RRD]=reg[0][RRS]+reg[0][RRT];

}

void  simulator::addi_Instruction(){

    reg[0][IRT] = reg[0][IRS] + IIM;
}

void  simulator::and_Instruction(){
    reg[0][RRD] = reg[0][RRS] & reg[0][RRT];
}

void simulator::andi_Instruction(){
    unsigned short temp = (unsigned short) IIM;
    //printf("temp 0x%08X",temp);
    unsigned int mask_temp = (unsigned int)temp;
   // printf("tempmask 0x%08X",temp);
    reg[0][IRT] = (int) (((unsigned int)reg[0][IRS]) & mask_temp);
}

void simulator::beq_Instruction(){
    if(reg[0][IRS]==reg[0][IRT]){
        PCtemp =PC+ (4+IIM*4);
        branch = 1;
    }
}

void simulator::bne_Instruction(){
    if(reg[0][IRS]!=reg[0][IRT]){
        PCtemp =PC+ (4+IIM*4);
        branch = 1;
    }
}

void simulator::j_Instruction(){

    PCtemp = (((PC+4)>>28)<<28) | ((JA)<<2);
    branch = 1;
}


void simulator::jal_Instruction(){
    reg[0][31]=PC+4;
    PCtemp = (((PC+4)>>28)<<28) | ((JA)<<2);  // 這邊啦啦啦啦啦啦啦啦啦啦啦啦啊啦
                                            //ㄆㄆㄆㄆㄆㄆㄆㄆ
    branch = 1;
}


void simulator::jr_Instruction(){
    PCtemp =(unsigned int) reg[0][RRS];
    branch = 1;
}

void simulator::lb_Instruction(){
    char temp;
    temp =  ((char)D_mem_byte[reg[0][IRS] + IIM]);
    reg[0][IRT] = (int)temp;
}

void simulator::lbu_Instruction(){ // 恩亨？
    int temp = 0;
    unsigned char unchar = (unsigned char) D_mem_byte[reg[0][IRS] + IIM];
    temp = (int)unchar;
    reg[0][IRT] = temp;
}


void simulator::lh_Instruction(){
    short temp;
    temp = (((short)D_mem_byte[reg[0][IRS] + IIM])<<8) | ((unsigned short)D_mem_byte[reg[0][IRS] + IIM+1]);

    reg[0][IRT] = temp;
   // printf("TEMP 0x%08X\n",temp);
    //reg[IRT] = temp
}

void simulator::lhu_Instruction(){
     unsigned short temp;
     temp = (((unsigned short)D_mem_byte[reg[0][IRS] + IIM])<<8) | ((unsigned short)D_mem_byte[reg[0][IRS] + IIM+1]);
   // printf("TEMP 0x%08X\n",temp);
    reg[0][IRT] = temp;
    /*(((int)D_mem_byte[reg[IRS] + IIM])<<8)|((int) (D_mem_byte[reg[IRS] + IIM +1]))<0?
                     temp
                    :(((int)D_mem_byte[reg[IRS] + IIM])<<8)|((int) (D_mem_byte[reg[IRS] + IIM +1]))*/;
}


void simulator::lui_Instruction(){
    unsigned short temp ;
    temp = (unsigned short)IIM;
    unsigned int temp2 =((unsigned int) temp)<<16;
    reg[0][IRT] = temp2;
}

void simulator::lw_Instruction(){
    int temp;
    temp = reg[0][IRS]+IIM ;
    reg[0][IRT] = (((unsigned int)D_mem_byte[temp])<<24 )| (((unsigned int)D_mem_byte[temp+1])<<16) | (((unsigned int)D_mem_byte[temp+2])<<8) | (((unsigned int)D_mem_byte[temp+3]));
}

void simulator::nand_Instruction(){
    reg[0][RRD] = ~(reg[0][RRS]&reg[0][RRT]);
}

void simulator::nor_Instruction(){
    reg[0][RRD] = ~(reg[0][RRS]|reg[0][RRT]);
}

void simulator::nori_Instruction(){

    unsigned short temp = (unsigned short) IIM;
    unsigned int mask_temp = (unsigned int)temp;
    reg[0][IRT] = (int)(~(((unsigned int)reg[0][IRS]) | mask_temp));
}

void simulator::or_Instruction(){
    reg[0][RRD] = reg[0][RRS] | reg[0][RRT];
}

void simulator::ori_Instruction(){
    //unsigned short unshort = (unsigned short);
    unsigned short temp = (unsigned short) IIM;

    unsigned int mask_temp = (unsigned int)temp;
    reg[0][IRT] = (int)(((unsigned int)reg[0][IRS]) | mask_temp);
}

void simulator::sb_Instruction(){
    D_mem_byte[reg[0][IRS]+IIM] =(unsigned char) (reg[0][IRT] & 0x000000FF);
}

void simulator::sh_Instruction(){
    D_mem_byte[reg[0][IRS]+IIM] =  (unsigned char)((reg[0][IRT]&0x0000FF00)>>8);
    D_mem_byte[reg[0][IRS]+IIM+1] = (unsigned char)(reg[0][IRT]&0x000000FF);

}

void simulator::sll_Instruction(){

    reg[0][RRD] =  reg[0][RRT] << r_instruction.shamt;

}


void simulator::slt_Instruction(){
    reg[0][RRD] = reg[0][RRS]<reg[0][RRT];
}

void simulator::slti_Instruction(){
    reg[0][IRT] = reg[0][IRS] <IIM;
}

void simulator::sra_Instruction(){
   /* int sign=((0x80000000&reg[RRT])>>31);

    if(sign){
        int temp = (int)(reg[RRT] >>r_instruction.shamt);
        temp *= -1;
        reg[RRD]= (unsigned int)temp;
    }
    else{
        reg[RRD] = reg[RRT] >>r_instruction.shamt;
    }*/
    reg[0][RRD] = reg[0][RRT] >>r_instruction.shamt;

}

void simulator::srl_Instruction(){
    unsigned int temp = (unsigned int) reg[0][RRT];
    temp = temp>>r_instruction.shamt;
    reg[0][RRD] =temp;
}

void simulator::sub_Instruction(){
    reg[0][RRD] = reg[0][RRS] - reg[0][RRT];
}

void simulator::sw_Instruction(){
   D_mem_byte[reg[0][IRS]+IIM] = ((unsigned int)(reg[0][IRT]&0xFF000000))>>24;
   D_mem_byte[reg[0][IRS]+IIM+1] = (reg[0][IRT]&0x00FF0000)>>16;
   D_mem_byte[reg[0][IRS]+IIM+2] = (reg[0][IRT]&0x0000FF00)>>8;
   D_mem_byte[reg[0][IRS]+IIM+3] = reg[0][IRT]&0x000000FF;
}

void simulator::xor_Instruction(){
    reg[0][RRD] = reg[0][RRS]^reg[0][RRT];
}



void simulator::shiftPipeline(){

    int bubble;
    bubble=-1;
    if(IFstall&&IDstall)bubble=1;
    else if(IFflush)bubble=0;

    int i;
    for(i=4;i>(bubble>=0? bubble:0);i--){

        for(int j=0;j<32;j++){

            reg[i][j] = reg[i-1][j];
        }
        if(i==4)continue;
        ppName[i]=ppName[i-1];
        rt[i]=rt[i-1];
        rs[i]=rs[i-1];
        rd[i]=rd[i-1];
        writeTo0[i]=writeTo0[i-1];
        numberOverflow[i]=numberOverflow[i-1];
        addressOverflow[i]=addressOverflow[i-1];
        misalignment[i]=misalignment[i-1];

    }

    if(bubble>=0){

        ppName[i]=NOP;
        rt[i]=0;
        rs[i]=0;
        rd[i]=0;
        writeTo0[i]=false;
        numberOverflow[i]=false;
        addressOverflow[i]=false;
        misalignment[i]=false;
    }



}

void simulator::forwardDetect(){


    if(ppName[0]==BEQ||ppName[0]==BNE||ppName[0]==JR){
        if(rd[2]!=0&&(rd[2]==rs[0])){

            if(ppName[2]==JR||ppName[2]==BEQ||ppName[2]==BNE||ppName[2]==SW||ppName[2]==SH||ppName[2]==SB){
                return;
            }

           if(!(IFstall&&IDstall))
            ID_FWD_rs=true;

        }
    }

    if(ppName[0]==BEQ||ppName[0]==BNE||ppName[0]==JR){
        if(rd[2]!=0&&(rd[2]==rt[0])){

            if(ppName[2]==JR||ppName[2]==BEQ||ppName[2]==BNE||ppName[2]==SW||ppName[2]==SH||ppName[2]==SB){
                return;
            }

            if(!(IFstall&&IDstall))
           ID_FWD_rt=true;

        }
    }
 /*   if(rd[2]!=0&&rd[2]==rt[0]){

        if(ppName[1]==JR||ppName[1]==BEQ||ppName[1]==BNE||ppName[1]==SW||ppName[1]==SH||ppName[1]==SB){
            return;
        }

        ID_FWD_rt=true;
    }
*/


    if(rd[2]!=0&&rd[2]==rs[1]){

        if(ppName[2]==JR||ppName[2]==BEQ||ppName[2]==BNE||ppName[2]==SW||ppName[2]==SH||ppName[2]==SB){
            return;
        }
      if(!(ppName[1]==BEQ||ppName[1]==BNE||ppName[1]==JR))
       EX_FWD_EXMEM_rs=true;

    }
    else if(rd[3]!=0&&rd[3]==rs[1]){

        if(ppName[3]==JR||ppName[3]==BEQ||ppName[3]==BNE||ppName[3]==SW||ppName[3]==SH||ppName[3]==SB){
            return;
        }
        if(!(ppName[1]==BEQ||ppName[1]==BNE||ppName[1]==JR))
        EX_FWD_MEMWB_rs=true;
    }


    if(rd[2]!=0&&rd[2]==rt[1]){

        if(ppName[2]==JR||ppName[2]==BEQ||ppName[2]==BNE||ppName[2]==SW||ppName[2]==SH||ppName[2]==SB){
            return;
        }
       if(!(ppName[1]==BEQ||ppName[1]==BNE||ppName[1]==JR))
        EX_FWD_EXMEM_rt=true;
    }
    else if(rd[3]!=0&&rd[3]==rt[1]){

         if(ppName[3]==JR||ppName[3]==BEQ||ppName[3]==BNE||ppName[3]==SW||ppName[3]==SH||ppName[3]==SB){
            return;
        }
        if(!(ppName[1]==BEQ||ppName[1]==BNE||ppName[1]==JR))
        EX_FWD_MEMWB_rt=true;
    }


}

void simulator::stallDetect(){

    if(ppName[1]==LW||ppName[1]==LH||ppName[1]==LB||ppName[1]==LHU||ppName[1]==LBU){

        if(rd[1]!=0&&(rd[1]==rs[0]||rd[1]==rt[0])){
            IFstall=true;
            IDstall=true;
        }


    }

    if(ppName[2]==LW||ppName[2]==LH||ppName[2]==LB||ppName[2]==LHU||ppName[2]==LBU){

        if(!(ppName[0]==BEQ||ppName[0]==BNE||ppName[0]==J||ppName[0]==JAL||ppName[0]==JR))return;

        if(rd[2]!=0&&(rd[2]==rs[0]||rd[2]==rt[0])){
            IFstall=true;
            IDstall=true;
        }

    }

    if(ppName[0]==BEQ||ppName[0]==BNE||ppName[0]==JR){

        if(rd[1]!=0&&(rd[1]==rs[0]||rd[1]==rt[0])){

            IFstall=true;
            IDstall=true;

        }

    }


}

bool simulator::printError(FILE* E){

    bool errorTemp;
    errorTemp=false;

    if(writeTo0[3]==true){
        fprintf(E , "In cycle %d: Write $0 Error\n", cycle);
    }

    if(addressOverflow[2]==true){
       fprintf(E , "In cycle %d: Address Overflow\n", cycle);
       errorTemp=true;
    }

    if(misalignment[2]==true){
        fprintf(E , "In cycle %d: Misalignment Error\n", cycle);
        errorTemp =true;
    }

    if(numberOverflow[1]==true){
        fprintf(E, "In cycle %d: Number Overflow\n", cycle);
    }

    return errorTemp;

}

