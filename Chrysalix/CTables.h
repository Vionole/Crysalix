#pragma once
#include <map>
#include <string>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Опкоды инструкций
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum OP_CODE {
	NOP,
	END,
	PAUSE,
	SLEEP,
	VAR,
	PRINT,
	FREE,
	LABEL,
	JUMP,
	INPT,
	CHANGE,
	TO,
	CALC,
	NEWTEMP,
	FORGET,
	TCOUNT,
	ISSET,
	TYPEOF,
	COMP,
	LOGIC,
	JIF,
	JIFNOT,
	DLABEL,
	SWAP,
	ARRAY,
	VTOARR,
	PUSHB,
	POPB,
	PUSHF,
	POPF,
	ERASE,
	INSRT,
	CLEAR,
	SIZEARR,
	GETVAL,
	SETVAL
};


/*map<string, OP_CODE> opcodeMap; /*= {
	{L"NOP", OP_CODE::NOP}, {L"nop", OP_CODE::NOP},
	{L"END", OP_CODE::END}, {L"end", OP_CODE::END},
    {L"PAUSE", OP_CODE::PAUSE}, {L"pause", OP_CODE::PAUSE},
    {L"SLEEP", OP_CODE::SLEEP}, {L"sleep", OP_CODE::SLEEP},
    {L"SLEEP", OP_CODE::SLEEP}, {L"sleep", OP_CODE::SLEEP},
};*/
/*
else if (lexeme.type == L"VAR" || lexeme.type == L"var") {
    inst.opCode = VAR;
    inst.as_string = L"VAR: ";
}
else if (lexeme.type == L"PRINT" || lexeme.type == L"print") {
    inst.opCode = PRINT;
    inst.as_string = L"PRINT: ";
}
else if (lexeme.type == L"FREE" || lexeme.type == L"free") {
    inst.opCode = FREE;
    inst.as_string = L"FREE: ";
}
else if (lexeme.type == L"LABEL" || lexeme.type == L"label") {
    inst.opCode = LABEL;
    inst.as_string = L"LABEL: ";
}
else if (lexeme.type == L"JUMP" || lexeme.type == L"jump") {
    inst.opCode = JUMP;
    inst.as_string = L"JUMP: ";
}
else if (lexeme.type == L"INPUT" || lexeme.type == L"input") {
    inst.opCode = INPT;
    inst.as_string = L"INPUT: ";
}
else if (lexeme.type == L"CHANGE" || lexeme.type == L"change") {
    inst.opCode = CHANGE;
    inst.as_string = L"CHANGE: ";
}
else if (lexeme.type == L"TO" || lexeme.type == L"to") {
    inst.opCode = TO;
    inst.as_string = L"TO: ";
}
else if (lexeme.type == L"CALC" || lexeme.type == L"calc") {
    inst.opCode = CALC;
    inst.as_string = L"CALC: ";
}
else if (lexeme.type == L"NEWTEMP" || lexeme.type == L"newtemp") {
    inst.opCode = NEWTEMP;
    inst.as_string = L"NEWTEMP: ";
}
else if (lexeme.type == L"FORGET" || lexeme.type == L"forget") {
    inst.opCode = FORGET;
    inst.as_string = L"FORGET: ";
}
else if (lexeme.type == L"TCOUNT" || lexeme.type == L"tcount") {
    inst.opCode = TCOUNT;
    inst.as_string = L"TCOUNT: ";
}
else if (lexeme.type == L"ISSET" || lexeme.type == L"isset") {
    inst.opCode = ISSET;
    inst.as_string = L"ISSET: ";
}
else if (lexeme.type == L"TYPEOF" || lexeme.type == L"typeof") {
    inst.opCode = TYPEOF;
    inst.as_string = L"TYPEOF: ";
}
else if (lexeme.type == L"COMP" || lexeme.type == L"comp") {
    inst.opCode = COMP;
    inst.as_string = L"COMP: ";
}
else if (lexeme.type == L"LOGIC" || lexeme.type == L"logic") {
    inst.opCode = LOGIC;
    inst.as_string = L"LOGIC: ";
}
else if (lexeme.type == L"JIF" || lexeme.type == L"jif") {
    inst.opCode = JIF;
    inst.as_string = L"JIF: ";
}
else if (lexeme.type == L"JIFNOT" || lexeme.type == L"jifnot") {
    inst.opCode = JIFNOT;
    inst.as_string = L"JIFNOT: ";
}
else if (lexeme.type == L"DLABEL" || lexeme.type == L"dlabel") {
    inst.opCode = DLABEL;
    inst.as_string = L"DLABEL: ";
}
else if (lexeme.type == L"SWAP" || lexeme.type == L"swap") {
    inst.opCode = SWAP;
    inst.as_string = L"SWAP: ";
}
else if (lexeme.type == L"ARRAY" || lexeme.type == L"array") {
    inst.opCode = ARRAY;
    inst.as_string = L"ARRAY: ";
}
else if (lexeme.type == L"VTOARR" || lexeme.type == L"vtoarr") {
    inst.opCode = VTOARR;
    inst.as_string = L"VTOARR: ";
}
else if (lexeme.type == L"PUSHB" || lexeme.type == L"pushb") {
    inst.opCode = PUSHB;
    inst.as_string = L"PUSHB: ";
}
else if (lexeme.type == L"POPB" || lexeme.type == L"popb") {
    inst.opCode = POPB;
    inst.as_string = L"POPB: ";
}
else if (lexeme.type == L"PUSHF" || lexeme.type == L"pushf") {
    inst.opCode = PUSHF;
    inst.as_string = L"PUSHF: ";
}
else if (lexeme.type == L"POPF" || lexeme.type == L"popf") {
    inst.opCode = POPF;
    inst.as_string = L"POPF: ";
}
else if (lexeme.type == L"ERASE" || lexeme.type == L"erase") {
    inst.opCode = ERASE;
    inst.as_string = L"ERASE: ";
}
else if (lexeme.type == L"INSERT" || lexeme.type == L"insert") {
    inst.opCode = INSRT;
    inst.as_string = L"INSERT: ";
}
else if (lexeme.type == L"CLEAR" || lexeme.type == L"clear") {
    inst.opCode = CLEAR;
    inst.as_string = L"CLEAR: ";
}
else if (lexeme.type == L"SIZE" || lexeme.type == L"size") {
    inst.opCode = SIZEARR;
    inst.as_string = L"SIZE: ";
}
else if (lexeme.type == L"GETVAL" || lexeme.type == L"getval") {
    inst.opCode = GETVAL;
    inst.as_string = L"GETVAL: ";
}
else if (lexeme.type == L"SETVAL" || lexeme.type == L"setval") {
    inst.opCode = SETVAL;
    inst.as_string = L"SETVAL: ";
}
*/