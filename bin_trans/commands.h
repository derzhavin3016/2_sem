
#define _ ,

#define DATA_SIZE sizeof(int)

#define CODE_INT *((int *)code_ptr)

DEF_CMD(PUSH, 1,
  {
  })

DEF_CMD(PUSH_REG, 11,
  {
  })

DEF_CMD(PUSH_RAM, 12,
  {
  })


DEF_CMD(ADD, 2,
  {
    
  })

DEF_CMD(POP, 3,
  {
  })

DEF_CMD(POP_RAM, 31,
  {
  })
#undef PUT_RAM

DEF_CMD(SUB, 4,
  {
  })

DEF_CMD(MUL, 5,
  {
  })

DEF_CMD(DIV, 6,
  {
  })

DEF_CMD(OUT, 7,
  {})

DEF_CMD(SQRT, 8, 
  {
  })

DEF_CMD(SIN, 9,
  {
  })

DEF_CMD(COS, 10,
  {
  })



DEF_CMD(JMP, 20,
{
})

DEF_JMP(JA, 21, first > second)

DEF_JMP(JAE, 22, first >= second)

DEF_JMP(JB, 23, first < second)

DEF_JMP(JBE, 24, first <= second)

DEF_JMP(JE, 25, first == second)

DEF_JMP(JNE, 26, first != second)

DEF_CMD(CALL, 40,
  {
  })

DEF_CMD(RET, 41, 1,
  {
  })


    DEF_CMD(IN, 42,
  {
  })

DEF_CMD(POW, 43,
  {
  })

DEF_CMD(END, -1,
  {
  })


#undef _

#undef DATA_SIZE

#undef STK_PUSH

#undef STK_POP
 
#undef GET_FST_SEC_VALUES

#undef CODE_INT

#undef PUT_REG

#undef JMP_DISASM

#undef JMP_ASM

#undef DEF_JMP