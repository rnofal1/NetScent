/* This file contains definitions for the AStar class and Node helper class
 * Ramsey Nofal, 01/2024
 */

/* Local */
#include "AStar.h"
#include "util.h"

Node::Node(int index, double lat, double lon, QString name) :
    index(index),
    lat(lat),
    lon(lon),
    name(name) {
}

void Node::add_neighbor(const Node* neighbor) {
    neighbors.push_back(neighbor);
}

double Node::distance_to(const Node& other_node) {
    return gps_distance(this->lat, this->lon, other_node.lat, other_node.lon);
}
double Node::gps_distance(double lat_a, double lon_a, double lat_b, double lon_b) {
    double haversine;
    double temp;
    double dist;

    lat_a  = lat_a  * DEGREE_TO_RADIAN;
    lon_a = lon_a * DEGREE_TO_RADIAN;
    lat_b  = lat_b  * DEGREE_TO_RADIAN;
    lon_b = lon_b * DEGREE_TO_RADIAN;

    haversine = (pow(sin((1.0 / 2) * (lat_b - lat_a)), 2)) + ((cos(lat_a)) * (cos(lat_b)) * (pow(sin((1.0 / 2) * (lon_b - lon_a)), 2)));
    temp = 2 * asin(min(1.0, sqrt(haversine)));
    dist = EARTH_RADIUS * temp;

    return dist;
}

Node* Node::closest_node(std::vector<Node*>& graph) {
    Node* closest_node = nullptr;
    double closest_dist = DBL_MAX;

    for(auto& curr_node : graph) {
        double curr_dist = distance_to(*curr_node);
        qDebug() << "Distance between " << name << " and " << curr_node->name << " is: " << curr_dist;
        if(curr_dist < closest_dist && curr_node != this) {
            closest_node = curr_node;
            closest_dist = curr_dist;
        }
    }

    if(closest_node == nullptr) {
        qDebug() << "No closest node to node with name: " << this->name;
    } else {
        qDebug() << "Closest node to " << this->name << " is " << closest_node->name;
    }

    return closest_node;
}

QString Node::get_info() {
    QString info = "Node #";
    info += QString::number(index);
    info += ", name: " + name;
    info += ", latitude: " + QString::number(lat);
    info += ", longitude: " + QString::number(lon);

    return info;
}


AStar::AStar() {
    graph = create_abridged_us_graph();
    place_my_location_in_graph();
}

void AStar::place_my_location_in_graph() {
    auto coords = get_my_lat_lon();
    double lat = coords.first;
    double lon = coords.second;

    my_location = new Node((int)graph.size(), lat, lon, "My Location");
    name_to_graph_index[my_location->name] = my_location->index;
    graph.push_back(my_location);

    auto closest_node_in_graph = my_location->closest_node(graph);
    if(closest_node_in_graph != nullptr) {
        connect_node_pair(my_location, closest_node_in_graph);
    }

//    my_location->add_neighbor(graph[name_to_graph_index["Denver"]]);
//    graph[name_to_graph_index["Denver"]]->add_neighbor(my_location);
}

void AStar::print_graph() {
    for(const auto& node : graph) {
        qDebug() << node->get_info();
    }
}

