#ifndef simulator_h
#define simulator_h
#endif
#include<iostream>


enum instruction_name {ADD,SUB,AND,OR,XOR,NOR,NAND,SLT,SLL,SRL,SRA,JR,ADDI,LW,LH,LHU,LB,LBU,
                        SW,SH,SB,LUI,ANDI,ORI,NORI,SLTI,BEQ,BNE,J,JAL,HALT,NOP};
enum type_name{Rtype,Itype,Jtype};

struct RInstruction{
    unsigned int opcode;
    unsigned int rs;
    unsigned int rt;
    unsigned int rd;
    unsigned int shamt;
    unsigned int funct;
};

struct IInstruction{
    unsigned int opcode;
    unsigned int rs;
    unsigned int rt;
    short immediate;
};

struct JInstruction{
    unsigned int opcode;
    unsigned int address;
};
typedef struct RInstruction Rinstruction;
typedef struct IInstruction Iinstruction;
typedef struct JInstruction Jinstruction;

class simulator{


    public :
    simulator();

    unsigned int PC;
    unsigned int PCtemp;
    int cycle;
    unsigned int I_mem[256];
    unsigned char I_mem_byte[1024];
    unsigned  char D_mem_byte[1024];
    unsigned  int D_mem[256];
    int reg[5][32];
    bool writeTo0[4];
    bool numberOverflow[4];
    bool misalignment[4];
    bool addressOverflow[4];

    bool branch;
    int I_number;
    int D_number;
    int I_number_byte;
    int D_number_byte;
    instruction_name nowOp;
    instruction_name ppName[4];
    type_name nowType;
    int opcode;
    int rs[4];
    int rt[4];
    int rd[4];
    unsigned int Instemp[4];

    bool skipCycle;
    bool errorHalt;
    bool IFstall,IDstall,IFflush,ID_FWD_rs,ID_FWD_rt,EX_FWD_EXMEM_rs,EX_FWD_MEMWB_rs,EX_FWD_EXMEM_rt,EX_FWD_MEMWB_rt;


    Rinstruction r_instruction;
    Iinstruction i_instruction;
    Jinstruction j_instruction;

    void add_Instruction();
    void sub_Instruction();
    void and_Instruction();
    void or_Instruction();
    void xor_Instruction();
    void nor_Instruction();
    void nand_Instruction();
    void slt_Instruction();
    void sll_Instruction();
    void srl_Instruction();
    void sra_Instruction();
    void jr_Instruction();
    void addi_Instruction();
    void lw_Instruction();
    void lh_Instruction();
    void lhu_Instruction();
    void lb_Instruction();
    void lbu_Instruction();
    void sw_Instruction();
    void sh_Instruction();
    void sb_Instruction();
    void lui_Instruction();
    void andi_Instruction();
    void ori_Instruction();
    void nori_Instruction();
    void slti_Instruction();
    void beq_Instruction();
    void bne_Instruction();
    void j_Instruction();
    void jal_Instruction();
    void halt_Instruction();

    void PrintReg(FILE* f,unsigned int u);
    void decodeInstruction();
    void afterInstruction();
    void ExecuteInstruction(FILE* E);
    void decodeIns();

   void forwardDetect();
   void stallDetect();
   void shiftPipeline();
    char* printIns(instruction_name in);
   bool printError(FILE* E);

    void Deal_WritetoReg0(FILE* E);
    void Deal_NumberOverflow(FILE* E);
    void Deal_MemoryAddOverflow(FILE* E);
    void Deal_DataMisaligned(FILE* E);
};


