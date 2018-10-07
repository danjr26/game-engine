#include "collision_mask.h"

SphereTreeCollisionMask::SphereTreeCollisionMask(Transform3d& in_transform, CustomArray<SphereNode> in_sphereTree) :
CollisionMask3(Type::sphere_tree),
transform	(in_transform), 
sphereTree	(in_sphereTree)
{}

SphereTreeCollisionMask::~SphereTreeCollisionMask() 
{}

Collision SphereTreeCollisionMask::Is_Colliding(SphereTreeCollisionMask& in_otherMask) {
	const static uint maxSphereBufferLength = 4096;
	static SphereNode* thisMaskSphereBuffer[maxSphereBufferLength];
	static SphereNode* thatMaskSphereBuffer[maxSphereBufferLength];
	static uint uniqueCallID = 0;
	SphereTreeCollisionMask& thisMask = *this;

	thisMaskSphereBuffer[0] = &thisMask.sphereTree[0];
	thatMaskSphereBuffer[0] = &in_otherMask.sphereTree[0];
	uniqueCallID++;

	uint thisMaskSphereBufferCount = 1;
	uint thatMaskSphereBufferCount = 1;

	SphereNode* currentThisMaskSphere = nullptr;
	SphereNode* currentThatMaskSphere = nullptr;

	Collision collision;

	currentThisMaskSphere = thisMaskSphereBuffer[0];
	currentThisMaskSphere->Update_Center(thisMask.transform, uniqueCallID);
	for (uint i = 0; i < thatMaskSphereBufferCount && thatMaskSphereBufferCount > 0; ) {
		currentThatMaskSphere = thatMaskSphereBuffer[i];
		currentThatMaskSphere->Update_Center(in_otherMask.transform, uniqueCallID);

		collision = Spheres_Are_Colliding(currentThisMaskSphere, currentThatMaskSphere);
		if (collision.didCollide) {
			if (!currentThatMaskSphere->Is_Leaf()) {
				thatMaskSphereBuffer[thatMaskSphereBufferCount] = currentThatMaskSphere->Child_2();
				thatMaskSphereBuffer[i] = currentThatMaskSphere->Child_1();
				thatMaskSphereBufferCount++;
				if (thatMaskSphereBufferCount >= maxSphereBufferLength)
					throw OverflowException(maxSphereBufferLength, "SphereTreeCollisionMask:Is_Colliding:sbuffer2");
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

	if (currentThisMaskSphere->Is_Leaf())
		return{ true, (currentThatMaskSphere->World_Center() + currentThisMaskSphere->World_Center()) * 0.5 };

	thisMaskSphereBuffer[1] = currentThisMaskSphere->Child_2();
	thisMaskSphereBuffer[0] = currentThisMaskSphere->Child_1();
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
		currentThisMaskSphere->Update_Center(thisMask.transform, uniqueCallID);
		
		bool shouldSplit = false;

		for (uint j = 0; j < thatMaskSphereBufferCount; j++) {
			if (!(tryBuffer[thisMaskSphereBufferCount - 1][j >> 3] & (1 << (j % 8))))
				//parent has ruled this one out already
				continue;

			currentThatMaskSphere = thatMaskSphereBuffer[j];
			currentThatMaskSphere->Update_Center(in_otherMask.transform, uniqueCallID);

			collision = Spheres_Are_Colliding(currentThisMaskSphere, currentThatMaskSphere);
			if (collision.didCollide) {
				if (!currentThisMaskSphere->Is_Leaf()) {
					shouldSplit = true;
					//will split into children because sphere collision detected and is nonbasic
					continue;
				}
				else {
					//bi-basic sphere collision, so update triangles if applicable

					currentThisMaskSphere->Update_Triangle(thisMask.transform, uniqueCallID);
					currentThatMaskSphere->Update_Triangle(in_otherMask.transform, uniqueCallID);

					collision = Triangles_Are_Colliding(currentThisMaskSphere, currentThatMaskSphere);

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
			thisMaskSphereBuffer[thisMaskSphereBufferCount] = currentThisMaskSphere->Child_2();
			thisMaskSphereBuffer[thisMaskSphereBufferCount - 1] = currentThisMaskSphere->Child_1();
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



Collision SphereTreeCollisionMask::Triangles_Are_Colliding(
	const SphereNode* in_sphere1, const SphereNode* in_sphere2) {

	const Triangle3f& triangle1 = in_sphere1->World_Triangle();
	const Triangle3f& triangle2 = in_sphere2->World_Triangle();

	//find plane of each triangle
	Planef plane1 = triangle1.Plane();
	Planef plane2 = triangle2.Plane();

	//find distances from each point of each triangle to the other triangle's plane
	Triple<float> pointDisplacements1To2 = Triple<float>(
		plane2.Signed_Distance_To(triangle1[0]),
		plane2.Signed_Distance_To(triangle1[1]),
		plane2.Signed_Distance_To(triangle1[2])
		);

	Triple<float> pointDisplacements2To1 = Triple<float> (
		plane1.Signed_Distance_To(triangle2[0]),
		plane1.Signed_Distance_To(triangle2[1]),
		plane1.Signed_Distance_To(triangle2[2])
		);

	Triple<float> pointDistances1To2 = Triple<float>(
		abs(pointDisplacements1To2[0]),
		abs(pointDisplacements1To2[1]),
		abs(pointDisplacements1To2[2])
		);

	Triple<float> pointDistances1To2 = Triple<float> (
		abs(pointDisplacements2To1[0]),
		abs(pointDisplacements2To1[1]),
		abs(pointDisplacements2To1[2])
		);

	// some bit magic to isolate unique point

	uchar index11, index12, index13, index21, index22, index23;

	index11 =
		(pointDisplacements1To2[0] < 0) |
		((pointDisplacements1To2[1] < 0) << 1) |
		((pointDisplacements1To2[2] < 0) << 2);
	if (pointDisplacements1To2[2]) {
		index11 = ~index11;
	} 
	index11 = 3 - index11;

	if (index11 == 3) {
		return{ false };
	}

	index12 = (index11 + 1) % 3;
	index13 = (index11 + 2) % 3;

	index21 =
		(pointDisplacements2To1[0] < 0) |
		((pointDisplacements2To1[1] < 0) << 1) |
		((pointDisplacements2To1[2] < 0) << 2);
	if (pointDisplacements2To1[2]) {
		index21 = ~index21;
	} 
	index21 = 3 - index21;

	if (index21 == 3) {
		return{ false };
	}

	index22 = (index21 + 1) % 3;
	index23 = (index21 + 2) % 3;

	//find axis perpendicular to both triangles' normals
	Vector3f axis = plane1.Coincidence(plane2);

	//lazily project each point of each triangle onto the axis
	Triple<float> triangle1ToAxis = Triple<float>(
		triangle1[0].Project_Coeff(axis),
		triangle1[1].Project_Coeff(axis),
		triangle1[2].Project_Coeff(axis)
	);
	Triple<float> triangle2ToAxis = Triple<float>(
		triangle2[0].Project_Coeff(axis),
		triangle2[1].Project_Coeff(axis),
		triangle2[2].Project_Coeff(axis)
	);

	float min1, min2, max1, max2;

	min1 = triangle1ToAxis[index12] + (triangle1ToAxis[index11] - triangle1ToAxis[index12]) * 
		pointDistances1To2[index12] / (pointDistances1To2[index12] + pointDistances1To2[index11]);
	max1 = triangle1ToAxis[index13] + (triangle1ToAxis[index11] - triangle1ToAxis[index13]) * 
		pointDistances1To2[index13] / (pointDistances1To2[index13] + pointDistances1To2[index11]);

	min2 = triangle1ToAxis[index22] + (triangle1ToAxis[index21] - triangle1ToAxis[index22]) * 
		pointDistances1To2[index22] / (pointDistances1To2[index22] + pointDistances1To2[index21]);
	max2 = triangle1ToAxis[index23] + (triangle1ToAxis[index21] - triangle1ToAxis[index23]) * 
		pointDistances1To2[index23] / (pointDistances1To2[index23] + pointDistances1To2[index21]);

	if (min1 > max1) {
		std::swap(min1, max1);
	}

	if (min2 > max2) {
		std::swap(min2, max2);
	}

	// 1 1 2 2 or 2 2 1 1
	Collision collision = { false };

	if (Is_Between_Inc(min2, min1, max1)) {
		// 1 2 1 2 or 1 2 2 1
		collision = { true, axis * Mean(min2, Min(max1, max2)) };
	}
	else if (Is_Between_Inc(max2, min1, max1)) {
		// 2 1 2 1
		collision = { true, axis * Mean(max2, Max(min1, max1)) };
	}
	else if (Is_Between_Inc(min1, min2, max2) && Is_Between_Inc(max1, min2, max2)) {
		// 2 1 1 2
		collision = { true, axis * Mean(min1, max1) };
	}
	
	return collision;
}

Collision SphereTreeCollisionMask::Spheres_Are_Colliding(const SphereNode* in_sphere1,  const SphereNode* in_sphere2) {
	float radius = in_sphere1->Radius() + in_sphere2->Radius();
	Vector3d distance = in_sphere1->World_Center() - in_sphere2->World_Center();

	if (distance.Dot_Self() <= radius * radius) {
		return Collision{ true, Mean(in_sphere1->World_Center(), in_sphere2->World_Center()) };
	}
	else {
		return Collision{ false };
	}
}