//Manually-created list of nodes
//ToDo: read this from a database
std::vector<Node*> AStar::create_map_nodes() {
    int index = 0;
    std::vector<Node*> nodes = {
        new Node(index++, 37.335480, -121.893028, "San Jose"),
        new Node(index++, 34.052235, -118.243683, "Los Angeles"),
        new Node(index++, 32.715736, -117.161087, "San Diego"),
        new Node(index++, 33.448376, -112.074036, "Phoenix"),
        new Node(index++, 29.424349, -98.491142, "San Antonio"),
        new Node(index++, 29.716131, -95.388718, "Houston"),
        new Node(index++, 32.779167, -96.808891, "Dallas"),
        new Node(index++, 36.174465, -86.767960, "Nashville"),
        new Node(index++, 25.782721, -80.140556, "Miami"),
        new Node(index++, 39.952583, -75.165222, "Philadelphia"),
        new Node(index++, 40.730610, -73.935242, "New York"),
        new Node(index++, 41.8781, -87.6298, "Chicago"),
        new Node(index++, 41.257160, -95.995102, "Omaha"),
        new Node(index++, 39.742043, -104.991531, "Denver")
    };

    for(const auto& node : nodes) {
        name_to_graph_index[node->name] = node->index;
    }

    return nodes;
}
//Manually/arbitrarily connect our nodes
void AStar::connect_all_us_map_nodes(std::vector<Node*>& us_graph) {
    connect_node_pair(us_graph[name_to_graph_index["San Jose"]], us_graph[name_to_graph_index["Los Angeles"]]);
    connect_node_pair(us_graph[name_to_graph_index["Los Angeles"]], us_graph[name_to_graph_index["San Diego"]]);
    connect_node_pair(us_graph[name_to_graph_index["San Diego"]], us_graph[name_to_graph_index["Phoenix"]]);

    connect_node_pair(us_graph[name_to_graph_index["Phoenix"]], us_graph[name_to_graph_index["Denver"]]);
    connect_node_pair(us_graph[name_to_graph_index["Phoenix"]], us_graph[name_to_graph_index["San Antonio"]]);

    connect_node_pair(us_graph[name_to_graph_index["San Antonio"]], us_graph[name_to_graph_index["Houston"]]);
    connect_node_pair(us_graph[name_to_graph_index["San Antonio"]], us_graph[name_to_graph_index["Dallas"]]);

    connect_node_pair(us_graph[name_to_graph_index["Houston"]], us_graph[name_to_graph_index["Dallas"]]);

    connect_node_pair(us_graph[name_to_graph_index["Dallas"]], us_graph[name_to_graph_index["Nashville"]]);
    connect_node_pair(us_graph[name_to_graph_index["Dallas"]], us_graph[name_to_graph_index["Omaha"]]);

    connect_node_pair(us_graph[name_to_graph_index["Nashville"]], us_graph[name_to_graph_index["Philadelphia"]]);
    connect_node_pair(us_graph[name_to_graph_index["Nashville"]], us_graph[name_to_graph_index["Miami"]]);
    connect_node_pair(us_graph[name_to_graph_index["Nashville"]], us_graph[name_to_graph_index["Chicago"]]);

    connect_node_pair(us_graph[name_to_graph_index["Philadelphia"]], us_graph[name_to_graph_index["New York"]]);
    connect_node_pair(us_graph[name_to_graph_index["Philadelphia"]], us_graph[name_to_graph_index["Chicago"]]);

    connect_node_pair(us_graph[name_to_graph_index["Chicago"]], us_graph[name_to_graph_index["Omaha"]]);
    connect_node_pair(us_graph[name_to_graph_index["Omaha"]], us_graph[name_to_graph_index["Denver"]]);
}
void AStar::connect_node_pair(Node* a, Node* b) {
    edges.push_back(std::make_pair(a, b));
    a->add_neighbor(b);
    b->add_neighbor(a);
}

//Completely arbitrary Node graph
std::vector<Node*> AStar::create_abridged_us_graph() {
    std::vector<Node*> us_graph = create_map_nodes();
    connect_all_us_map_nodes(us_graph);
    return us_graph;
}

std::vector<std::vector<double>> AStar::find_shortest_path(const QString& start, const QString& goal) {
    qDebug() << "IN MAP: " << start << " " << name_to_graph_index[start];
    return find_shortest_path(name_to_graph_index[start], name_to_graph_index[goal]);
}
std::vector<std::vector<double>> AStar::find_shortest_path(int start_index, int goal_index) {
    qDebug() << "Start: " << start_index << " Goal: " << goal_index << " " << graph[13]->neighbors.size();
    std::unordered_map<int, int> came_from;

    std::unordered_map<int, int> g_score;
    for(int i = 0; i < graph.size(); ++i) {
        g_score[i] = INT_MAX;
    }
    g_score[start_index] = 0;

    std::unordered_map<int, int> f_score;
    for(int i = 0; i < graph.size(); ++i) {
        f_score[i] = 0;
    }
    f_score[start_index] = 0;

    //set sorting
    auto comp = [&](int a, int b) {return f_score[a] < f_score[b]; };
    auto open_set = std::set<int, decltype(comp)>(comp);
    open_set.insert(start_index);

    //main loop
    while(!open_set.empty()) {
        int current_index = *open_set.begin();
        if(current_index == goal_index) {
            return reconstruct_path(came_from, current_index);
        }

        open_set.erase(current_index);
        Node* current_node = graph[current_index];
        for(auto& neighbor : current_node->neighbors) {
            int neighbor_index = neighbor->index;

            int tentative_g_score = g_score[current_index] + current_node->distance_to(*neighbor);
            if(tentative_g_score < g_score[neighbor_index]) {
                came_from[neighbor_index] = current_index;
                g_score[neighbor_index] = tentative_g_score;
                f_score[neighbor_index] = tentative_g_score + 0;
                if(open_set.find(neighbor_index) == open_set.end()) {
                    open_set.insert(neighbor_index);
                }
            }

        }
    }

    qDebug() << "No path found";
    return {};
}

std::vector<std::vector<double>> AStar::reconstruct_path(std::unordered_map<int, int>& came_from, int current) {
    //Create path of indices
    std::vector<int> index_path = {current};
    while(came_from.find(current) != came_from.end()) {
        current = came_from[current];
        index_path.insert(index_path.begin(), current);
    }

    //Path of indices -> path of coords
    std::vector<std::vector<double>> coord_path; //in order
    qDebug() << "Path:";
    for(const auto& index : index_path) {
        Node* curr_node = graph[index];
        coord_path.push_back({curr_node->lat, curr_node->lon});
        qDebug() << coord_path[coord_path.size() - 1][0] << " " << coord_path[coord_path.size() - 1][1];
    }

    return coord_path;
}
