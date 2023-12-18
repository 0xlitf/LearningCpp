#include <QCoreApplication>
#include <iostream>
#include <memory>
#include <iomanip>
#include <QDebug>

#define print(var)                                                          \
    std::cout << std::setw(6) << #var << ": " << var << std::endl;          \
    if (var)                                                                \
    {                                                                       \
        std::cout << std::setw(6) << "*" #var << ": " << *var << std::endl; \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        std::cout << std::setw(6) << ""                                     \
                  << "nullptr" << std::endl;                                \
    }

std::unique_ptr<int> get_unique()
{
    std::unique_ptr<int> u{new int(5)};
    return u;
}

std::shared_ptr<int> get_shared()
{
    std::shared_ptr<int> s{new int()};
    std::cout << "get_shared, addr: " << s.use_count() << std::endl;
    return s;
}

void deliver_by_value(std::unique_ptr<int> s_ptr)
{
    std::cout << std::setw(6) << "deliver_by_value use_count: " << s_ptr << std::endl;
}

void deliver_by_ref(const std::unique_ptr<int> &s_ptr)
{
    std::cout << std::setw(6) << "deliver_by_ref use_count: " << s_ptr << std::endl;
}

void deliver_by_value(std::shared_ptr<int> s_ptr)
{
    std::cout << std::setw(6) << "deliver_by_value use_count: " << s_ptr.use_count() << std::endl;
}

void deliver_by_ref(const std::shared_ptr<int> &s_ptr)
{
    std::cout << std::setw(6) << "deliver_by_ref use_count: " << s_ptr.use_count() << std::endl;
}

