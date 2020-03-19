//
// Created by Max on 3/14/20.
//

#ifndef UFO_MARTIJN_MAX_NODEWRAPPER_H
#define UFO_MARTIJN_MAX_NODEWRAPPER_H

#include <kmint/map/map.hpp>

namespace kmint::ufo {

/// Struct to wrap a node together with a double to hold the distance to this node.
/// Provides comparative operator overloads to compare struct
    class NodeWrapper {
    public:
        NodeWrapper(const float val, map::map_node &node, NodeWrapper &parent) :
                val_(val), node_(&node), parent_(&parent) {}

        NodeWrapper(const float val, map::map_node *node, NodeWrapper *parent) :
                val_(val), node_(node), parent_(parent) {}

        void setDist(const float dist) { val_ = dist; }

        float getDist() const { return val_; }

        map::map_node *getNode() const { return node_; }

        void setParent(NodeWrapper &parent) { parent_ = &parent; }
        void setParent(NodeWrapper *parent) { parent_ = parent; }

        NodeWrapper *getParent() const { return parent_; }

        NodeWrapper &operator=(NodeWrapper const &) = default;

        friend bool operator<(const NodeWrapper &lhs, const NodeWrapper &rhs) {
            return lhs.getDist() < rhs.getDist();
        }

        friend bool operator>(const NodeWrapper &lhs, const NodeWrapper &rhs) {
            return lhs.getDist() > rhs.getDist();
        }

        friend bool operator==(const NodeWrapper &lhs, const NodeWrapper &rhs) {
            return (lhs.getNode() == rhs.getNode());
        }

        friend std::ostream &operator<<(std::ostream &os, const NodeWrapper &nodeWrapper) {
            return os << "Distance to node[" << nodeWrapper.getNode()->node_id() << ":"
                      << nodeWrapper.getParent()->getNode()->node_id() << "]: " << nodeWrapper.getDist();
        }

    private:

        float val_ = FLT_MAX;
        map::map_node *node_;
        NodeWrapper *parent_;
    };
}

#endif //UFO_MARTIJN_MAX_NODEWRAPPER_H
