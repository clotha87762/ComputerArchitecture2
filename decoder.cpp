#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "decoder.h"
#include "simulator.h"
extern simulator sim;
using namespace std;
void decodeData(FILE* F){


    int i=0;
    int status;

    unsigned int data_temp=0;
    unsigned char byte;

    for(i=0;i<4;i++){
        status = fread(&byte,sizeof(char),1,F);
        data_temp = data_temp|(byte<<(8*(3-i)));
    }
   // printf("0x%08X\n",data_temp);
    //cout<<tmp<<endl;
    for(int q=0;q<5;q++)
    sim.reg[q][29]=data_temp;

    data_temp=0;

    for(i=0;i<4;i++){
        status = fread(&byte,sizeof(char),1,F);
        data_temp = data_temp|(byte<<(8*(3-i)));
    }
    sim.D_number = data_temp;
    sim.D_number_byte=data_temp*4;
   // printf("0x%08X\n",data_temp);

    i=0;

    while(i<sim.D_number_byte){

        status = fread(&byte,sizeof(char),1,F);
        if(status){

            sim.D_mem_byte[i++] = byte;

        }
    }

    int j=0;
    for(i=0;i<sim.D_number_byte;i+=4){
        sim.D_mem[j++] = (sim.D_mem_byte[i]<<24)|(sim.D_mem_byte[i+1]<<16)| (sim.D_mem_byte[i+2]<<8) | (sim.D_mem_byte[i+3]);
    }
}

void decodeInstructions(FILE* F){

    int i=0;
    int status;
    unsigned int ins_temp;
    unsigned char byte;
    unsigned int pctemp=0;

    for(i=0;i<4;i++){
        status = fread(&byte,sizeof(char),1,F);
        pctemp = pctemp|(byte<<(8*(3-i)));
    }
    //printf("0x%08X\n",pctemp);

    sim.PC = pctemp;
    int inum = 0;


    for(i=0;i<4;i++){
        status = fread(&byte,sizeof(char),1,F);
        inum= inum|(byte<<(8*(3-i)));
    }

    //printf("0x%08X\n",inum);
    sim.I_number = inum;
    sim.I_number_byte = inum*4;


    i=pctemp;
    int p = 0;
    while(p++<sim.I_number_byte){

        status = fread(&byte,sizeof(char),1,F);
        if(status){
            sim.I_mem_byte[i++] = byte;
        }

    }

    int j=pctemp/4;
    for(i=pctemp;i<sim.I_number_byte+pctemp;i+=4){
        sim.I_mem[j++] = (sim.I_mem_byte[i]<<24)|(sim.I_mem_byte[i+1]<<16)| (sim.I_mem_byte[i+2]<<8) | (sim.I_mem_byte[i+3]);

    }

}


