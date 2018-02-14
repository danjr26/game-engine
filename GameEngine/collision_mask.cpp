#include "collision_mask.h"

CollisionMask::CollisionMask(Transform3d* in_transform, Vector3d* in_velocity, SphereTree* in_tree) :
transform	(in_transform), 
velocity	(in_velocity),
tree		(in_tree)
{}

CollisionMask::~CollisionMask() 
{}

Collision CollisionMask::Is_Colliding(CollisionMask* in_otherMask) {
	const static uint maxSphereBufferLength = 4096;
	static SphereNode* thisMaskSphereBuffer[maxSphereBufferLength];
	static SphereNode* thatMaskSphereBuffer[maxSphereBufferLength];
	static uint uniqueCallIdentifier = 0;
	CollisionMask* thisMask = this;

	thisMaskSphereBuffer[0] = thisMask->tree->trunk;
	thatMaskSphereBuffer[0] = in_otherMask->tree->trunk;
	uniqueCallIdentifier++;

	uint thisMaskSphereBufferCount = 1;
	uint thatMaskSphereBufferCount = 1;

	SphereNode* currentThisMaskSphere = nullptr;
	SphereNode* currentThatMaskSphere = nullptr;

	Collision collision;

	currentThisMaskSphere = thisMaskSphereBuffer[0];
	currentThisMaskSphere->worldCenter = thisMask->transform->S_To_O(currentThisMaskSphere->localCenter);
	currentThisMaskSphere->lastCenterUpdate = uniqueCallIdentifier;
	for (uint i = 0; i < thatMaskSphereBufferCount && thatMaskSphereBufferCount > 0; ) {
		currentThatMaskSphere = thatMaskSphereBuffer[i];
		currentThatMaskSphere->worldCenter = in_otherMask->transform->S_To_O(currentThatMaskSphere->localCenter);
		currentThatMaskSphere->lastCenterUpdate = uniqueCallIdentifier;

		collision = Spheres_Are_Colliding(currentThisMaskSphere, currentThatMaskSphere);
		if (collision.didCollide) {
			if (currentThatMaskSphere->child1 != nullptr) {
				thatMaskSphereBuffer[thatMaskSphereBufferCount] = currentThatMaskSphere->child2;
				thatMaskSphereBuffer[i] = currentThatMaskSphere->child1;
				thatMaskSphereBufferCount++;
				if (thatMaskSphereBufferCount >= maxSphereBufferLength)
					Die("CollisionMask.Is_Colliding: sbuffer2 overflow");
			}
			else
				i++;
		}
		else {
			thatMaskSphereBufferCount--;
			thatMaskSphereBuffer[i] = thatMaskSphereBuffer[thatMaskSphereBufferCount];
		}
	}

	if (thatMaskSphereBufferCount == 0)
		return{false, Vector3d()};

	if (currentThisMaskSphere->child1 == nullptr)
		return{ true, (currentThatMaskSphere->worldCenter + currentThisMaskSphere->worldCenter) * 0.5 };

	thisMaskSphereBuffer[1] = currentThisMaskSphere->child2;
	thisMaskSphereBuffer[0] = currentThisMaskSphere->child1;
	thisMaskSphereBufferCount++;

	static uchar tryBuffer[maxSphereBufferLength][maxSphereBufferLength / 8];
	static bool tryBufferInitialized = false;
	if (!tryBufferInitialized) {
		for (int i = 0; i < maxSphereBufferLength; i++) {
			for (int j = 0; j < maxSphereBufferLength / 8; j++)
				tryBuffer[i][j] = 0xff;
		}
		tryBufferInitialized = true;
	}

	while (thisMaskSphereBufferCount > 0) {
		currentThisMaskSphere = thisMaskSphereBuffer[thisMaskSphereBufferCount - 1];
		if (currentThisMaskSphere->lastCenterUpdate != uniqueCallIdentifier) {
			currentThisMaskSphere->worldCenter = thisMask->transform->S_To_O(currentThisMaskSphere->localCenter);
			currentThisMaskSphere->lastCenterUpdate = uniqueCallIdentifier;
		}
		bool shouldSplit = false;

		for (uint j = 0; j < thatMaskSphereBufferCount; j++) {
			if (!(tryBuffer[thisMaskSphereBufferCount - 1][j >> 3] & (1 << (j % 8))))
				//parent has ruled this one out already
				continue;

			currentThatMaskSphere = thatMaskSphereBuffer[j];
			if (currentThatMaskSphere->lastCenterUpdate != uniqueCallIdentifier) {
				currentThatMaskSphere->worldCenter = in_otherMask->transform->S_To_O(currentThatMaskSphere->localCenter);
				currentThatMaskSphere->lastCenterUpdate = uniqueCallIdentifier;
			}

			collision = Spheres_Are_Colliding(currentThisMaskSphere, currentThatMaskSphere);
			if (collision.didCollide) {
				if (currentThisMaskSphere->child1 != nullptr) {
					shouldSplit = true;
					//will split into children because sphere collision detected and is nonbasic
					continue;
				}
				else {
					//bi-basic sphere collision, so update triangles if applicable
					if (currentThisMaskSphere->triupdate != uniqueCallIdentifier && currentThisMaskSphere->tri != nullptr) {
						for (int k = 0; k < 3; k++)
							currentThisMaskSphere->objtri[k] = thisMask->transform->S_To_O(currentThisMaskSphere->tri[k]);
						currentThisMaskSphere->triupdate = uniqueCallIdentifier;
					}
					if (currentThatMaskSphere->triupdate != uniqueCallIdentifier && currentThatMaskSphere->tri != nullptr) {
						for (int k = 0; k < 3; k++)
							currentThatMaskSphere->objtri[k] = in_otherMask->transform->S_To_O(currentThatMaskSphere->tri[k]);
						currentThatMaskSphere->triupdate = uniqueCallIdentifier;
					}

					collision = Hard_Test(currentThisMaskSphere, currentThatMaskSphere);

					if (collision.didCollide) {
						for (int k = 0; k < maxSphereBufferLength; k++) {
							for (int m = 0; m < maxSphereBufferLength / 8; m++)
								tryBuffer[k][m] = 0xff;
						}
						return collision;
					}
					else {
						tryBuffer[thisMaskSphereBufferCount - 1][j >> 3] &= ~(1 << (j % 8));
						//no collision at the triangle level, so no need to update trybuffer, since no children exist
						continue;
					}

				}
			}
			else {
				tryBuffer[thisMaskSphereBufferCount - 1][j >> 3] &= ~(1 << (j % 8));
				//no collision at the sphere level, so eliminate thisMask pairing
				continue;
			}
		}

		if (shouldSplit) {
			//splitting into children to achieve greater accuracy
			thisMaskSphereBuffer[thisMaskSphereBufferCount] = currentThisMaskSphere->child2;
			thisMaskSphereBuffer[thisMaskSphereBufferCount - 1] = currentThisMaskSphere->child1;
			memcpy(&tryBuffer[thisMaskSphereBufferCount][0], &tryBuffer[thisMaskSphereBufferCount - 1][0], thatMaskSphereBufferCount >> 3);
			thisMaskSphereBufferCount++;
		}
		else {
			thisMaskSphereBufferCount--;
			thisMaskSphereBuffer[thisMaskSphereBufferCount] = nullptr;
		}
			
	}

	for (int k = 0; k < maxSphereBufferLength; k++) {
		for (int m = 0; m < maxSphereBufferLength / 8; m++)
			tryBuffer[k][m] = 0xff;
	}

	return{ false, Vector3d() };
}

