#include <QCoreApplication>
#include <iostream>
#include <memory>
#include <tuple>
#include <string>
#include <stdexcept>
#include <set>

std::tuple<int, int> foo_tuple()
{
    return {1, -1};  // N4387 前错误
    return std::tuple<int, int>{1, -1};  // 始终有效
    return std::make_tuple(1, -1); // 始终有效
}

std::tuple<double, char, std::string> get_student(int id)
{
    if (id == 0) return std::make_tuple(3.8, 'A', "Lisa Simpson");
    if (id == 1) return std::make_tuple(2.9, 'C', "Milhouse Van Houten");
    if (id == 2) return std::make_tuple(1.7, 'D', "Ralph Wiggum");
    throw std::invalid_argument("id");
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    {
        auto student0 = get_student(0);
        std::cout << "ID: 0, "
                  << "GPA: " << std::get<0>(student0) << ", "
                  << "grade: " << std::get<1>(student0) << ", "
                  << "name: " << std::get<2>(student0) << '\n';

        double gpa1;
        char grade1;
        std::string name1;
        std::tie(gpa1, grade1, name1) = get_student(1);
        std::cout << "ID: 1, "
                  << "GPA: " << gpa1 << ", "
                  << "grade: " << grade1 << ", "
                  << "name: " << name1 << '\n';

        // C++17 结构化绑定：
        auto [gpa2, grade2, name2] = get_student(2);
        std::cout << "ID: 2, "
                  << "GPA: " << gpa2 << ", "
                  << "grade: " << grade2 << ", "
                  << "name: " << name2 << '\n';
    }
    {
        std::set<std::string> set_of_str;
        bool inserted = false;
        std::tie(std::ignore, inserted) = set_of_str.insert("Test");
        if (inserted) {
            std::cout << "Value was inserted successfully\n";
        }
    }
    return a.exec();
}
