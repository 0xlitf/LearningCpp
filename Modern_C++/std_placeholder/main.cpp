#include <QCoreApplication>
#include <iostream>
#include <memory>
#include <tuple>
#include <string>
#include <stdexcept>

// 可变参数模板
template <typename... Args>
void PrintArgs(Args... args) {
    std::cout << "\n";
    (std::cout << ... << args);
    std::cout << "\n";

    (std::cout << ... << args) << " - ";
    std::cout << "\n";
}


// 类模板的可变参数
template <typename... Types>
class Tuple {
public:
    Tuple(Types... args) {
        // 在构造函数中使用参数包初始化元组
    }

private:
         // 元组的成员变量
};


// 递归模板
template <typename T>
void Process(T value) {
    // 处理最后一个参数
    std::cout << value << std::endl;
}

template <typename T, typename... Rest>
void Process(T value, Rest... rest) {
    // 处理第一个参数 'value'
    std::cout << value << ", ";
    // 递归处理剩余的参数 'rest'
    Process(rest...);
}


// 参数展开（Parameter Pack Expansion）
//template <typename... Args>
//void Process(Args... args) {
//    (DoSomething(args), ...);  // 展开参数包，对每个参数执行 DoSomething 操作
//}


//



int main(int argc, char *argv[])
{
    //    QCoreApplication a(argc, argv);

    PrintArgs("1", "2");

    //    return a.exec();
    return 0;
}