template <typename T, typename... Ts>
std::unique_ptr<T> make_unique_implement(Ts &&...params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (!qEnvironmentVariableIsEmpty("VSAPPIDDIR")) { // start direct in vs
        qSetMessagePattern("%{file}(%{line}): "
                           "[%{type}]:"
                           " [%{function}]:\n"
                           " [%{time yyyy-MM-dd h:mm:ss.zzz t}]"
                           " %{message}");
        qDebug() << "start in vs: " << qgetenv("VSAPPIDDIR");
    } else if (!qEnvironmentVariableIsEmpty("QTDIR")) { // start direct in QtCreator
        qSetMessagePattern("\033[1;37m [file://%{file}:%{line}]:\033[0m"
                           " %{if-debug}\033[1;36m%{endif}%{if-info}\033[1;44m%{endif}%{if-warning}\033[1;43m%{endif}%{if-critical}\033[1;41m%{endif}%{if-fatal}\033[1;41m%{endif}[%{type}]:\033[0m"
                           "\033[1;35m [%{function}]:\033[0m\n"
                           "\033[0;37m [%{time yyyy-MM-dd h:mm:ss.zzz t}]\033[0m"
                           "\033[1;91m %{message}\033[0m");
        qDebug() << "start in QtCreator: " << qgetenv("QTDIR");
    } else {
        // qInstallMessageHandler(logger);
    }

    {
        // init style

        std::unique_ptr<int> ptr_0 =
            std::make_unique<int>(0); // std::make_unique C++14, make_unique_implement
        print(ptr_0);

        // init style
        std::unique_ptr<int> ptr_1(new int(1));
        print(ptr_1);

        // init & reset style
        std::unique_ptr<int> ptr_2;
        print(ptr_2);
        ptr_2.reset(new int{2});
        print(ptr_2);

        ptr_2.reset(0);
        print(ptr_2);

        std::unique_ptr<int> ptr_3(new int{});

        /*  error:
            std::unique_ptr<int> ptr_4(ptr_3);
         *  unique_ptr没有提供拷贝构造函数, 只提供了移动构造函数, 不共享各自拥有的堆内存
         *  和 shared_ptr 指针不同，为 unique_ptr 自定义释放规则，只能采用函数对象的方式 */

        std::unique_ptr<int> ptr_5(std::move(ptr_3));

        struct MyDelete
        {
            void operator()(int *p6)
            {
                print(p6);
                delete p6;
            }
        };

        std::unique_ptr<int, MyDelete> ptr_6(new int{}, MyDelete());
        std::unique_ptr<int, std::function<void(int *)>> ptr_7(new int, [](int *p7)
                                                               {
            print(p7);
            delete p7; });

        *ptr_7 = 10;
        int *ptr_8 = ptr_7.release(); // 释放当前 unique_ptr
        // 指针对所指堆内存的所有权，但该存储空间并不会被销毁
        print(ptr_7);
        print(ptr_8);

        std::unique_ptr<int, MyDelete> ptr_9;
        ptr_9.reset(ptr_8);
        print(ptr_9);
        print(ptr_8);
        *ptr_8 = 0;
        print(ptr_9);
        print(ptr_8);

        int *ptr_10 = new int(10); // 利用传统指针在堆区开辟一个存放整数的区域
        print(ptr_10);
        std::unique_ptr<int> ptr_11{ptr_10}; // 通过传统指针创建智能指针
        print(ptr_11);
        std::unique_ptr<int> ptr_12{std::move(ptr_11)}; // 有意让两个独占指针同时指向同一个内存区域
        print(ptr_11);
        print(ptr_12);
        // error: deliver_by_value(ptr_12);
        deliver_by_ref(ptr_12);

        std::cout << "get_unique, addr 1: " << get_unique() << ", value: " << *get_unique() << std::endl;
        std::cout << "get_unique, addr 2: " << get_unique() << ", value: " << *get_unique() << std::endl;
    }

    {
        std::shared_ptr<int> s_ptr_1 = std::make_shared<int>(10);
        std::cout << std::setw(6) << "s_ptr_1 use_count: " << s_ptr_1.use_count() << std::endl;
        deliver_by_value(s_ptr_1);
        std::cout << std::setw(6) << "s_ptr_1 use_count: " << s_ptr_1.use_count() << std::endl;
        deliver_by_ref(s_ptr_1); // 使用引用传递shared_ptr将导致引用计数不增加
        std::cout << std::setw(6) << "s_ptr_1 use_count: " << s_ptr_1.use_count() << std::endl;

        auto &&s1 =
            get_shared(); // 函数的返回值是通过复制的方式，所以不会增加引用计数，而且函数中创建的对象也不会被删除
        std::cout << "get_shared, addr 1: " << s1.use_count() << std::endl;
        auto &&s2 = get_shared();
        std::cout << "get_shared, addr 2: " << s2.use_count() << std::endl;

        std::shared_ptr<int> s_ptr_2{new int(2)};
        std::cout << "s_ptr_2: " << s_ptr_2.use_count() << std::endl;
        std::shared_ptr<int> s_ptr_2_copy = s_ptr_2;
        std::cout << "s_ptr_2_copy: " << s_ptr_2_copy.use_count() << std::endl;
        s_ptr_2 = nullptr; // 清空指针, 计数-1
        std::cout << "s_ptr_2_copy: " << s_ptr_2_copy.use_count() << std::endl;

        {
            std::shared_ptr<std::function<void()>> f1(new std::function<void()>([=]() { 
                qDebug() << "new 1"; 
                }),
                [](std::function<void()> *p) {
                    qDebug() << "delete 1";
                });
            (*f1)();
        }

        {
            class CircularRef
            {
            public:
                void set_friend(std::shared_ptr<CircularRef> p)
                {
                    m_friend = p;
                }

            private:
                std::shared_ptr<CircularRef> m_friend; // 在代码块的结尾不会释放
            };

            std::shared_ptr<CircularRef> s_ptr_1 = std::make_shared<CircularRef>();
            std::shared_ptr<CircularRef> s_ptr_2 = std::make_shared<CircularRef>();
            std::cout << "s_ptr_1.use_count: " << s_ptr_1.use_count() << std::endl;
            std::cout << "s_ptr_2.use_count: " << s_ptr_2.use_count() << std::endl;
            s_ptr_1->set_friend(s_ptr_2);
            s_ptr_2->set_friend(s_ptr_1);
            std::cout << "s_ptr_1.use_count: " << s_ptr_1.use_count() << std::endl;
            std::cout << "s_ptr_2.use_count: " << s_ptr_2.use_count() << std::endl;
        }

        {
            class CircularRef
            {
            public:
                void set_friend(std::shared_ptr<CircularRef> p)
                {
                    m_friend = p;
                }

                std::shared_ptr<CircularRef> lock_friend()
                {
                    return m_friend.lock(); // lock可将weak_ptr升级为shared_ptr, 增加引用计数
                }

            private:
                std::weak_ptr<CircularRef> m_friend; // weak_ptr不会增加引用计数
            };

            std::shared_ptr<CircularRef> s_ptr_1 = std::make_shared<CircularRef>();
            std::shared_ptr<CircularRef> s_ptr_2 = std::make_shared<CircularRef>();
            std::cout << "s_ptr_1.use_count: " << s_ptr_1.use_count() << std::endl;
            std::cout << "s_ptr_2.use_count: " << s_ptr_2.use_count() << std::endl;
            s_ptr_1->set_friend(s_ptr_2);
            s_ptr_2->set_friend(s_ptr_1);
            std::cout << "s_ptr_1.use_count: " << s_ptr_1.use_count() << std::endl;
            std::cout << "s_ptr_2.use_count: " << s_ptr_2.use_count() << std::endl
                      << std::endl;

            s_ptr_1->lock_friend(); // 并未构造新的shared_ptr, 引用计数不变
            std::cout << "s_ptr_1.use_count: " << s_ptr_1.use_count() << std::endl;
            std::cout << "s_ptr_2.use_count: " << s_ptr_2.use_count() << std::endl
                      << std::endl;

            std::shared_ptr<CircularRef> s_ptr_3 =
                s_ptr_1->lock_friend(); // 从s_ptr_2构造新的shared_ptr:s_ptr_3, 引用计数+1
            std::cout << "s_ptr_1.use_count: " << s_ptr_1.use_count() << std::endl;
            std::cout << "s_ptr_2.use_count: " << s_ptr_2.use_count() << std::endl;
            std::cout << "s_ptr_3.use_count: " << s_ptr_3.use_count() << std::endl
                      << std::endl;
        }

        std::shared_ptr<int> s_ptr_4 = std::make_shared<int>(14);
        {
            std::weak_ptr<int> w_ptr_4 =
                s_ptr_4; // 只是监测/观察资源, 并不管理资源 error: << *w_ptr_4
            std::cout << "w_ptr_4.use_count: " << w_ptr_4.use_count() << ", &w_ptr_4: " << &w_ptr_4
                      << std::endl;
            std::cout << "s_ptr_4.use_count: " << s_ptr_4.use_count() << ", &s_ptr_4: " << &s_ptr_4
                      << ", s_ptr_4: " << s_ptr_4 << std::endl;

            std::shared_ptr<int> s_ptr_5 = w_ptr_4.lock();
            s_ptr_4 = nullptr;

            std::cout << "w_ptr_4.use_count: " << w_ptr_4.use_count() << ", &w_ptr_4: " << &w_ptr_4
                      << std::endl;
            std::cout << "s_ptr_4.use_count: " << s_ptr_4.use_count() << ", &s_ptr_4: " << &s_ptr_4
                      << ", s_ptr_4: " << s_ptr_4 << std::endl;
            std::cout << "s_ptr_5.use_count: " << s_ptr_5.use_count() << ", &s_ptr_5: " << &s_ptr_5
                      << ", s_ptr_5: " << s_ptr_5 << std::endl;

            s_ptr_5 = nullptr;

            if (w_ptr_4.expired())
            {
                std::cout << "weak pointer is expired" << std::endl;
            }

            s_ptr_5 = w_ptr_4.lock();
            std::cout << s_ptr_5 << std::endl;
        }

        {
            std::shared_ptr<int> shared = std::make_shared<int>(42);
            std::weak_ptr<int> weak = shared;

            if (auto sharedCopy = weak.lock())
            {
                // 使用 weak_ptr 获取资源的智能指针，并检查资源是否存在
                std::cout << "Weak pointer value: " << *sharedCopy << std::endl;
            }
            else
            {
                std::cout << "Resource has been released." << std::endl;
            }

            shared.reset(); // 释放资源

            if (auto sharedCopy = weak.lock())
            {
                std::cout << "Weak pointer value: " << *sharedCopy << std::endl;
            }
            else
            {
                std::cout << "Resource has been released." << std::endl;
            }
        }

        class MyClass : public std::enable_shared_from_this<MyClass>
        {
        public:
            std::shared_ptr<MyClass> getShared()
            {
                return shared_from_this(); // 获取指向自身的 shared_ptr
            }
        };

        std::shared_ptr<MyClass> ptr1 = std::make_shared<MyClass>();
        std::cout << "ptr1 use count: " << ptr1.use_count() << std::endl;
        std::cout << "ptr1->getShared() use count: " << ptr1->getShared().use_count() << std::endl;

        std::shared_ptr<MyClass> ptr2 = ptr1->getShared();

        std::cout << "ptr1 use count: " << ptr1.use_count() << std::endl;
        std::cout << "ptr2 use count: " << ptr2.use_count() << std::endl;
    }
    {

        class DanglingPointer
        {
        public:
            static std::shared_ptr<int> sharedPtr()
            {
                return std::shared_ptr<int>(new int{});
            }
        };

        auto ptr_ok = DanglingPointer::sharedPtr();
        qDebug() << "*ptr_ok: " << *ptr_ok << &ptr_ok << ptr_ok.get();

        int *ptr_error = DanglingPointer::sharedPtr().get();
        qDebug() << "*ptr_error: " << *ptr_error << ptr_error;
    }

    struct NestedSharedPointerA {
        NestedSharedPointerA(){
            qDebug() << "A cons";
        }
        ~NestedSharedPointerA(){
            qDebug() << "A dis";
        }

        int a = 0;
        struct NestedSharedPointerB {
            NestedSharedPointerB(){
                qDebug() << "B cons";
            }
            ~NestedSharedPointerB(){
                qDebug() << "B dis";
            }

            int c = 3;
        };
        QSharedPointer<NestedSharedPointerB> b = QSharedPointer<NestedSharedPointerB>(new NestedSharedPointerB);
    };
    QSharedPointer<NestedSharedPointerA> m_a = QSharedPointer<NestedSharedPointerA>(new NestedSharedPointerA);
    qDebug() << "m_a a b" << m_a << m_a->a << m_a->b;

    auto out_b = m_a->b;
    auto out_b2 = m_a->b;
    qDebug() << "b out_b" << m_a->b << out_b;

    m_a.clear();
    qDebug() << "m_a" << m_a;
    qDebug() << "out_b" << out_b << out_b->c;

    out_b.clear();
    qDebug() << "after out_b.clear(), out_b" << out_b;

    auto m_oa = new NestedSharedPointerA;

    delete m_oa;

    return a.exec();
}