Collision CollisionMask::Triangles_Are_Colliding(
	const SphereNode* in_sphere1, const SphereNode* in_sphere2) {

	Vector3f* triangle1 = in_sphere1->objtri;
	Vector3f* triangle2 = in_sphere2->objtri;

	//find normal of each triangle
	Vector3d plane1 = ((triangle1[1] - triangle1[0]).Cross(triangle1[2] - triangle1[0]));
	Vector3d plane2 = ((triangle2[1] - triangle2[0]).Cross(triangle2[2] - triangle2[0]));

	//project each triangle onto its own normal
	double selfProjection1 = triangle1[0].Dot(plane1);
	double selfProjection2 = triangle2[0].Dot(plane2);


	//find distances from each point of each triangle to the other triangle's plane
	double crossProjectionDifferences1To2[3] = {
		triangle1[0].Dot(plane2) - selfProjection2,
		triangle1[1].Dot(plane2) - selfProjection2,
		triangle1[2].Dot(plane2) - selfProjection2
	};
	double crossProjectionDifferences2To1[3] = {
		triangle2[0].Dot(plane1) - selfProjection1,
		triangle2[1].Dot(plane1) - selfProjection1,
		triangle2[2].Dot(plane1) - selfProjection1
	};

	//check that triangles intersect each other's planes
	char pointSides1To2 =
		(crossProjectionDifferences1To2[0] < 0) |
		((crossProjectionDifferences1To2[1] < 0) << 1) |
		((crossProjectionDifferences1To2[2] < 0) << 2);

	if (pointSides1To2 == 7 || pointSides1To2 == 0)
		return{ false };

	char pointSides2To1 =
		(crossProjectionDifferences2To1[0] < 0) |
		((crossProjectionDifferences2To1[1] < 0) << 1) |
		((crossProjectionDifferences2To1[2] < 0) << 2);

	if (pointSides2To1 == 7 || pointSides2To1 == 0)
		return{ false };


	plane1.Normalize();
	plane2.Normalize();

	for (int i = 0; i < 3; i++)
		crossProjectionDifferences1To2[i] = triangle1[i].Dot(plane2) - selfProjection2;
	
	for (int i = 0; i < 3; i++)
		crossProjectionDifferences2To1[i] = triangle2[i].Dot(plane1) - selfProjection1;


	//find axis perpendicular to both triangles' normals
	Vector3d axis = plane1.Cross(plane2).Normalized();

	//project each point of each triangle onto the axis
	double projections1ToAxis[3] = {
		triangle1[0].Dot(axis),
		triangle1[1].Dot(axis),
		triangle1[2].Dot(axis)
	};
	double projections2ToAxis[3] = {
		triangle2[0].Dot(axis),
		triangle2[1].Dot(axis),
		triangle2[2].Dot(axis)
	};

	double min1, min2, max1, max2;

	switch (pointSides1To2) {
	case 1:
	case 6:
		min1 = projections1ToAxis[2] + (projections1ToAxis[0] - projections1ToAxis[2]) * 
			fabs(crossProjectionDifferences1To2[2]) / (fabs(crossProjectionDifferences1To2[2]) + fabs(crossProjectionDifferences1To2[0]));
		max1 = projections1ToAxis[1] + (projections1ToAxis[0] - projections1ToAxis[1]) * 
			fabs(crossProjectionDifferences1To2[1]) / (fabs(crossProjectionDifferences1To2[1]) + fabs(crossProjectionDifferences1To2[0]));
		break;
	case 2:
	case 5:
		min1 = projections1ToAxis[2] + (projections1ToAxis[1] - projections1ToAxis[2]) * 
			fabs(crossProjectionDifferences1To2[2]) / (fabs(crossProjectionDifferences1To2[2]) + fabs(crossProjectionDifferences1To2[1]));
		max1 = projections1ToAxis[0] + (projections1ToAxis[1] - projections1ToAxis[0]) * 
			fabs(crossProjectionDifferences1To2[0]) / (fabs(crossProjectionDifferences1To2[0]) + fabs(crossProjectionDifferences1To2[1]));
		break;
	case 4:
	case 3:
		min1 = projections1ToAxis[0] + (projections1ToAxis[2] - projections1ToAxis[0]) * 
			fabs(crossProjectionDifferences1To2[0]) / (fabs(crossProjectionDifferences1To2[0]) + fabs(crossProjectionDifferences1To2[2]));
		max1 = projections1ToAxis[1] + (projections1ToAxis[2] - projections1ToAxis[1]) * 
			fabs(crossProjectionDifferences1To2[1]) / (fabs(crossProjectionDifferences1To2[1]) + fabs(crossProjectionDifferences1To2[2]));
		break;
	}

	switch (pointSides2To1) {
	case 1:
	case 6:
		min2 = projections2ToAxis[2] + (projections2ToAxis[0] - projections2ToAxis[2]) * 
			fabs(crossProjectionDifferences2To1[2]) / (fabs(crossProjectionDifferences2To1[2]) + fabs(crossProjectionDifferences2To1[0]));
		max2 = projections2ToAxis[1] + (projections2ToAxis[0] - projections2ToAxis[1]) * 
			fabs(crossProjectionDifferences2To1[1]) / (fabs(crossProjectionDifferences2To1[1]) + fabs(crossProjectionDifferences2To1[0]));
		break;
	case 2:
	case 5:
		min2 = projections2ToAxis[2] + (projections2ToAxis[1] - projections2ToAxis[2]) * 
			fabs(crossProjectionDifferences2To1[2]) / (fabs(crossProjectionDifferences2To1[2]) + fabs(crossProjectionDifferences2To1[1]));
		max2 = projections2ToAxis[0] + (projections2ToAxis[1] - projections2ToAxis[0]) * 
			fabs(crossProjectionDifferences2To1[0]) / (fabs(crossProjectionDifferences2To1[0]) + fabs(crossProjectionDifferences2To1[1]));
		break;
	case 4:
	case 3:
		min2 = projections2ToAxis[0] + (projections2ToAxis[2] - projections2ToAxis[0]) * 
			fabs(crossProjectionDifferences2To1[0]) / (fabs(crossProjectionDifferences2To1[0]) + fabs(crossProjectionDifferences2To1[2]));
		max2 = projections2ToAxis[1] + (projections2ToAxis[2] - projections2ToAxis[1]) * 
			fabs(crossProjectionDifferences2To1[1]) / (fabs(crossProjectionDifferences2To1[1]) + fabs(crossProjectionDifferences2To1[2]));
		break;
	}

	if (min1 > max1) {
		std::swap(min1, max1);
	}

	if (min2 > max2) {
		std::swap(min2, max2);
	}

	Collision collision;

	if ((min1 < min2 && min2 < max1) ||
		(min1 < max2 && max2 < max1) ||
		(min2 < min1 && min2 < max1 && min1 < max2 && max1 < max2)) {
		switch (pointSides1To2) {
		case 1:
		case 6:
			collision = { 
				true, 
				(triangle1[0] + ((triangle1[1] + triangle1[2]) * 0.5 - triangle1[0]) * (fabs(crossProjectionDifferences1To2[0]) / 
				(fabs(crossProjectionDifferences1To2[0]) + (fabs(crossProjectionDifferences1To2[1]) + fabs(crossProjectionDifferences1To2[2])) * 0.5))) 
			};
			break;
		case 2:
		case 5:
			collision = { 
				true, 
				(triangle1[1] + ((triangle1[0] + triangle1[2]) * 0.5 - triangle1[1]) * (fabs(crossProjectionDifferences1To2[1]) / 
				(fabs(crossProjectionDifferences1To2[1]) + (fabs(crossProjectionDifferences1To2[0]) + fabs(crossProjectionDifferences1To2[2])) * 0.5))) 
			};
			break;
		case 3:
		case 4:
			collision = { 
				true, 
				(triangle1[2] + ((triangle1[1] + triangle1[0]) * 0.5 - triangle1[2]) * (fabs(crossProjectionDifferences1To2[2]) / 
				(fabs(crossProjectionDifferences1To2[2]) + (fabs(crossProjectionDifferences1To2[1]) + fabs(crossProjectionDifferences1To2[0])) * 0.5))) 
			};
			break;
		}
		return collision;
	}
	
		
	return{ false };
}

