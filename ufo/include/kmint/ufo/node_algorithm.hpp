#ifndef UFO_NODE_ALGORITHM_HPP
#define UFO_NODE_ALGORITHM_HPP

#include "kmint/map/map.hpp"
#include "kmint/ufo/NodeWrapper.h"
#include "kmint/ufo/PathWrapper.h"

namespace kmint::ufo {
    ///
    /// Represents possible heuristics able to be calculated
    ///
    enum Heuristic {
        MANHATTAN, DIAGONAL, EUCLIDEAN, DIJKSTRA
    };

    ///
    /// Gets the number of periods for which an entity has to wait before
    /// it can move to an adjacent node
    ///
    double waiting_time(map::map_node const &node);

    ///
    /// Finds a random node adjacent to argument node
    ///
    map::map_node const &random_adjacent_node(map::map_node const &node);

    ///
    /// Finds a node of a given kind (which is the character by which it is
    /// represented textually)
    ///
    /// You must take care that a node of the given kind exists. Otherwise the
    /// behaviour of this function is undefined.
    ///
    map::map_node &find_node_of_kind(map::map_graph const &graph, char kind);

    map::map_node &random_node_of_kind(map::map &map, char kind);

    map::map_node const &random_node_of_kind(map::map const &map, char kind);

    ///
    /// Finds the closest node to a location
    ///
    map::map_node const &find_closest_node_to(map::map_graph const &graph, math::vector2d location);

    NodeWrapper* findNode(NodeWrapper* node, std::vector<NodeWrapper*> &openList);

    /// Get the node wrapper with the smallest fCost
    NodeWrapper* getMinNode(std::vector<NodeWrapper*> &openList);

    /// Moves node from openList to closedList
    /// Helper function for a*
    /// \param node to be moved
    /// \param openList ref to openList
    /// \param closedList ref to closedList
    void moveNodeToClosed(NodeWrapper* node, std::vector<NodeWrapper*> &openList, std::vector<NodeWrapper*> &closedList);

    /// Calculate heuristic using method h
    /// \param h heuristic calculation method
    /// \param x current x position
    /// \param y current y position
    /// \param goal_x goal x position
    /// \param goal_y goal y position
    /// \return calculated heuristic as float
    float calculate_heuristic(Heuristic h, float x, float y, float goal_x = -1, float goal_y = -1);

    float calculate_heuristic(Heuristic h, map::map_node const &loc, map::map_node const &destLoc);

    PathWrapper tag_shortest_path_astar(
            Heuristic heuristic,
            map::map_node const &actorLoc,
            map::map_node const &goalLoc,
            map::map_graph &graph);

} // namespace kmint::ufo

#endif /* UFO_NODE_ALGORITHM_HPP */
