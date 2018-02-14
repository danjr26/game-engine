#ifndef OCT_TREE_H
#define OCT_TREE_H
#include "component.h"
#include "vectors.h"

union OctInfo {
	struct {
		uint	nodecode;
		ushort	ncomponents;
		ushort	nchildren;
	} node;
	CollidableComponent* data;
};

class OctTree {
public:
	OctInfo	structure[1024];
	uint	depth;
	uint	length;

			OctTree			(uint depth);
			~OctTree		();
	void	Refresh		();
	uint	Calculate_Level	(CollidableComponent** read1, uint nread1, CollidableComponent** read2, uint nread2, uint writei, uint depthcount);
};
/*
class OctTreeCoords {
public:
	OctTree*	parent;
	char*		coords;

			OctTreeCoords	(OctTree* parent, Vector3d position, float r);
			~OctTreeCoords	();
	void	New_Coords		(Vector3d position, float r);
	bool	operator<		(OctTreeCoords& c);
	bool	operator>		(OctTreeCoords& c);
	bool	operator<=		(OctTreeCoords& c);
	bool	operator>=		(OctTreeCoords& c);
	bool	operator==		(OctTreeCoords& c);
	bool	operator!=		(OctTreeCoords& c);
};
*/
#endif