/*
Collision CollisionMask::Sphere_And_Triangle_Are_Colliding(
	const SphereNode* in_sphere, const SphereNode* in_triangle) {

	Vector3d triangle[3];

	for (int i = 0; i < 3; i++)
		triangle[i] = in_triangle->objtri[i] - in_sphere->objcenter;

	Vector3d plane = (triangle[1] - triangle[0]).Cross(triangle[2] - triangle[0]);
	double d = triangle[0].Dot(plane);
	double e = plane.Dot(plane);

	if (d * d > in_sphere->radius * in_sphere->radius * e)
		return{ false };

	double aa = triangle[0].Dot(triangle[0]);
	double ab = triangle[0].Dot(triangle[1]);
	double ac = triangle[0].Dot(triangle[2]);
	double bb = triangle[1].Dot(triangle[1]);
	double bc = triangle[1].Dot(triangle[2]);
	double cc = triangle[2].Dot(triangle[2]);
	double rr = in_sphere->radius * in_sphere->radius;

	if (((aa > rr) && (ab > aa) && (ac > aa)) ||
		((bb > rr) && (ab > bb) && (bc > bb)) ||
		((cc > rr) && (ac > cc) && (bc > cc)))
		return{ false };

	Vector3d edges[3] = {
		triangle[1] - triangle[0],
		triangle[2] - triangle[1],
		triangle[0] - triangle[2]
	};

	double ds[3] = {
		ab - aa,
		bc - bb,
		ac - cc
	};

	double es[3] = {
		edges[0].Dot(edges[0]),
		edges[1].Dot(edges[1]),
		edges[2].Dot(edges[2])
	};

	Vector3d cprojs[3] = {
		triangle[0] * es[0] - edges[0] * ds[0],
		triangle[1] * es[1] - edges[1] * ds[1],
		triangle[2] * es[2] - edges[2] * ds[2]
	};

	Vector3d pprojs[3] = {
		triangle[0] * es[1] - cprojs[1],
		triangle[1] * es[2] - cprojs[2],
		triangle[2] * es[0] - cprojs[0]
	};

	if (((cprojs[0].Dot(cprojs[0]) > rr * es[0] * es[0]) && (cprojs[0].Dot(pprojs[2]) > 0.0f)) ||
		((cprojs[1].Dot(cprojs[1]) > rr * es[1] * es[1]) && (cprojs[1].Dot(pprojs[0]) > 0.0f)) ||
		((cprojs[2].Dot(cprojs[2]) > rr * es[2] * es[2]) && (cprojs[2].Dot(pprojs[1]) > 0.0f)))
		return{ false };

	return{ true, (in_triangle->objcenter + in_sphere->objcenter) * 0.5 };
}*/