#include "extra_mesh_vertex_datum.h"
#include "exceptions.h"

uint ExtraMeshVertexDatum::Get_Data_Type_Size() const {
	switch (dataType) {
	case _byte: return sizeof(byte);
	case _ubyte: return sizeof(ubyte);
	case _short: return sizeof(short);
	case _ushort: return sizeof(ushort);
	case _int: return sizeof(int);
	case _uint: return sizeof(uint);
	case _float: return sizeof(float);
	case _double: return sizeof(double);
	default: throw InvalidArgumentException("invalid data type");
	}
}
