#include "ribbon2.h"
#include "game_engine.h"
#include "line.h"
#include "ray.h"
#include <set>
#include <unordered_map>
#include <vector>

Ribbon2::Ribbon2(Texture* i_texture) :
	mMeshVertexData(MeshVertexData::DataType::_ushort),
	mTextureInstance(i_texture) {

	mMeshVertexData.addMember(
		MeshVertexData::MemberID::position,
		MeshVertexData::DataType::_double, 2,
		nullptr
	);
	mMeshVertexData.addMember(
		MeshVertexData::MemberID::color,
		MeshVertexData::DataType::_float, 4,
		nullptr
	);
	mMeshVertexData.addMember(
		MeshVertexData::MemberID::uv,
		MeshVertexData::DataType::_float, 2,
		nullptr
	);

	mGPUPusher.initialize(&mMeshVertexData);
}

Ribbon2::graph_t& Ribbon2::getGraph() {
	return mGraph;
}

TextureInstance& Ribbon2::getTextureInstance() {
	return mTextureInstance;
}

void Ribbon2::updateMesh() {
	double t1 = GE.clock().now();
	struct NodeEntry {
		Vector2d mOffset;
		ushort mCloseIndex1;
		ushort mCloseIndex2;
		ushort mFarIndex1;
		ushort mFarIndex2;

		struct Comparator {
			bool operator()(const NodeEntry& ne1, const NodeEntry& ne2) const {
				return ne1.mOffset.ccwTheta() < ne2.mOffset.ccwTheta();
			}
		};
	};

	std::unordered_map<node_t*, std::set<NodeEntry, NodeEntry::Comparator>> allEdges;
	for (auto it = mGraph.mNodes.begin(); it != mGraph.mNodes.end(); it++) {
		allEdges[&*it] = {};
	}

	std::unordered_map<ubyte, const void*> newData;
	std::vector<Vector2d> newPositions(6);
	std::vector<Vector2f> newUVs(6);
	std::vector<Color4f> newColors(6);
	std::vector<ushort> newIndices(12);
	for (auto it = mGraph.mNodes.begin(); it != mGraph.mNodes.end(); it++) {
		node_t& node1 = *it;
		for (auto jt = it->mEdges.begin(); jt != it->mEdges.end(); jt++) {
			edge_t& edge = *jt;
			ushort baseIndex = (ushort)mMeshVertexData.getNumberVertices();
			node_t& node2 = *jt->mDest;
			Vector2d outward = node2.mData.mPosition - node1.mData.mPosition;
			Vector2d offset = outward.orthogonal().normalized() / 2.0;
			Vector2d offset1 = offset * node1.mData.mWidth;
			Vector2d offset2 = offset * node2.mData.mWidth;

			newPositions.assign({
				node1.mData.mPosition + offset1,
				node1.mData.mPosition,
				node1.mData.mPosition - offset1,
				node2.mData.mPosition - offset2,
				node2.mData.mPosition,
				node2.mData.mPosition + offset2
				});
			newUVs.assign({
				node1.mData.mUV1,
				(node1.mData.mUV1 + node1.mData.mUV2) / 2.0,
				node1.mData.mUV2,
				node2.mData.mUV2,
				(node2.mData.mUV1 + node2.mData.mUV2) / 2.0,
				node2.mData.mUV1
				});
			newColors.assign({
				node1.mData.mColor,
				node1.mData.mColor,
				node1.mData.mColor,
				node2.mData.mColor,
				node2.mData.mColor,
				node2.mData.mColor
				});
			newIndices.assign({
				(ushort)(baseIndex + 0),
				(ushort)(baseIndex + 1),
				(ushort)(baseIndex + 5),
				(ushort)(baseIndex + 1),
				(ushort)(baseIndex + 5),
				(ushort)(baseIndex + 4),
				(ushort)(baseIndex + 1),
				(ushort)(baseIndex + 4),
				(ushort)(baseIndex + 2),
				(ushort)(baseIndex + 2),
				(ushort)(baseIndex + 4),
				(ushort)(baseIndex + 3)
				});

			newData[MeshVertexData::MemberID::position] = &newPositions[0];
			newData[MeshVertexData::MemberID::uv] = &newUVs[0];
			newData[MeshVertexData::MemberID::color] = &newColors[0];

			mMeshVertexData.addVertices(6, newData);
			mMeshVertexData.addFaces(4, &newIndices[0]);

			allEdges[&node1].insert({
				outward,
				(ushort)(baseIndex + 0),
				(ushort)(baseIndex + 2),
				(ushort)(baseIndex + 5),
				(ushort)(baseIndex + 3)
				});
			allEdges[&node2].insert({
				-outward,
				(ushort)(baseIndex + 3),
				(ushort)(baseIndex + 5),
				(ushort)(baseIndex + 2),
				(ushort)(baseIndex + 0)
				});
		}
	}


	for (auto it = mGraph.mNodes.begin(); it != mGraph.mNodes.end(); it++) {
		std::set<NodeEntry, NodeEntry::Comparator>& entries = allEdges[&*it];
		if (entries.size() <= 1) continue;
		ushort baseIndex = (ushort)mMeshVertexData.getNumberVertices();

		newPositions.assign({ it->mData.mPosition });
		newUVs.assign({ (it->mData.mUV1 + it->mData.mUV2) / 2.0 });
		newColors.assign({ it->mData.mColor });

		newData[MeshVertexData::MemberID::position] = &newPositions[0];
		newData[MeshVertexData::MemberID::uv] = &newUVs[0];
		newData[MeshVertexData::MemberID::color] = &newColors[0];
		mMeshVertexData.addVertices(1, newData);

		for (auto jt = entries.begin(); jt != entries.end(); jt++) {
			const NodeEntry& entry1 = *jt;
			auto next = std::next(jt);
			const NodeEntry& entry2 = (next == entries.end()) ? *entries.begin() : *next;
			if (entry1.mOffset.ccwTheta(entry2.mOffset) > PI) {
				newIndices.assign({
					baseIndex,
					entry1.mCloseIndex2,
					entry2.mCloseIndex1
					});
				mMeshVertexData.addFaces(1, &newIndices[0]);
			}
			else {
				Vector2d midline = (entry1.mOffset.normalized() + entry2.mOffset.normalized()).normalized();

				Vector2d closeVertex1, closeVertex2, farVertex1, farVertex2;
				mMeshVertexData.getMemberValue(MeshVertexData::MemberID::position, entry1.mCloseIndex2, &closeVertex1[0]);
				mMeshVertexData.getMemberValue(MeshVertexData::MemberID::position, entry2.mCloseIndex1, &closeVertex2[0]);
				mMeshVertexData.getMemberValue(MeshVertexData::MemberID::position, entry1.mFarIndex2, &farVertex1[0]);
				mMeshVertexData.getMemberValue(MeshVertexData::MemberID::position, entry2.mFarIndex1, &farVertex2[0]);

				Line2Collider<double> lineMask1(Line2d::fromPoints(closeVertex1, farVertex1));
				Line2Collider<double> lineMask2(Line2d::fromPoints(closeVertex2, farVertex2));

				lineMask1.setAsIs(true);
				lineMask2.setAsIs(true);

				InPlaceCollisionEvaluator2d evaluator;
				evaluator.returnPoint(true);
				Vector2d newPoint = evaluator.evaluateTyped(lineMask1, lineMask2).mPoint;
				
				mMeshVertexData.setMemberValue(MeshVertexData::MemberID::position, entry1.mCloseIndex2, &newPoint[0]);
				mMeshVertexData.setMemberValue(MeshVertexData::MemberID::position, entry2.mCloseIndex1, &newPoint[0]);
			}
		}
	}

	mGPUPusher.pushAll();

	double t2 = GE.clock().now();
	Log::main(std::string("t: ") + std::to_string(t2 - t1));
}

