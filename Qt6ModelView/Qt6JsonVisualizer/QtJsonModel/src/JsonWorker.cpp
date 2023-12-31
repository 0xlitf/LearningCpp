#include "JsonWorker.h"
#include <string.h>
#include <QElapsedTimer>

bool JsonWorker::IsValid(QString& jsonString)
{
	simdjson::error_code error;
	simdjson::dom::parser parser;
    QElapsedTimer timer;
    timer.start();
	error = parser.parse(simdjson::padded_string(jsonString.toStdString())).error();
    this->ParsingTime = timer.elapsed();
	if (error)
	{
		return false;
	}
	else
	{
		return true;
	}
}
int JsonWorker::getParsingTime()
{
    return this->ParsingTime;
}
void JsonWorker::GenerateTreeView(QString& jsonString, QJsonModel* jsonModel)
{
	jsonModel->loadJson(QByteArray::fromStdString(jsonString.toStdString()));
}

QString JsonWorker::QueryJson(QString& jsonString, QString queryString)
{
    try
    {
        json data = json::parse(jsonString.toStdString());
        std::string query = queryString.toStdString();
        jsoncons::basic_json<char> temp = jsonpath::json_query(data, query);
        std::string result = temp.as_string();
        return QString::fromUtf8(result.c_str());
    }
    catch (...)  {
        return QString::fromStdString("");
    }
}
