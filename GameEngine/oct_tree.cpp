#include "oct_tree.h"
#include "framework.h"

OctTree::OctTree(uint depth) :
depth(depth) {
	for (int i = 0; i < 1024; i++)
		structure[i].data = nullptr;
}

OctTree::~OctTree() 
{}

void OctTree::Refresh() {
	IndexedArray<CollidableComponent3>& components = GEngine::Get().Collision().components;
	for (int i = 0; i < 1024; i++)
		structure[i].data = nullptr;
	length = Calculate_Level(components.Pointer(), components.Size(), nullptr, 0, 0, 0);
}

uint OctTree::Calculate_Level(CollidableComponent3** read1, uint nread1, CollidableComponent3** read2, uint nread2, uint writei, uint depthcount) {
	if (writei + 1 + nread1 + nread2 >= 1024)
		Die("OctTree.Calculate_Level: overflow");
	OctInfo node;
	node.node.nodecode = 0xf0f0f0f0;
	node.node.ncomponents = nread1 + nread2;

	Vector3d mean = Vector3d();
	uint counts[9];

	for (uint i = 0; i < 9; i++)
		counts[i] = 0;

	for (uint i = 0; i < nread1; i++)
		mean += read1[i]->mask->transform->position;
	for (uint i = 0; i < nread2; i++)
		mean += read2[i]->mask->transform->position;
	mean /= nread1 + nread2;

	uchar bins[512];
	for (uint i = 0; i < nread1; i++) {
		bins[i] = 0;
		if (read1[i]->mask->transform->position.x < mean.x - read1[i]->mask->tree->trunk->radius)
			bins[i] |= 1;
		else {
			if (read1[i]->mask->transform->position.x < mean.x + read1[i]->mask->tree->trunk->radius) {
				bins[i] |= 8;
			}
		}
		if (read1[i]->mask->transform->position.y < mean.y- read1[i]->mask->tree->trunk->radius)
			bins[i] |= 2;
		else {
			if (read1[i]->mask->transform->position.y < mean.y + read1[i]->mask->tree->trunk->radius) {
				bins[i] |= 8;
			}
		}
		if (read1[i]->mask->transform->position.z < mean.z - read1[i]->mask->tree->trunk->radius)
			bins[i] |= 4;
		else {
			if (read1[i]->mask->transform->position.z < mean.z + read1[i]->mask->tree->trunk->radius) {
				bins[i] |= 8;
			}
		}

		if (bins[i] > 8)
			bins[i] = 8;

		counts[bins[i]]++;
	}

	for (uint i = 0; i < nread1; i++) {
		if (bins[i] > 8)
			Die("");
	}

	for (uint i = 0; i < nread2; i++) {
		bins[i + nread1] = 0;
		if (read2[i]->mask->transform->position.x < mean.x - read2[i]->mask->tree->trunk->radius)
			bins[i + nread1] |= 1;
		else {
			if (read2[i]->mask->transform->position.x < mean.x + read2[i]->mask->tree->trunk->radius) {
				bins[i + nread1] |= 8;
			}
		}
		if (read2[i]->mask->transform->position.y < mean.y - read2[i]->mask->tree->trunk->radius)
			bins[i + nread1] |= 2;
		else {
			if (read2[i]->mask->transform->position.y < mean.y + read2[i]->mask->tree->trunk->radius) {
				bins[i + nread1] |= 8;
			}
		}
		if (read2[i]->mask->transform->position.z < mean.z - read2[i]->mask->tree->trunk->radius)
			bins[i + nread1] |= 4;
		else {
			if (read2[i]->mask->transform->position.z < mean.z + read2[i]->mask->tree->trunk->radius) {
				bins[i + nread1] |= 8;
			}
		}

		if (bins[i + nread1] > 8)
			bins[i + nread1] = 8;

		counts[bins[i + nread1]]++;
	}

	for (uint i = 0; i < nread1 + nread2; i++) {
		if (bins[i] > 8)
			Die("");
	}

	uint binoffsets[9];
	for (uint i = 0; i < 9; i++) {
		binoffsets[i] = 0;
		for (int k = i - 1; k >= 0; k--)
			binoffsets[i] += counts[k];
	}

	uint counts2[9];
	node.node.nchildren = 0;
	for (uint i = 0; i < 9; i++) {
		if (counts[i] != 0)
			node.node.nchildren++;
		counts2[i] = 0;
	}

	structure[writei] = node;
	writei++;

	OctInfo datum;

	for (uint i = 0; i < nread1; i++) {
		datum.data = read1[i];
		structure[writei + binoffsets[bins[i]] + counts2[bins[i]]] = datum;
		counts2[bins[i]]++;
	}

	for (uint i = 0; i < nread2; i++) {
		datum.data = read2[i];
		structure[writei + binoffsets[bins[i + nread1]] + counts2[bins[i + nread1]]] = datum;
		counts2[bins[i + nread1]]++;
	}

	for (uint i = 0; i < nread1 + nread2; i++) {
		if (bins[i] > 8)
			Die("");
	}

	for (uint i = 0; i < 9; i++) {
		if (counts[i] != counts2[i])
			Die("OctTree.Calculate_Level: count mismatch");
	}

	uint thiswrite = writei;
	writei += nread1 + nread2;

	if(depthcount == depth - 1)
		return writei;

	for (int i = 0; i < 8; i++)
		writei = Calculate_Level(
			&structure[thiswrite + binoffsets[i]].data, counts[i], 
			&structure[thiswrite + binoffsets[8]].data, counts[8], 
			writei,  depthcount + 1);

	return writei;
}
/*
OctTreeCoords::OctTreeCoords(OctTree* parent, Vector3d position, float r)
: parent(parent) {
	coords = new char[parent->depth];
	New_Coords(position, r);
}

OctTreeCoords::~OctTreeCoords() {
	delete[] coords;
}

void OctTreeCoords::New_Coords(Vector3d position, float r) {
	Vector3d high = parent->dim / 2.0f;
	Vector3d low = high * -1;
	Vector3d mid = Vector3d();
	for (int i = 0; i < parent->depth; i++) {
		coords[i] = 0;
		if (position.x >= mid.x) {
			if (position.x - r >= mid.x && position.x + r <= high.x) {
				coords[i] |= OCT_X_POS;
				low.x = mid.x;
			}
			else {
				coords[i] = OCT_NONE;
				break;
			}
		}
		else {
			if (position.x - r >= low.x && position.x + r <= mid.x) {
				coords[i] |= OCT_X_POS;
				high.x = mid.x;
			}
			else {
				coords[i] = OCT_NONE;
				break;
			}
		}
		if (position.y >= mid.y) {
			if (position.y - r >= mid.y && position.y + r <= high.y) {
				coords[i] |= OCT_Y_POS;
				low.y = mid.y;
			}
			else {
				coords[i] = OCT_NONE;
				break;
			}
		}
		else {
			if (position.y - r >= low.y && position.y + r <= mid.y) {
				coords[i] |= OCT_Y_POS;
				high.y = mid.y;
			}
			else {
				coords[i] = OCT_NONE;
				break;
			}
		}
		if (position.z >= mid.z) {
			if (position.z - r >= mid.z && position.z + r <= high.z) {
				coords[i] |= OCT_Z_POS;
				low.z = mid.z;
			}
			else {
				coords[i] = OCT_NONE;
				break;
			}
		}
		else {
			if (position.z - r >= low.z && position.z + r <= mid.z) {
				coords[i] |= OCT_Z_POS;
				high.z = mid.z;
			}
			else {
				coords[i] = OCT_NONE;
				break;
			}
		}
		mid = (high + low) / 2.0f;
	}
}

bool OctTreeCoords::operator<(OctTreeCoords& c) {
	for (int i = 0; i < parent->depth; i++) {
		if (this->coords[i] & OCT_NONE || c.coords[i] & OCT_NONE)
			return false;
		if (this->coords[i] < c.coords[i])
			return true;
		if (this->coords[i] > c.coords[i])
			return false;
	}
	return false;
}

bool OctTreeCoords::operator>(OctTreeCoords& c) {
	for (int i = 0; i < parent->depth; i++) {
		if (this->coords[i] & OCT_NONE || c.coords[i] & OCT_NONE)
			return false;
		if (this->coords[i] > c.coords[i])
			return true;
		if (this->coords[i] < c.coords[i])
			return false;
	}
	return false;
}

bool OctTreeCoords::operator<=(OctTreeCoords& c) {
	return !(*this > c);
}

bool OctTreeCoords::operator>=(OctTreeCoords& c) {
	return !(*this < c);
}

bool OctTreeCoords::operator==(OctTreeCoords& c) {
	for (int i = 0; i < parent->depth; i++) {
		if (this->coords[i] & OCT_NONE || c.coords[i] & OCT_NONE)
			return true;
		if (this->coords[i] != c.coords[i])
			return false;
	}
	return true;
}

bool OctTreeCoords::operator!=(OctTreeCoords& c) {
	return !(*this == c);
}*/
