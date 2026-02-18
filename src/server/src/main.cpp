#include "godotServer.h"

int main() {
    godotServer server("127.0.0.1:8765");
    server.start();
}