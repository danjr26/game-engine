#ifndef GRAPH_H
#define GRAPH_H

#include <forward_list>
#include <unordered_set>
#include <unordered_map>

template<class NodeData, class EdgeData>
struct GraphEdge;

template<class NodeData, class EdgeData>
struct GraphNode {
	using node_t = GraphNode<NodeData, EdgeData>;
	using edge_t = GraphEdge<NodeData, EdgeData>;

	NodeData mData;
	std::forward_list<edge_t> mEdgesOut;

	GraphNode() {}

	GraphNode(const NodeData& i_data) :
	mData(i_data)
	{}

	void connect1(node_t& i_other, const EdgeData& i_edgeData) {
		mEdgesOut.push_front(edge_t{ i_edgeData, &i_other });
	}

	void connect2(node_t& i_other, const EdgeData& i_edgeData) {
		this->connect1(i_other, i_edgeData);
		i_other.connect1(*this, i_edgeData);
	}
};

template<class NodeData, class EdgeData>
struct GraphEdge {
	using node_t = GraphNode<NodeData, EdgeData>;
	using edge_t = GraphEdge<NodeData, EdgeData>;

	EdgeData mData;
	node_t* mNode;
};

template<class NodeData, class EdgeData>
struct Graph {
	using node_t = GraphNode<NodeData, EdgeData>;
	using edge_t = GraphEdge<NodeData, EdgeData>;

	std::forward_list<node_t> mNodes;

	node_t& addNode() {
		mNodes.push_front(node_t());
		return mNodes.front();
	}

	node_t& addNode(const NodeData& i_data) {
		mNodes.emplace_front(i_data);
		return mNodes.front();
	}

	void removeNode(const node_t& i_node) {
		mNodes.remove_if([&i_node](const node_t& n) { return &n == &i_node; });
	}

	bool getTopologicalOrder(std::vector<node_t*>& o_nodes) {
		std::unordered_map<node_t*, uint> waitingNodes;
		std::unordered_set<node_t*> readyNodes;

		// count up incoming edges
		for (auto it = mNodes.begin(); it != mNodes.end(); it++) {
			waitingNodes.insert(std::make_pair(&*it, 0u));
			for (auto jt = it->mEdgesOut.begin(); jt != it->mEdgesOut.end(); jt++) {
				auto result = waitingNodes.insert(std::make_pair(jt->mNode, 0u));
				result.first->second++;
			}
		}

		while (!waitingNodes.empty()) {
			bool cyclic = true;
			for (auto it = waitingNodes.begin(); it != waitingNodes.end(); it++) {
				if (!it->second) {
					readyNodes.insert(it->first);
					cyclic = false;
					for (auto jt = it->first->mEdgesOut.begin(); jt != it->first->mEdgesOut.end(); jt++) {
						auto result = waitingNodes.find(jt->mNode);
						if (result != waitingNodes.end()) {
							result->second--;
						}
					}
				}
			}
			if (cyclic) {
				return false;
			}
			for (auto it = readyNodes.begin(); it != readyNodes.end(); it++) {
				waitingNodes.erase(*it);
				o_nodes.push_back(*it);
			}
			readyNodes.clear();
		}

		return true;
	}
};

#endif