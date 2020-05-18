
#define _ ,

#define DATA_SIZE sizeof(int)

#define TO_INT(PT) *((int *)(PT))

#define OUT_ADD(code) 

#define INC_PC ++PC

#define INC_OUT ++buf_out

#define ADD_DATA_SIZE(num) num += DATA_SIZE;

#define INC_BT {INC_PC; INC_OUT;}

#define ADD_BT {ADD_DATA_SIZE(PC) ADD_DATA_SIZE(buf_out)}

#define ADD_OUT ADD_DATA_SIZE(buf_out)

#define CODE(i) (code_ptr[PC + (i)])

#define OUT_CODE(i) (buf_out[i])

#define CUR_CODE (code_ptr[PC])

#define CUR_OUT_CODE (*buf_out)

#define PUT_OUT(CMD) CUR_OUT_CODE = CMD; INC_OUT;

//---------------------------------------------------------------
// IMPORTANT !!!!
// adasm              x86
// ax                 esi
// bx                 ebx
// cx                 edi
// dx                 edx
// --                eax, ecx - for operations
//---------------------------------------------------------------



DEF_CMD(PUSH, 1, 
  {
    PUT_OUT(PUSH);
    TO_INT(buf_out) = TO_INT(code_ptr + PC);
    ADD_BT
  },
  {
    ADD_DATA_SIZE(PC)
  })

#define CHECK_REG(nm, eax, ebx, ecx, edx)                                                       \
{                                                                                               \
    char add = 0;                                                                              \
    switch (CODE(-1))                                                                          \
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
    INC_BT                                                                                      \
}


DEF_CMD(PUSH_REG, 11,
  {
    CUR_OUT_CODE = PUSH_EAX;
    CHECK_REG(push, 6, 7, 1, 2);
  },
  {
    INC_PC;
  })

DEF_CMD(PUSH_RAM, 12,
  {
  },
  {
    INC_PC;
  })
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
    PUT_OUT(POP_EAX);
    PUT_OUT(POP_ECX);
    PUT_OUT(ADD_EAX_ECX);
    PUT_OUT(PUSH_EAX);
  },
  {
  })
//  pop eax - 
//  pop ecx - special for operations
//  add eax, ecx
//  push eax


DEF_CMD(POP, 3,
  {
    CUR_OUT_CODE = POP_EAX;
    CHECK_REG(pop, 6, 7, 1, 2)
  },
  {
    ADD_DATA_SIZE(PC);
  })


DEF_CMD(POP_RAM, 31,
  {

  },
  {
    INC_PC;
  })
#undef PUT_RAM

// pop eax
// mov dword ptr [reg], eax

DEF_CMD(SUB, 4,
  {
    PUT_OUT(POP_EAX);
    PUT_OUT(POP_ECX);
    PUT_OUT(SUB_EAX_ECX);
    PUT_OUT(PUSH_EAX);
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
    PUT_OUT(POP_EAX);
    PUT_OUT(POP_ECX);
    PUT_OUT(MUL_ECX);
    PUT_OUT(PUSH_EAX);
  },
  {})
// pop eax
// pop ecx
// mul ecx
// push eax
//


DEF_CMD(DIV, 6,
  {
    PUT_OUT(POP_EAX);
    PUT_OUT(POP_ECX);
    PUT_OUT(PUSH_EDX);
    PUT_OUT(XOR_EDX_EDX);
    PUT_OUT(DIV_ECX);
    PUT_OUT(POP_EDX);
    PUT_OUT(PUSH_EAX);
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

#define JMP_FILL_TBL                                        \
  TABLE.PushOldInf(&code_ptr[PC - 1], code_ptr + code_ptr[PC]);     \
  PC += sizeof(int);

DEF_CMD(JMP, 20,
{
  PUT_OUT(JMP_REL);
  ADD_OUT;
},
{
  JMP_FILL_TBL;
})

// jmp 
//
//

#define DEF_JMP(name, cde)
DEF_CMD(name, cde,
  {

  },
  {
    JMP_FILL_TBL;
  }
)

#if 0
DEF_JMP(JA, 21, first > second)
// pop eax
// pop ecx
// cmp eax, ecx
// ja ...
//


DEF_JMP(JAE, 22, first >= second)
// pop eax
// pop ecx
// cmp eax, ecx
// jae ..


DEF_JMP(JB, 23, first < second)
// pop eax
// pop ecx
// cmp eax, ecx
// jb ...


DEF_JMP(JBE, 24, first <= second)
// pop eax
// pop ecx
// cmp eax, ecx
// jbe .. 

DEF_JMP(JE, 25, first == second)
// pop eax
// pop ecx
// cmp eax, ebx
// je ..

DEF_JMP(JNE, 26, first != second)
// pop eax
// pop ecx
// cmp
// jne ...
#endif

DEF_CMD(CALL, 40,
  {
  },
  {
    JMP_FILL_TBL;
  })
// call ...
#undef JMP_FILL_TBL

DEF_CMD(RET, 41,
  {
  },
  {})
  // ret

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
  // push 0
  // call ExitProcess

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

#undef DEST_PROCESS