#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

int main() {
    string sign, day;
    cout << "Введіть знак зодіаку англійською (наприклад, Pisces): ";
    cin >> sign;
    cout << "Введіть дату (наприклад, 2025-04-16): ";
    cin >> day;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Помилка з'єднання з сервером." << endl;
        return 1;
    }

    string message = sign + " " + day;
    send(sock, message.c_str(), message.size(), 0);

    char buffer[4096] = {0};
    read(sock, buffer, 4096);
    cout << "Отримано гороскоп:\n" << buffer << endl;

    close(sock);
    return 0;
}

