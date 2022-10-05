#include <iostream>
#include "Queue.hpp"
#include <algorithm>
#include <iterator>

int main()
{
    Queue<std::string> idols;
    try {
        idols.dequeue();
    }
    catch (const std::out_of_range& oof) {
        std::cerr << oof.what() << std::endl;
    }
    idols.enqueue("Tachibana Arisu");
    idols.enqueue("Sasaki Chie");
    idols.enqueue("Satou Shin");
    idols.enqueue("Katagiri Sanae");
    idols.enqueue("Akizuki Ritsuko");
    idols.enqueue("Shijou Takane");
    idols.enqueue("Hakozaki Serika");

    Queue<std::string> others;
    others = idols;
    others.enqueue("Shirase Sakuya");
    others.enqueue("Sonoda Chiyoko");
    others.enqueue("Morikubo Nono");
    others.enqueue("Sakuramori Kaori");

    std::cout << idols.peek() << '\n';
    std::cout << others.peek() << "\n\n\n";    

    std::sort(idols.begin(), idols.end());
    std::sort(others.begin(), others.end());
    idols.dequeue();
    idols.dequeue();
    others.dequeue();
    others.dequeue();

    std::ostream_iterator<decltype(idols.peek())> os(std::cout, "\n");
    std::copy(idols.begin(), idols.end(), os);
    std::cout << std::endl;
    std::copy(others.begin(), others.end(), os);
}