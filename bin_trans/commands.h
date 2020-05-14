
#define _ ,

#define DATA_SIZE sizeof(int)

#define TO_INT(PT) *((int *)(PT))


//---------------------------------------------------------------
// IMPORTANT !!!!
// myproc             x86
// ax                 esi
// bx                 ebx
// cx                 edi
// dx                 edx
// --                eax, ecx - for operations
//---------------------------------------------------------------
DEF_CMD(PUSH, 1,
  {
    *buf_out++ = 0x68;
    TO_INT(buf_out) = TO_INT(code_ptr + PC);
    PC += sizeof(int);
    buf_out += sizeof(int);
  },
  {})

#define CHECK_REG(nm, eax, ebx, ecx, edx)                                                       \
{                                                                                               \
    char add = 0;                                                                              \
    switch (*code_ptr)                                                                          \
    {                                                                                           \
    case 1:                                                                                     \
      add = eax;                                                                                \
      break;                                                                                    \
    case 2:                                                                                     \
      add = ebx;                                                                                \
      break;                                                                                    \
    case 3:                                                                                     \
      add = ecx;                                                                                \
      break;                                                                                    \
    case 4:                                                                                     \
      add = edx;                                                                                \
      break;                                                                                    \
    default:                                                                                    \
      printf("Unrecognized register to %s : %u\nPosition %zd", #nm, (unsigned)*code_ptr, PC);    \
      break;                                                                                    \
    }                                                                                           \
    *buf_out += add;                                                                            \
    ++buf_out;                                                                                  \
}


DEF_CMD(PUSH_REG, 11,
  {
    *buf_out = 0x50;
    CHECK_REG(push, 6, 7, 1, 2)
  },
  {})

DEF_CMD(PUSH_RAM, 12,
  {
  },
  {})
//  push [ax] or [bx] or [cx] or [dx] -> get from memory by reg location and push
//  --------------x86----------------
//  Somewhere in data  section:
//  buf           dd 0 dup(RAM_SIZE)
//  ---------------------------------
//  
//  mov eax, dword ptr [reg] -> 
//

DEF_CMD(ADD, 2,
  {
    
  },
  {})
//  pop eax - 
//  pop ecx - special for operations
//  add eax, ecx
//  push eax


DEF_CMD(POP, 3,
  {
    *buf_out = 0x58;
    CHECK_REG(pop, 6, 7, 1, 2)
  },
  {})
//  just pop num
//
//


DEF_CMD(POP_RAM, 31,
  {

  },
  {})
#undef PUT_RAM

// pop eax
// mov dword ptr [reg], eax

DEF_CMD(SUB, 4,
  {
  },
  {})

//  pop eax - 
//  pop ecx - special for operations
//  sub eax, ecx
//  push eax
//
//
//  
DEF_CMD(MUL, 5,
  {
  },
  {})
// pop eax
// pop ecx
// mul ecx
// push eax
//


DEF_CMD(DIV, 6,
  {
  },
  {})

// pop eax
// pop ecx
// push edx
// xor edx, edx
// div ecx
// pop edx
// push eax

DEF_CMD(OUT, 7,
  {
  },
  {})

// call ToDec
// 
//

DEF_CMD(SQRT, 8, 
  {
  },
  {})

DEF_CMD(SIN, 9,
  {
  },
  {})

DEF_CMD(COS, 10,
  {
  },
  {})



DEF_CMD(JMP, 20,
{
},
  {})

// jmp 
//
//

#if 0
DEF_JMP(JA, 21, first > second)

DEF_JMP(JAE, 22, first >= second)

DEF_JMP(JB, 23, first < second)

DEF_JMP(JBE, 24, first <= second)

DEF_JMP(JE, 25, first == second)

DEF_JMP(JNE, 26, first != second)
#endif

DEF_CMD(CALL, 40,
  {
  },
  {})

DEF_CMD(RET, 41,
  {
  },
  {})


    DEF_CMD(IN, 42,
  {
  },
  {})

DEF_CMD(POW, 43,
  {
  },
  {})

DEF_CMD(END, -1,
  {
  },
  {})


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