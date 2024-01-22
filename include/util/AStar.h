/* This file contains declarations for the AStar class and Node helper class
 * Ramsey Nofal, 1/2024
 */


#ifndef ASTAR_H
#define ASTAR_H

/* Standard C/C++ */
#include <vector>
#include <iostream>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <math.h>
#include <float.h>

/* Standard Qt */
#include <QString>
#include <QDebug>

#define PI 3.14159265358979323846
#define EARTH_RADIUS 6372797.56085
#define DEGREE_TO_RADIAN PI / 180

class Node {
public:
    int index;
    double lat;
    double lon;
    QString name;
    std::vector<const Node*> neighbors;

    Node(int index, double lat, double lon, QString name);
    void add_neighbor(const Node* neighbor);
    double distance_to(const Node& other_node);
    double gps_distance(double lat_a, double lon_a, double lat_b, double lon_b);

    Node* closest_node(std::vector<Node*>& graph);

    QString get_info();
};

class AStar {
public:
    AStar();

    std::vector<Node*> graph;
    std::vector<std::pair<Node*, Node*>> edges;

    //Place MyLocation Within Graph, connect to closest
    void place_my_location_in_graph();

    void print_graph();

    std::vector<Node*> create_map_nodes();
    void connect_all_us_map_nodes(std::vector<Node*>& us_graph);
    void connect_node_pair(Node* a, Node* b);
    std::vector<Node*> create_abridged_us_graph();

    std::vector<std::vector<double>> reconstruct_path(std::unordered_map<int, int>& came_from, int current);
    std::vector<std::vector<double>> find_shortest_path(int start_index, int goal_index);
    std::vector<std::vector<double>> find_shortest_path(const QString& start, const QString& goal);

private:
    Node* my_location;
    std::map<QString, int> name_to_graph_index;
};


#endif // ASTAR_H
