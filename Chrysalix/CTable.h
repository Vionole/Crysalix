#pragma once
#include <map>
#include <string>

using namespace std;

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Таблица соответствий
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct CTable {
    map<wstring, OP_CODE> opCodeMap = {
        {L"NOP", OP_CODE::NOP}, {L"nop", OP_CODE::NOP},
        {L"END", OP_CODE::END}, {L"end", OP_CODE::END},
        {L"PAUSE", OP_CODE::PAUSE}, {L"pause", OP_CODE::PAUSE},
        {L"SLEEP", OP_CODE::SLEEP}, {L"sleep", OP_CODE::SLEEP},
        {L"VAR", OP_CODE::VAR}, {L"var", OP_CODE::VAR},
        {L"PRINT", OP_CODE::PRINT}, {L"print", OP_CODE::PRINT},
        {L"FREE", OP_CODE::FREE}, {L"free", OP_CODE::FREE},
        {L"LABEL", OP_CODE::LABEL}, {L"label", OP_CODE::LABEL},
        {L"JUMP", OP_CODE::JUMP}, {L"jump", OP_CODE::JUMP},
        {L"INPUT", OP_CODE::INPT}, {L"input", OP_CODE::INPT},
        {L"INPUT", OP_CODE::INPT}, {L"input", OP_CODE::INPT},
        {L"CHANGE", OP_CODE::CHANGE}, {L"change", OP_CODE::CHANGE},
        {L"TO", OP_CODE::TO}, {L"to", OP_CODE::TO},
        {L"CALC", OP_CODE::CALC}, {L"calc", OP_CODE::CALC},
        {L"NEWTEMP", OP_CODE::NEWTEMP}, {L"newtemp", OP_CODE::NEWTEMP},
        {L"FORGET", OP_CODE::FORGET}, {L"forget", OP_CODE::FORGET},
        {L"TCOUNT", OP_CODE::TCOUNT}, {L"tcount", OP_CODE::TCOUNT},
        {L"ISSET", OP_CODE::ISSET}, {L"isset", OP_CODE::ISSET},
        {L"TYPEOF", OP_CODE::TYPEOF}, {L"typeof", OP_CODE::TYPEOF},
        {L"COMP", OP_CODE::COMP}, {L"comp", OP_CODE::COMP},
        {L"LOGIC", OP_CODE::LOGIC}, {L"logic", OP_CODE::LOGIC},
        {L"JIF", OP_CODE::JIF}, {L"jif", OP_CODE::JIF},
        {L"JIFNOT", OP_CODE::JIFNOT}, {L"jifnot", OP_CODE::JIFNOT},
        {L"DLABEL", OP_CODE::DLABEL}, {L"dlabel", OP_CODE::DLABEL},
        {L"SWAP", OP_CODE::SWAP}, {L"swap", OP_CODE::SWAP},
        {L"ARRAY", OP_CODE::ARRAY}, {L"array", OP_CODE::ARRAY},
        {L"VTOARR", OP_CODE::VTOARR}, {L"vtoarr", OP_CODE::VTOARR},
        {L"PUSHB", OP_CODE::PUSHB}, {L"pushb", OP_CODE::PUSHB},
        {L"POPB", OP_CODE::POPB}, {L"popb", OP_CODE::POPB},
        {L"PUSHF", OP_CODE::PUSHF}, {L"pushf", OP_CODE::PUSHF},
        {L"POPF", OP_CODE::POPF}, {L"popf", OP_CODE::POPF},
        {L"ERASE", OP_CODE::ERASE}, {L"erase", OP_CODE::ERASE},
        {L"INSERT", OP_CODE::INSRT}, {L"insert", OP_CODE::INSRT},
        {L"CLEAR", OP_CODE::CLEAR}, {L"clear", OP_CODE::CLEAR},
        {L"SIZE", OP_CODE::SIZEARR}, {L"size", OP_CODE::SIZEARR},
        {L"GETVAL", OP_CODE::GETVAL}, {L"getval", OP_CODE::GETVAL},
        {L"SETVAL", OP_CODE::SETVAL}, {L"setval", OP_CODE::SETVAL},
    };
};