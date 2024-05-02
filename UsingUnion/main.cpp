
#include <QApplication>
#include <QTextCodec>
#include <QTextEdit>
#include <iostream>
#include <windows.h>

// 内存操作最主要的点：
// 1、内存对齐
// 2、强转时byte长度
// 3、理解union极其内部数据或数据数组
// 4、转换时内存小端序 char 1 2 == uint8_t 49 50, 转为uint16_t为0x5049

; // remove warnings Unterminated '#pragma pack (push, ...)' at end of file
#pragma pack(push, 1)

struct UnionTest {
    uint8_t start{};
    union Data {
        struct Union1 {
            uint8_t u1_1st;
            uint16_t u1_2nd;
        } union1;

        struct Union2 {
            uint16_t u2_1st;
            uint8_t u2_2nd;
        } union2;
    } data;
};

#pragma pack(pop)

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    const char* data = "1234"; // 49 50 51 52

    UnionTest test;

    qDebug() << "std::strlen((const char*) data) " << std::strlen((const char*) data);
    qDebug() << "sizeof data " << sizeof data;
    qDebug() << "sizeof(UnionTest) " << sizeof(UnionTest);

    if (std::strlen((const char*) data) == sizeof(UnionTest)) {
        std::memcpy(&test, data, sizeof(UnionTest));

        std::cout << "std::cout start: " << test.start << ", " << (int) test.start << std::endl; // uint8_t int
        qDebug() << "start: " << test.start << (int) test.start;                                 // uint8_t int
        qDebug() << "u1_1st: " << test.data.union1.u1_1st;                                       //
        qDebug() << "u1_2nd: " << test.data.union1.u1_2nd;                                       // 0x3433

        qDebug() << "start: " << test.start << (int) test.start;
        qDebug() << "u2_1st: " << test.data.union2.u2_1st; // 0x3332
        qDebug() << "u2_2nd: " << test.data.union2.u2_2nd;
    } else {
        qDebug() << "Data size does not match UnionTest size";
    }

    QByteArray byteArray = QByteArray::fromHex("EFBFBDEFBFBDC6B7EFBFBDCDBAEFBFBDEFBFBDDEB7EFBFBDEFBFBDEFBFBDEFBFBDEFBFBDDEFBFDDFBFEFBFBDD0BCEFBFBDEFBFBDEFBFBDEFBFBDEFBFBDEFBFBD");
    qDebug() << "byteArray " << byteArray;
    qDebug() << "fromUtf8 " << QString::fromUtf8(byteArray);
    qDebug() << "fromLocal8Bit " << QString::fromLocal8Bit(byteArray);

    QTextCodec* codec = QTextCodec::codecForName("GBK"); // 或 "GB2312"

    // 将原始数据从GBK转换为UTF-8
    QString decodedString = codec->toUnicode(byteArray);
    qDebug() << "GBK " << decodedString;

    // 原始文本
    wchar_t text[] = L"产品型号无法在数据库中检索到！";

    // 计算所需的字节数（不包括结束的null字符）
    int bytesNeeded = WideCharToMultiByte(CP_ACP, 0, text, -1, NULL, 0, NULL, NULL) - 1;

    // 分配足够的空间
    char* gbkBytes = new char[bytesNeeded];

    // 执行转换
    WideCharToMultiByte(CP_ACP, 0, text, -1, gbkBytes, bytesNeeded, NULL, NULL);

    // 输出字节序列
    qDebug() << "GBK字节序列: ";
    for (int i = 0; i < bytesNeeded; ++i) {
        printf("%02X ", (unsigned char) gbkBytes[i]);
        qDebug() << "gbkBytes" << i << QString("%1").arg((unsigned char) gbkBytes[i], 2, 16, QChar('0'));
    }

    qDebug() << "gbkBytes" << QString::fromLatin1(gbkBytes);
    qDebug() << "gbkBytes" << QString::fromLocal8Bit(gbkBytes);

    // 清理
    delete[] gbkBytes;

    // return 0;

    return app.exec();
}
