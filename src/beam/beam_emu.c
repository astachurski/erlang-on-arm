/*
 * beam_emu.c
 *
 *  Created on: Oct 26, 2013
 *      Author: Studnicki
 */

#include "global.h"
#include "beam_emu.h"
#include "beam_load.h"
#include "export.h"

int init_done = 0;

Eterm x0;
Eterm reg[X_REGS_ALLOCATED];
Eterm E[X_REGS_ALLOCATED];
Eterm tmp0, tmp1;
char buf[256];

void process_main(void* arg) {
	ErlProcess* p = (ErlProcess*)arg;
	//first time this function is called op labels from here are exported to the loader
	if(!init_done) {
		int i;
		void* temp[] = { JUMP_TABLE };
		for(i=0; i<154; i++) {
				jump_table_add(i, temp[i]);
		}
		init_done = 1;
		return;
	}

	Goto(*(p->i));

	OpCase(LABEL):
		//ignore
		debug("label\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(FUNC_INFO):
		//ignore
		debug("func_info\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(INT_CODE_END):
		debug("int_code_end\n");
		p->i +=1;
	OpCase(CALL):
		debug("call\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(CALL_LAST):
		debug("call_last\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(CALL_ONLY):
		debug("call_only\n");
		p->i = (BeamInstr*)(Arg(1));
		Goto(*(p->i));
	OpCase(CALL_EXT):
		debug("call_ext\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(CALL_EXT_LAST):
		debug("call_ext_last\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(BIF0):
		debug("bif0\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(BIF1):
		debug("bif1\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(BIF2):
		debug("bif2\n");
		p->i +=6;
		Goto(*(p->i));
	OpCase(ALLOCATE):
		debug("allocate\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(ALLOCATE_HEAP):
		debug("allocate_heap\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(ALLOCATE_ZERO):
		debug("allocate_zero\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(ALLOCATE_HEAP_ZERO):
		debug("allocate_heap_zero\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(TEST_HEAP):
		debug("test_heap\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(INIT):
		debug("init\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(DEALLOCATE):
		debug("deallocate\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(RETURN):
		do {} while(0);
		char buf[256];
		sprintf(buf, "%d\n", x0);
		debug(buf);
		p->i +=1;
		Goto(*(p->i));
	OpCase(SEND):
		debug("send\n");
		p->i +=1;
		Goto(*(p->i));
	OpCase(REMOVE_MESSAGE):
		debug("remove_message\n");
		p->i +=1;
		Goto(*(p->i));
	OpCase(TIMEOUT):
		debug("timeout\n");
		p->i +=1;
		Goto(*(p->i));
	OpCase(LOOP_REC):
		debug("loop_rec\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(LOOP_REC_END):
		debug("loop_rec_end\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(WAIT):
		debug("wait\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(WAIT_TIMEOUT):
		debug("wait_timeout\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(M_PLUS):
		debug("m_plus\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(M_MINUS):
		debug("m_minus\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(M_TIMES):
		debug("m_times\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(M_DIV):
		debug("m_div\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(INT_DIV):
		debug("int_div\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(INT_REM):
		debug("int_rem\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(INT_BAND):
		debug("int_band\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(INT_BOR):
		debug("int_bor\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(INT_BXOR):
		debug("int_bxor\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(INT_BSL):
		debug("int_bsl\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(INT_BSR):
		debug("int_bsr\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(INT_BNOT):
		debug("int_bnot\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(IS_LT):
		debug("is_lt\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(IS_GE):
		debug("is_ge\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(IS_EQ):
		debug("is_eq\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(IS_NE):
		debug("is_ne\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(IS_EQ_EXACT):
		debug("is_eq_exact\n");
		Resolve(Arg(1), tmp0);
		Resolve(Arg(2), tmp1);
		if(tmp0 == tmp1) {
			p->i +=4;
		}
		else {
			p->i = (BeamInstr*)(Arg(0));
		}
		Goto(*(p->i));
	OpCase(IS_NE_EXACT):
		debug("is_ne_exact\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(IS_INTEGER):
		debug("is_integer\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(IS_FLOAT):
		debug("is_float\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(IS_NUMBER):
		debug("is_number\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(IS_ATOM):
		debug("is_atom\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(IS_PID):
		debug("is_pid\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(IS_REFERENCE):
		debug("is_reference\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(IS_PORT):
		debug("is_port\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(IS_NIL):
		debug("is_nil\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(IS_BINARY):
		debug("is_binary\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(IS_CONSTANT):
		debug("is_constant\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(IS_LIST):
		debug("is_list\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(IS_NONEMPTY_LIST):
		debug("is_nonempty_list\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(IS_TUPLE):
		debug("is_tuple\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(TEST_ARITY):
		debug("test_arity\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(SELECT_VAL):
		debug("select_val\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(SELECT_TUPLE_ARITY):
		debug("select_tuple_arity\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(JUMP):
		debug("jump\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(CATCH):
		debug("catch\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(CATCH_END):
		debug("catch_end\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(MOVE):
		debug("move\n");
		Resolve(Arg(0), tmp1);
		Move(tmp1, Arg(1));
		p->i +=3;
		Goto(*(p->i));
	OpCase(GET_LIST):
		debug("get_list\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(GET_TUPLE_ELEMENT):
		debug("get_tuple_element\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(SET_TUPLE_ELEMENT):
		debug("set_tuple_element\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(PUT_STRING):
		debug("put_string\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(PUT_LIST):
		debug("put_list\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(PUT_TUPLE):
		debug("put_tuple\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(PUT):
		debug("put\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(BADMATCH):
		debug("badmatch\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(IF_END):
		debug("if_end\n");
		p->i +=1;
		Goto(*(p->i));
	OpCase(CASE_END):
		debug("case_end\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(CALL_FUN):
		debug("call_fun\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(MAKE_FUN):
		debug("make_fun\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(IS_FUNCTION):
		debug("is_function\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(CALL_EXT_ONLY):
		debug("call_ext_only\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(BS_START_MATCH):
		debug("bs_start_match\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(BS_GET_INTEGER):
		debug("bs_get_integer\n");
		p->i +=6;
		Goto(*(p->i));
	OpCase(BS_GET_FLOAT):
		debug("bs_get_float\n");
		p->i +=6;
		Goto(*(p->i));
	OpCase(BS_GET_BINARY):
		debug("bs_get_binary\n");
		p->i +=6;
		Goto(*(p->i));
	OpCase(BS_SKIP_BITS):
		debug("bs_skip_bits\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(BS_TEST_TAIL):
		debug("bs_test_tail\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(BS_SAVE):
		debug("bs_save\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(BS_RESTORE):
		debug("bs_restore\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(BS_INIT):
		debug("bs_init\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(BS_FINAL):
		debug("bs_final\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(BS_PUT_INTEGER):
		debug("bs_put_integer\n");
		p->i +=6;
		Goto(*(p->i));
	OpCase(BS_PUT_BINARY):
		debug("bs_put_binary\n");
		p->i +=6;
		Goto(*(p->i));
	OpCase(BS_PUT_FLOAT):
		debug("bs_put_float\n");
		p->i +=6;
		Goto(*(p->i));
	OpCase(BS_PUT_STRING):
		debug("bs_put_string\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(BS_NEED_BUF):
		debug("bs_need_buf\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(FCLEARERROR):
		debug("fclearerror\n");
		p->i +=1;
		Goto(*(p->i));
	OpCase(FCHECKERROR):
		debug("fcheckerror\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(FMOVE):
		debug("fmove\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(FCONV):
		debug("fconv\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(FADD):
		debug("fadd\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(FSUB):
		debug("fsub\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(FMUL):
		debug("fmul\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(FDIV):
		debug("fdiv\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(FNEGATE):
		debug("fnegate\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(MAKE_FUN2):
		debug("make_fun2\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(TRY):
		debug("try\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(TRY_END):
		debug("try_end\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(TRY_CASE):
		debug("try_case\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(TRY_CASE_END):
		debug("try_case_end\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(RAISE):
		debug("raise\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(BS_INIT2):
		debug("bs_init2\n");
		p->i +=7;
		Goto(*(p->i));
	OpCase(BS_BITS_TO_BYTES):
		debug("bs_bits_to_bytes\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(BS_ADD):
		debug("bs_add\n");
		p->i +=6;
		Goto(*(p->i));
	OpCase(APPLY):
		debug("apply\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(APPLY_LAST):
		debug("apply_last\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(IS_BOOLEAN):
		debug("is_boolean\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(IS_FUNCTION2):
		debug("is_function2\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(BS_START_MATCH2):
		debug("bs_start_match2\n");
		p->i +=6;
		Goto(*(p->i));
	OpCase(BS_GET_INTEGER2):
		debug("bs_get_integer2\n");
		p->i +=8;
		Goto(*(p->i));
	OpCase(BS_GET_FLOAT2):
		debug("bs_get_float2\n");
		p->i +=8;
		Goto(*(p->i));
	OpCase(BS_GET_BINARY2):
		debug("bs_get_binary2\n");
		p->i +=8;
		Goto(*(p->i));
	OpCase(BS_SKIP_BITS2):
		debug("bs_skip_bits2\n");
		p->i +=6;
		Goto(*(p->i));
	OpCase(BS_TEST_TAIL2):
		debug("bs_test_tail2\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(BS_SAVE2):
		debug("bs_save2\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(BS_RESTORE2):
		debug("bs_restore2\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(GC_BIF1):
		debug("gc_bif1\n");
		p->i +=6;
		Goto(*(p->i));
	OpCase(GC_BIF2):
		debug("gc_bif2\n");
		Export* e = (Export*)Arg(2);
		Resolve(Arg(3), tmp0);
		Resolve(Arg(4), tmp1);
		Eterm args[] = {tmp0, tmp1};
		Eterm result = (e->bif)(args);
		Move(result, Arg(5));
		p->i +=7;
		Goto(*(p->i));
	OpCase(BS_FINAL2):
		debug("bs_final2\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(BS_BITS_TO_BYTES2):
		debug("bs_bits_to_bytes2\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(PUT_LITERAL):
		debug("put_literal\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(IS_BITSTR):
		debug("is_bitstr\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(BS_CONTEXT_TO_BINARY):
		debug("bs_context_to_binary\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(BS_TEST_UNIT):
		debug("bs_test_unit\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(BS_MATCH_STRING):
		debug("bs_match_string\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(BS_INIT_WRITABLE):
		debug("bs_init_writable\n");
		p->i +=1;
		Goto(*(p->i));
	OpCase(BS_APPEND):
		debug("bs_append\n");
		p->i +=9;
		Goto(*(p->i));
	OpCase(BS_PRIVATE_APPEND):
		debug("bs_private_append\n");
		p->i +=7;
		Goto(*(p->i));
	OpCase(TRIM):
		debug("trim\n");
		p->i +=3;
		Goto(*(p->i));
	OpCase(BS_INIT_BITS):
		debug("bs_init_bits\n");
		p->i +=7;
		Goto(*(p->i));
	OpCase(BS_GET_UTF8):
		debug("bs_get_utf8\n");
		p->i +=6;
		Goto(*(p->i));
	OpCase(BS_SKIP_UTF8):
		debug("bs_skip_utf8\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(BS_GET_UTF16):
		debug("bs_get_utf16\n");
		p->i +=6;
		Goto(*(p->i));
	OpCase(BS_SKIP_UTF16):
		debug("bs_skip_utf16\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(BS_GET_UTF32):
		debug("bs_get_utf32\n");
		p->i +=6;
		Goto(*(p->i));
	OpCase(BS_SKIP_UTF32):
		debug("bs_skip_utf32\n");
		p->i +=5;
		Goto(*(p->i));
	OpCase(BS_UTF8_SIZE):
		debug("bs_utf8_size\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(BS_PUT_UTF8):
		debug("bs_put_utf8\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(BS_UTF16_SIZE):
		debug("bs_utf16_size\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(BS_PUT_UTF16):
		debug("bs_put_utf16\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(BS_PUT_UTF32):
		debug("bs_put_utf32\n");
		p->i +=4;
		Goto(*(p->i));
	OpCase(ON_LOAD):
		debug("on_load\n");
		p->i +=1;
		Goto(*(p->i));
	OpCase(RECV_MARK):
		debug("recv_mark\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(RECV_SET):
		debug("recv_set\n");
		p->i +=2;
		Goto(*(p->i));
	OpCase(GC_BIF3):
		debug("gc_bif3\n");
		p->i +=8;
		Goto(*(p->i));
	OpCase(LINE):
		//ignore for now
		p->i +=2;
		Goto(*(p->i));
}
