#include "vfs.hpp"
#include <iostream>
#include <stdexcept>

int main() {
    vfs::File f("a.txt", "123");
    std::cout << f.name() << " " << f.size() << "\n";    // a.txt 3
    f.write("hello");
    std::cout << f.size() << "\n";                        // 5
    std::unique_ptr<vfs::Entry> p = f.clone();
    std::cout << p->name() << " " << p->size() << "\n";   // a.txt 5
    try { vfs::File bad("a/b"); } catch (const std::invalid_argument&) { std::cout << "ok\n"; }
}