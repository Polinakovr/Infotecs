#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <mutex>
#include <arpa/inet.h>
#include <condition_variable>
#include <unistd.h>
#include "../lib/library.h"
#include <cstring>

std::vector<std::string> BUFFER;
std::mutex mtx;
std::condition_variable cv;
bool dataReady = false;
bool running = true;

int createSocket()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        std::cerr << "Ошибка возврата дескриптора гнезда" << std::endl;
        return -1;
    }
    struct sockaddr_in client;
    client.sin_family = AF_INET;
    client.sin_port = htons(8080);
    if (inet_pton(AF_INET, "127.0.0.1", &client.sin_addr) <= 0)
    {
        std::cerr << "Ошибка" << std::endl;
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&client, sizeof(client)) == -1)
    {  
        std::cerr << "Ошибка соединения" << std::endl;
        close(sock);
        return -1;
    }

    return sock;
}

void processing(int sock)
{
    while (running)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []
                { return dataReady || !BUFFER.empty(); });

        if (!BUFFER.empty())
        {
            std::string b = BUFFER.back();
            BUFFER.clear();
            dataReady = false;
            lock.unlock();
            int res = sum(b);
            std::cout << "Сумма чисел: " << res << std::endl;

            while (true)
            {
                if (send(sock, &res, sizeof(res), 0) == -1)
                {
                    std::cerr << "Ошибка отправки данных. Попытка переподключения..." << std::endl;
                    close(sock);
                    std::this_thread::sleep_for(std::chrono::seconds(1));

                    sock = createSocket();
                    if (sock == -1)
                    {
                        continue;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
}

bool validate(const std::string &str)
{
    auto result = std::find_if(str.begin(), str.end(), [](char ch)
                               { return !std::isdigit(ch); });
    if (result != str.end())
    {
        std::cerr << "Строка должна состоять только из цифр: ";
        return false;
    }
    else if (str.size() > 64)
    {
        std::cerr << "Строка не должна превышать 64 символа" << std::endl;
        return false;
    }
    return true;
}

void inputstr()
{
    std::cout << "\nВведите строку, состоящую только из цифр и не превышающую 64 символа." << std::endl;
    while (running)
    {

        std::string a;

        while (true)
        {
            std::getline(std::cin, a);
            if (validate(a))
            {
                break;
            }
        }

        sort_elem(a);
        std::cout << "Отсортированная строка: ";
        for (auto &symbol : a)
        {
            std::cout << symbol;
        }
        std::cout << std::endl;

        std::lock_guard<std::mutex> lock(mtx);
        BUFFER.push_back(a);
        dataReady = true;
        cv.notify_one();
    }
}

int main()
{
    int sock = createSocket();
    if (sock == -1)
    {
        return -1;
    }

    std::thread t1(inputstr);
    std::thread t2(processing, sock);

    t1.join();
    t2.join();

    std::string input;
    while (true)
    {
        std::getline(std::cin, input);
        if (input == "exit")
        {
            running = false;
            cv.notify_all();
            BUFFER.clear();
            break;
        }
    }

    close(sock);
    return 0;
}