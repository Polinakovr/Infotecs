#include <cstring>
#include <sys/types.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include "../lib/library.h"
int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)

    {
        std::cerr << "Ошибка возврата дескриптора гнезда" << std::endl;

        return -1;
    }
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(8080);
    if (inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr) <= 0)
    {
        close(sock);
    }
    if (bind(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
    {
        std::cerr << "Ошибка bind" << std::endl;
        close(sock);
        return -1;
    }

    if (listen(sock, 5) == -1)
    {
        std::cerr << "Ошибка listen" << std::endl;
        close(sock);
        return -1;
    }
    std::cout << "Ожидание подключения..." << std::endl;
    while (true)
    {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSock = accept(sock, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSock == -1)
        {
            std::cerr << "Ошибка accept" << std::endl;
            continue;
        }

        std::cout << "Клиент подключен." << std::endl;

        while (true)
        {
            int sum;
            int resize = recv(clientSock, &sum, sizeof(sum), 0);
            if (resize == -1)
            {
                std::cerr << "Ошибка recv" << std::endl;
                break;
            }
            else if (resize == 0)
            {

                std::cout << "Клиент отключен." << std::endl;
                break;
            }
            std::string data = std::to_string(sum);
            std::cout << "Строчка " << data << std::endl;
            bool result = analyzer(data);
            std::cout << "Результат analyzer: " << (result? "true" : "false") << std::endl;
        }
        close(clientSock);
        std::cout << "Соединение с клиентом закрыто." << std::endl;
    }

    close(sock);
    return 0;
}
