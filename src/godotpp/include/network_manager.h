//
// Created by totod on 16/02/2026.
//

#ifndef GODOTPP_NETWORK_MANAGER_H
#define GODOTPP_NETWORK_MANAGER_H
#include <functional>
#include <snl.h>
#include <godot_cpp/classes/packed_scene.hpp>

#include "godot_cpp/classes/node.hpp"


using CreationLambda = std::function<godot::Node*()>;


namespace godot {
class NetworkManager : public Node {
    GDCLASS(NetworkManager, Node)

public:
    // NetID -> Node
    std::unordered_map<uint32_t, godot::Node*> network_to_node_map;

    // TypeID -> Fonction de création
    std::unordered_map<uint32_t, CreationLambda> type_registry;

    // Socket
    GameSocket* client_socket = nullptr;

    int32_t client_id = -1;

    Ref<PackedScene> player_scene;
    void set_player_scene(const Ref<PackedScene>& p_scene) { player_scene = p_scene; }
    Ref<PackedScene> get_player_scene() const { return player_scene; }

protected:
    static void _bind_methods();

public:
    NetworkManager();
    ~NetworkManager();
    void _ready() override;
    void _process(double p_delta) override;

    void register_type(uint32_t type_id, CreationLambda creator) {
        type_registry[type_id] = creator;
    }

    void poll_network();

    void handleHelo(int bytes_received, uint8_t* buffer);

    void handleSpawn(int bytes_received, uint8_t* buffer);

    void handlePing(int bytes_received, uint8_t* buffer);

    void spawn_entity(uint32_t net_id, uint32_t type_id);
};
}


#endif //GODOTPP_NETWORK_MANAGER_H