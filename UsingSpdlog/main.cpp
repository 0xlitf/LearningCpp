#include <QApplication>
#include <QTextEdit>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/qt_sinks.h"

void stdout_example()
{
    // create a color multi-threaded logger
    auto console = spdlog::stdout_color_mt("console");
    auto err_logger = spdlog::stderr_color_mt("stderr");
    spdlog::get("console")->info("loggers can be retrieved from a global registry using the spdlog::get(logger_name)");
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    auto log_widget = new QTextEdit();

    int max_lines = 500; // keep the text widget to max 500 lines. remove old lines if needed.
    auto logger = spdlog::qt_color_logger_mt("qt_logger", log_widget, max_lines);
    logger->info("Some info message");

    log_widget->show();

    stdout_example();

    return app.exec();
}
