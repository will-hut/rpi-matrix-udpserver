#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "led-matrix.h"
#include "asio.hpp"

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

volatile bool interrupt_received = false;

static void InterruptHandler(int signo) {
  interrupt_received = true;
}

int main(int argc, char *argv[]) {
    RGBMatrix::Options defaults;
    defaults.hardware_mapping = "regular";  // or e.g. "adafruit-hat"
    defaults.rows = 32;
    defaults.chain_length = 1;
    defaults.parallel = 1;
    defaults.show_refresh_rate = true;
    Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults);

    // Create 
    std::uint16_t port = 1337;
    asio::io_context io_context;
    asio::ip::udp::endpoint receiver(asio::ip::udp::v4(), port);
    asio::ip::udp::socket socket(io_context, receiver);

    std::cout << "Bound to port" << std::to_string(port) << std::endl;

    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    // Grab width and height
    int width = canvas->width();
    int height = canvas->height();

    while(!interrupt_received)
    {
        // Recieve UDP packet
        char buffer[65536];
        asio::ip::udp::endpoint sender;
        std::size_t bytes_transferred = socket.receive_from(asio::buffer(buffer), sender);

        // Write buffer to screen
        int i = 0;
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                canvas->SetPixel(x, y, buffer[i], buffer[i+1], buffer[i+2]);
                i += 3;
            }
        }
    }

    // Shutdown RGB matrix
    canvas->Clear();
    delete canvas;

    std::cout << "Program terminated." << std::endl;
    return 0;
}