double Ribbon2::z() const {
	return mDepthTransform.getWorldDepth();
}

bool Ribbon2::shouldCull() const {
	return false;
}

void Ribbon2::render() {
	ShaderProgram* shaderProgram;

	Matrix4f modelMatrix = mDepthTransform.getWorldMatrix() * mTransform.getWorldMatrix();
	Matrix4f viewMatrix = GE.cameras().getActive()->getViewMatrix();
	Matrix4f projectionMatrix = GE.cameras().getActive()->getProjectionMatrix();

	if (mTextureInstance.getTexture() == nullptr) {
		shaderProgram = GE.assets().get<ShaderProgram>("MonoShader");

		GLint locations[2] = {
			shaderProgram->getUniformLocation("mvpMatrix"),
			shaderProgram->getUniformLocation("color")
		};

		Color4f color = Color4f(1.0, 1.0, 1.0, 1.0);

		Matrix4f mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

		shaderProgram->use();
		glUniformMatrix4fv(locations[0], 1, GL_TRUE, mvpMatrix.pointer());
		glUniform4fv(locations[1], 1, color.pointer());
	}
	else {
		shaderProgram = GE.assets().get<ShaderProgram>("DeferredShader");

		GLint locations[4] = {
			shaderProgram->getUniformLocation("modelMatrix"),
			shaderProgram->getUniformLocation("viewMatrix"),
			shaderProgram->getUniformLocation("projectionMatrix"),
			shaderProgram->getUniformLocation("color")
		};

		mTextureInstance.use();

		Color4f color = Color4f(1.0, 1.0, 1.0, 1.0);

		shaderProgram->use();
		glUniformMatrix4fv(locations[0], 1, GL_TRUE, modelMatrix.pointer());
		glUniformMatrix4fv(locations[1], 1, GL_TRUE, viewMatrix.pointer());
		glUniformMatrix4fv(locations[2], 1, GL_TRUE, projectionMatrix.pointer());
		glUniform4fv(locations[3], 1, color.pointer());
	}

	mGPUPusher.draw();
}
