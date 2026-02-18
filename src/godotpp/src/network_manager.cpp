//
// Created by totod on 16/02/2026.
//

#include "../include/network_manager.h"

#include <iostream>
#include <snl.h>
#include "../../SharedDefinition.h"

void godot::NetworkManager::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_player_scene", "p_scene"), &NetworkManager::set_player_scene);
    ClassDB::bind_method(D_METHOD("get_player_scene"), &NetworkManager::get_player_scene);

    // 2. On ajoute la propriété à l'inspecteur
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "player_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_player_scene", "get_player_scene");

}

godot::NetworkManager::NetworkManager() {
    // print helo message to console
    std::cout << "NetworkManager initialized!" << std::endl;
}

godot::NetworkManager::~NetworkManager() {
}

void godot::NetworkManager::_ready() {
    Node::_ready();


    // --- ENREGISTREMENT DES TYPES ---

    // 1 -> Player
    if (player_scene.is_null()) {
        UtilityFunctions::print_rich("[color=red]Erreur : Player Scene non assignée dans le NetworkManager ![/color]");
        return;
    }
    register_type(1, [this]() -> Node* {
        Node* new_node = player_scene->instantiate();
        return new_node;
    });


    // on envoye un HELO au serv
    client_socket = net_socket_create("127.0.0.1:0");
    uint8_t HeloMess[1] = {static_cast<uint8_t>(MessageType::HELO)};
    net_socket_send(client_socket, "127.0.0.1:8765", HeloMess, sizeof(HeloMess));
}

void godot::NetworkManager::_process(double p_delta) {
    Node::_process(p_delta);
    poll_network();
}

void godot::NetworkManager::poll_network() {
    uint8_t buffer[1024];
    char sender[256];
    int bytes_received = 0;

    bytes_received = net_socket_poll(client_socket, buffer, sizeof(buffer), sender, sizeof(sender));

    if (bytes_received > 0) {
        switch (static_cast<MessageType>(buffer[0])) {

            case MessageType::HELO:
                handleHelo(bytes_received, buffer);
                break;

            case MessageType::SPAWN:
                handleSpawn(bytes_received, buffer);
                break;

            case MessageType::PING:


            default:
                UtilityFunctions::print("Received unknown message type: ", static_cast<int>(buffer[0]));
                break;
        }
    }
}

void godot::NetworkManager::handleHelo(int bytes_received, uint8_t* buffer) {
    UtilityFunctions::print("Handling HELO message");
    if (bytes_received != 5) {
        UtilityFunctions::print("Invalid HELO message size: ", bytes_received);
        return;
    }
    std::memcpy(&client_id, &buffer[1], sizeof(client_id));
    UtilityFunctions::print("Received HELO message. Assigned Client ID: ", client_id);
}

void godot::NetworkManager::handleSpawn(int bytes_received, uint8_t* buffer) {
    if (bytes_received != 9) {
        UtilityFunctions::print("Invalid SPAWN message size: ", bytes_received);
        return;
    }

    uint32_t net_id;
    std::memcpy(&net_id, &buffer[1], sizeof(net_id));

    uint32_t type_id;
    std::memcpy(&type_id, &buffer[5], sizeof(type_id));

    spawn_entity(net_id, type_id);

    UtilityFunctions::print("Handling SPAWN message. NetID: ", net_id, " TypeID: ", type_id);
}

void godot::NetworkManager::handlePing(int bytes_received, uint8_t* buffer) {
    if (bytes_received != 9) {
        UtilityFunctions::print("Invalid PING message size: ", bytes_received);
        return;
    }

}


void godot::NetworkManager::spawn_entity(uint32_t net_id, uint32_t type_id) {
    if (network_to_node_map.count(net_id) > 0) return; // Déjà existe

    if (type_registry.count(type_id)) {
        Node *new_node = type_registry[type_id]();
        add_child(new_node);
        network_to_node_map[net_id] = new_node;

        UtilityFunctions::print("Spawned Entity NetID: ", net_id);
    }
}
