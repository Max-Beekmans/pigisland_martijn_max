#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/random.hpp"
#include <algorithm>
#include <cassert>
#include <numeric>
#include <iostream>
#include <set>
#include <queue>
#include <stack>

namespace kmint::ufo {

    map::map_node const &random_adjacent_node(map::map_node const &node) {
        int r = random_int(0, node.num_edges());
        return node[r].to();
    }

    map::map_node &find_node_of_kind(map::map_graph const &graph, char kind) {
        auto i = std::find_if(graph.begin(), graph.end(), [kind](auto const &node) {
            return node.node_info().kind == kind;
        });
        return const_cast<map::map_node &>(*i);
    }

    map::map_node &random_node_of_kind(map::map &map, char kind) {
        auto b = map.begin_of_kind(kind);
        auto e = map.end_of_kind(kind);
        int n = e - b;
        assert(n > 0);
        return *b[random_int(0, n)];
    }

    map::map_node const &random_node_of_kind(map::map const &map, char kind) {
        auto b = map.begin_of_kind(kind);
        auto e = map.end_of_kind(kind);
        int n = e - b;
        assert(n > 0);
        return *b[random_int(0, n)];
    }

    map::map_node const &find_closest_node_to(map::map_graph const &graph,
                                              math::vector2d location) {
        auto i = std::min_element(graph.begin(), graph.end(),
                                  [location](auto const &l, auto const &r) {
                                      return distance2(l.location(), location) <
                                             distance2(r.location(), location);
                                  });
        return *i;
    }

    double waiting_time(map::map_node const &node) { return node[0].weight(); }

    float calculate_heuristic(Heuristic h, const float x, const float y, const float goal_x, const float goal_y) {
        switch (h) {
            case MANHATTAN: {
                return std::abs(x) + std::abs(y);
            }
            case DIAGONAL: {
                if (goal_x < 0 || goal_y < 0) {
                    return -1;
                }
                return std::max(std::abs(x - goal_x), std::abs(y - goal_y));
            }
            case EUCLIDEAN: {
                if (goal_x < 0 || goal_y < 0) {
                    return -1;
                }
                return std::sqrt(std::abs(x - goal_x) * 2 + std::abs(y - goal_y) * 2);
            }
            default: {
                return -1;
            }
        }
    }

    float calculate_heuristic(Heuristic h, map::map_node const &loc, map::map_node const &destLoc) {
        return calculate_heuristic(h, loc.location().x(), loc.location().y(), destLoc.location().x(), destLoc.location().y());
    }

    /// Struct to wrap a node together with a double to hold the distance to this node.
    /// Provides comparative operator overloads to compare struct
    struct NodeWrapper {
    public:

        NodeWrapper(const float val, map::map_node &node, map::map_node &parent):
            val_(val), node_(&node), parent_(&parent) {}

        NodeWrapper(const float val, map::map_node *node, map::map_node *parent):
            val_(val), node_(node), parent_(parent) {}

        void setDist(const float dist) { val_ = dist; }
        float getDist() const { return val_; }
        map::map_node *getNode() const { return node_; }
        void setParent(map::map_node &parent) { parent_ = &parent; }
        map::map_node *getParent() const { return parent_; }

        friend bool operator < (const NodeWrapper& lhs, const NodeWrapper& rhs) {
            return lhs.getDist() < rhs.getDist();
        }

        friend bool operator > (const NodeWrapper& lhs, const NodeWrapper& rhs) {
            return lhs.getDist() > rhs.getDist();
        }

        friend bool operator == (const NodeWrapper& lhs, const NodeWrapper& rhs) {
            return (lhs.getNode() == rhs.getNode());
        }

        friend std::ostream& operator << (std::ostream& os, const NodeWrapper& nodeWrapper) {
            return os << "Distance to node[" << nodeWrapper.getNode()->node_id() << "]: " << nodeWrapper.getDist();
        }

    private:
        float val_;
        map::map_node *node_;
        map::map_node *parent_;
    };
    /// tag the shortest path from actorLoc to goalLoc using our own implementation of A*
    /// \param h. heuristic used to calculate distance between the current position and the goal position
    /// \param actorLoc. starting map_node of search.
    /// \param goalLoc. destination map_node of search
    void tag_shortest_path_astar(Heuristic h, map::map_node const &actorLoc, map::map_node const &goalLoc, map::map_graph &graph) {
        //! With each assignment to the queue it will re-sort based on NodeWrapper.val
        std::deque<NodeWrapper> openDeQueue{};
        //! I dislike the way you have to get a non-const reference from the kmint framework
        //! This is my workaround
        auto &mutableActorLoc = graph[actorLoc.node_id()];
        openDeQueue.emplace_back(NodeWrapper(0.0, mutableActorLoc, mutableActorLoc));
        while (!openDeQueue.empty()) {
            NodeWrapper &top_wrapper = openDeQueue.front();
            auto *top_node = top_wrapper.getNode();
            auto &topNodeRef = graph[top_node->node_id()];
            //! Same workaround as above
            float dist = top_wrapper.getDist();
            //! pop front node and tag as visited
            top_node->tag(graph::node_tag::visited);
            openDeQueue.pop_front();
            for(auto &edge : topNodeRef) {
                map::map_node &successor = edge.to();
                //! Check if this edge leads to the goal node.
                if(successor.node_id() == goalLoc.node_id()) {
                    //TODO end search and trace path
                    for(auto c : openDeQueue) {
                        std::cout << c << std::endl;
                    }
                }
                //! Check if the successor node has been visited before
                if(successor.tag() == graph::node_tag::visited) {
                    continue;
                }
                //! Calculate new distance for successor
                NodeWrapper wrapper{0.0, successor, topNodeRef};
                float newDist = dist + edge.weight() + calculate_heuristic(h, successor, goalLoc);
                auto it = std::find(openDeQueue.begin(), openDeQueue.end(), wrapper);
                //! Check if the successor is already on the queue
                //! Else check if the newDist is shorter than the old one
                if(it == openDeQueue.end()) {
                    //! Add successor and it's new distance to the openDeQueue
                    openDeQueue.emplace_back(newDist, successor, topNodeRef);
                } else if(it->getDist() > newDist) {
                    it->setDist(newDist);
                }
                //! Sort the DeQueue
                std::sort(openDeQueue.begin(), openDeQueue.end());
            }
        }
        //! Failed to find goal
        for(auto c : openDeQueue) {
            std::cout << "Fail: " << std::endl;
            std::cout << c << std::endl;
        }
    }
} // namespace kmint
