#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

using namespace std;

string getHoroscopeFromAPI(const string& sign, const string& day) {
    string command = "curl -s \"https://horoscope-app-api.vercel.app/api/v1/get-horoscope/daily?sign="
                     + sign + "&day=" + day + "\"";
    string result;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "Error fetching data.";

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    cout << "Сервер очікує з'єднання на порту 8080..." << endl;

    while ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) > 0) {
        char buffer[1024] = {0};
        read(new_socket, buffer, 1024);

        istringstream iss(buffer);
        string sign, day;
        iss >> sign >> day;

        string response = getHoroscopeFromAPI(sign, day);
        send(new_socket, response.c_str(), response.size(), 0);
        close(new_socket);
    }

    return 0;
}

