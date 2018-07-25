"""
TODO: doc
"""

 #  7 bits
OPCODE_SYSTEM    = 0x73
OPCODE_FENCE     = 0x0f
OPCODE_OP        = 0x33
OPCODE_OPIMM     = 0x13
OPCODE_STORE     = 0x23
OPCODE_LOAD      = 0x03
OPCODE_BRANCH    = 0x63
OPCODE_JALR      = 0x67
OPCODE_JAL       = 0x6f
OPCODE_AUIPC     = 0x17
OPCODE_LUI       = 0x37
OPCODE_OP_FP     = 0x53
OPCODE_OP_FMADD  = 0x43
OPCODE_OP_FNMADD = 0x4f
OPCODE_OP_FMSUB  = 0x47
OPCODE_OP_FNMSUB = 0x4b
OPCODE_STORE_FP  = 0x27
OPCODE_LOAD_FP   = 0x07

OPCODE_LOAD_POST  = 0x0b;
OPCODE_STORE_POST = 0x2b;
OPCODE_PULP_OP    = 0x5b;
OPCODE_VECOP      = 0x57;
OPCODE_HWLOOP     = 0x7b;

OPCODE_PULP_OP    = 0x5b;
def instr_len(instr):
    """Return the number of bytes used to encode instr"""
    if (instr & 0x3) != 0x3:  # RVC
        return 3
    elif (instr & 0x1f) != 0x1f:  # 32
        return 4
    elif (instr & 0x3f) == 0x1f:  # 48
        return 6
    elif (instr & 0x7f) == 0x7f:  # 64
        return 2
    else:
        return 2  # unimplemented
