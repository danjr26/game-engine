#ifndef GRAPH_H
#define GRAPH_H

#include <forward_list>

template<class NodeData, class EdgeData>
struct GraphEdge;

template<class NodeData, class EdgeData>
struct GraphNode {
	using node_t = GraphNode<NodeData, EdgeData>;
	using edge_t = GraphEdge<NodeData, EdgeData>;

	NodeData mData;
	std::forward_list<edge_t> mEdges;

	void connect1(node_t& in_other, EdgeData& in_edgeData) {
		mEdges.push_front(edge_t{ in_edgeData, &in_other });
	}

	void connect2(node_t& in_other, EdgeData& in_edgeData) {
		this->connect1(in_other, in_edgeData);
		other.connect1(*this, in_edgeData);
	}
};

template<class NodeData, class EdgeData>
struct GraphEdge {
	using node_t = GraphNode<NodeData, EdgeData>;
	using edge_t = GraphEdge<NodeData, EdgeData>;

	EdgeData mData;
	node_t* mDest;
};

template<class NodeData, class EdgeData>
struct Graph {
	using node_t = GraphNode<NodeData, EdgeData>;
	using edge_t = GraphEdge<NodeData, EdgeData>;

	std::forward_list<node_t> mNodes;

	node_t& addNode() {
		mNodes.push_front(node_t());
		return *mNodes.begin();
	}

	node_t& addNode(const node_t& in_node) {
		mNodes.push_front(in_node);
		return *mNodes.begin();
	}
};

#endif