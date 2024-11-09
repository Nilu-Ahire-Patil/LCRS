// server.cpp
#include <boost/asio.hpp>
#include <iostream>
#include <fstream>

using boost::asio::ip::tcp;
const int PORT = 12345;

void handle_client(tcp::socket& socket) {
    try {
        // Create a buffer to receive data
        boost::asio::streambuf buf;
        boost::system::error_code error;

        // Read the filename first
        boost::asio::read_until(socket, buf, "\n", error);
        std::istream is(&buf);
        std::string filename;
        std::getline(is, filename);
        
        if (error) throw boost::system::system_error(error);

        std::ofstream outfile("received_" + filename, std::ios::binary);
        if (!outfile.is_open()) {
            std::cerr << "Error opening file for writing\n";
            return;
        }

        // Read the file content
        while (boost::asio::read(socket, buf, boost::asio::transfer_at_least(1), error)) {
            if (error && error != boost::asio::error::eof) throw boost::system::system_error(error);
            outfile << &buf;
        }

        std::cout << "File received successfully: " << filename << std::endl;
        outfile.close();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), PORT));

        std::cout << "Server is listening on port " << PORT << std::endl;
        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::cout << "Client connected.\n";
            handle_client(socket);
        }
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << "\n";
    }

    return 0;
}
