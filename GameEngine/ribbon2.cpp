#include "ribbon2.h"
#include "game_engine.h"
#include <unordered_set>
#include <unordered_map>
#include <vector>

Ribbon2::Ribbon2(Texture* in_texture) :
	mMeshVertexData(MeshVertexData::DataType::_ushort),
	mTextureInstance(in_texture) {

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
	std::unordered_map<ubyte, const void*> newData;
	std::vector<Vector2d> newPositions(4);
	std::vector<Vector2f> newUVs(4);
	std::vector<ColorRGBAf> newColors(4);
	std::vector<ushort> newIndices(6);

	for (auto it = mGraph.mNodes.begin(); it != mGraph.mNodes.end(); it++) {
		node_t& node1 = *it;
		for (auto jt = it->mEdges.begin(); jt != it->mEdges.end(); jt++) {
			edge_t& edge = *jt;
			ushort baseIndex = (ushort)mMeshVertexData.getNumberVertices();
			node_t& node2 = *jt->mDest;
			Vector2d offset = (node2.mData.mPosition - node1.mData.mPosition).orthogonal().normalized() / 2.0;
			Vector2d offset1 = offset * node1.mData.mWidth;
			Vector2d offset2 = offset * node2.mData.mWidth;
			newPositions.assign({
				node1.mData.mPosition + offset1,
				node1.mData.mPosition - offset1,
				node2.mData.mPosition - offset2,
				node2.mData.mPosition + offset2
				});
			newUVs.assign({
				Vector2f(edge.mData.mMinUV.x(), edge.mData.mMaxUV.y()),
				edge.mData.mMinUV,
				Vector2f(edge.mData.mMaxUV.x(), edge.mData.mMinUV.y()),
				edge.mData.mMaxUV
				});
			newColors.assign({
				node1.mData.mColor,
				node1.mData.mColor,
				node2.mData.mColor,
				node2.mData.mColor
				});
			newIndices.assign({
				(ushort)(baseIndex + 0),
				(ushort)(baseIndex + 1),
				(ushort)(baseIndex + 2),
				(ushort)(baseIndex + 2),
				(ushort)(baseIndex + 3),
				(ushort)(baseIndex + 0)
				});
			newData[MeshVertexData::MemberID::position] = &newPositions[0];
			newData[MeshVertexData::MemberID::uv] = &newUVs[0];
			newData[MeshVertexData::MemberID::color] = &newColors[0];
			mMeshVertexData.addVertices(4, newData);
			mMeshVertexData.addFaces(2, &newIndices[0]);
		}
	}

	mGPUPusher.pushAll();
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

		ColorRGBAf color = ColorRGBAf(1.0, 1.0, 1.0, 1.0);

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

		ColorRGBAf color = ColorRGBAf(1.0, 1.0, 1.0, 1.0);

		shaderProgram->use();
		glUniformMatrix4fv(locations[0], 1, GL_TRUE, modelMatrix.pointer());
		glUniformMatrix4fv(locations[1], 1, GL_TRUE, viewMatrix.pointer());
		glUniformMatrix4fv(locations[2], 1, GL_TRUE, projectionMatrix.pointer());
		glUniform4fv(locations[3], 1, color.pointer());
	}

	mGPUPusher.draw();
}
