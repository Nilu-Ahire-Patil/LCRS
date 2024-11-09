// client.cpp
#include <boost/asio.hpp>
#include <iostream>
#include <fstream>

using boost::asio::ip::tcp;
const char* SERVER_IP = "127.0.0.1";
const int PORT = 12345;

void send_file(const std::string& filename, tcp::socket& socket) {
    try {
        std::ifstream infile(filename, std::ios::binary);
        if (!infile.is_open()) {
            std::cerr << "Could not open file: " << filename << "\n";
            return;
        }

        // Send the filename first
        boost::asio::write(socket, boost::asio::buffer(filename + "\n"));

        // Send the file content
        char buffer[1024];
        while (infile.read(buffer, sizeof(buffer)) || infile.gcount() > 0) {
            boost::asio::write(socket, boost::asio::buffer(buffer, infile.gcount()));
        }

        std::cout << "File sent successfully: " << filename << std::endl;
        infile.close();
    } catch (const std::exception& e) {
        std::cerr << "Error sending file: " << e.what() << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: client <filename>\n";
        return 1;
    }

    try {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(SERVER_IP, std::to_string(PORT));
        tcp::socket socket(io_context);

        boost::asio::connect(socket, endpoints);

        send_file(argv[1], socket);
    } catch (const std::exception& e) {
        std::cerr << "Client error: " << e.what() << "\n";
    }

    return 0;
}
