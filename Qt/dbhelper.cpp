// dbhelper.cpp
#include "dbhelper.h"
#include <QList>
#include <QMap>

QSqlDatabase DBHelper::getDB()
{
    QSqlDatabase db;
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        // 使用SQLite数据库
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("smart_home.db"); // 数据库文件路径
    }
    return db;
}

bool DBHelper::initDB()
{
    QSqlDatabase db = getDB();
    if (!db.open()) {
        qDebug() << "数据库打开失败：" << db.lastError().text();
        return false;
    }

    // 创建设备状态表
    QSqlQuery query;
    QString createDeviceTable = R"(
        CREATE TABLE IF NOT EXISTS device_status (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            device_name VARCHAR(50) UNIQUE NOT NULL,
            status BOOLEAN NOT NULL DEFAULT 0,
            extra_value INTEGER DEFAULT 0,
            update_time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
        )
    )";
    if (!query.exec(createDeviceTable)) {
        qDebug() << "创建设备状态表失败：" << query.lastError().text();
        return false;
    }

    // 如果 extra_value 列不存在，则添加（兼容旧数据库）
    query.exec("PRAGMA table_info(device_status)");
    bool hasExtraValue = false;
    while (query.next()) {
        if (query.value(1).toString() == "extra_value") {
            hasExtraValue = true;
            break;
        }
    }
    if (!hasExtraValue) {
        query.exec("ALTER TABLE device_status ADD COLUMN extra_value INTEGER DEFAULT 0");
    }

    // 创建操作日志表
    QString createLogTable = R"(
        CREATE TABLE IF NOT EXISTS operation_log (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            device_name VARCHAR(50) NOT NULL,
            operation VARCHAR(50) NOT NULL,
            result VARCHAR(20) NOT NULL,
            operator_name VARCHAR(20) NOT NULL,
            create_time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
        )
    )";
    if (!query.exec(createLogTable)) {
        qDebug() << "创建操作日志表失败：" << query.lastError().text();
        return false;
    }

    // 创建系统配置表
    QString createConfigTable = R"(
        CREATE TABLE IF NOT EXISTS system_config (
            key VARCHAR(50) PRIMARY KEY,
            value VARCHAR(100) NOT NULL
        )
    )";
    if (!query.exec(createConfigTable)) {
        qDebug() << "创建系统配置表失败：" << query.lastError().text();
        return false;
    }

    // 创建温湿度历史表
    QString createTempHistoryTable = R"(
        CREATE TABLE IF NOT EXISTS temp_humidity_history (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            temperature REAL NOT NULL,
            humidity REAL NOT NULL,
            create_time DATETIME NOT NULL
        )
    )";
    if (!query.exec(createTempHistoryTable)) {
        qDebug() << "创建温湿度历史表失败：" << query.lastError().text();
        return false;
    }

    // 初始化默认设备状态（如果不存在）
    QStringList devices = {"light", "air", "curtain", "door"};
    for (const QString& dev : devices) {
        query.prepare("INSERT OR IGNORE INTO device_status (device_name, status) VALUES (?, 0)");
        query.addBindValue(dev);
        if (!query.exec()) {
            qDebug() << "初始化设备状态失败：" << query.lastError().text();
        }
    }

    db.close();
    return true;
}

bool DBHelper::getDeviceStatus(const QString& deviceName, bool& status)
{
    QSqlDatabase db = getDB();
    if (!db.open()) {
        qDebug() << "数据库打开失败：" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT status FROM device_status WHERE device_name = ?");
    query.addBindValue(deviceName);
    if (!query.exec()) {
        qDebug() << "读取设备状态失败：" << query.lastError().text();
        db.close();
        return false;
    }

    if (!query.next()) {
        status = false;
        db.close();
        return true;
    }

    status = query.value(0).toBool();
    db.close();
    return true;
}

bool DBHelper::updateDeviceStatus(const QString& deviceName, bool status)
{
    QSqlDatabase db = getDB();
    if (!db.open()) {
        qDebug() << "数据库打开失败：" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.prepare(R"(
        INSERT OR REPLACE INTO device_status (device_name, status, update_time)
        VALUES (?, ?, ?)
    )");
    query.addBindValue(deviceName);
    query.addBindValue(status);
    query.addBindValue(currentTime);
    if (!query.exec()) {
        qDebug() << "更新设备状态失败：" << query.lastError().text();
        db.close();
        return false;
    }

    db.close();
    return true;
}

int DBHelper::getDeviceExtraValue(const QString& deviceName)
{
    QSqlDatabase db = getDB();
    if (!db.open()) {
        qDebug() << "数据库打开失败：" << db.lastError().text();
        return 0;
    }

    QSqlQuery query;
    query.prepare("SELECT extra_value FROM device_status WHERE device_name = ?");
    query.addBindValue(deviceName);
    if (!query.exec()) {
        qDebug() << "读取设备扩展值失败：" << query.lastError().text();
        db.close();
        return 0;
    }

    if (!query.next()) {
        db.close();
        return 0;
    }

    int value = query.value(0).toInt();
    db.close();
    return value;
}

bool DBHelper::updateDeviceExtraValue(const QString& deviceName, int value)
{
    QSqlDatabase db = getDB();
    if (!db.open()) {
        qDebug() << "数据库打开失败：" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.prepare(R"(
        INSERT OR REPLACE INTO device_status (device_name, status, extra_value, update_time)
        VALUES (?, (SELECT status FROM device_status WHERE device_name = ?), ?, ?)
    )");
    query.addBindValue(deviceName);
    query.addBindValue(deviceName);
    query.addBindValue(value);
    query.addBindValue(currentTime);
    if (!query.exec()) {
        qDebug() << "更新设备扩展值失败：" << query.lastError().text();
        db.close();
        return false;
    }

    db.close();
    return true;
}

bool DBHelper::addOperationLog(const QString& deviceName, const QString& operation, const QString& result, const QString& operatorName)
{
    QSqlDatabase db = getDB();
    if (!db.open()) {
        qDebug() << "数据库打开失败：" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.prepare(R"(
        INSERT INTO operation_log (device_name, operation, result, operator_name, create_time)
        VALUES (?, ?, ?, ?, ?)
    )");
    query.addBindValue(deviceName);
    query.addBindValue(operation);
    query.addBindValue(result);
    query.addBindValue(operatorName);
    query.addBindValue(currentTime);

    if (!query.exec()) {
        qDebug() << "添加操作日志失败：" << query.lastError().text();
        db.close();
        return false;
    }

    db.close();
    return true;
}

bool DBHelper::getOperationLogs(QList<QMap<QString, QVariant>>& logs)
{
    QSqlDatabase db = getDB();
    if (!db.open()) {
        qDebug() << "数据库打开失败：" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT id, device_name, operation, result, operator_name, create_time FROM operation_log ORDER BY create_time DESC");
    if (!query.exec()) {
        qDebug() << "读取操作日志失败：" << query.lastError().text();
        db.close();
        return false;
    }

    logs.clear();
    while (query.next()) {
        QMap<QString, QVariant> log;
        log["id"] = query.value(0).toInt();
        log["device_name"] = query.value(1).toString();
        log["operation"] = query.value(2).toString();
        log["result"] = query.value(3).toString();
        log["operator_name"] = query.value(4).toString();
        log["create_time"] = query.value(5).toString();
        logs.append(log);
    }

    db.close();
    return true;
}

bool DBHelper::getOperationLogs(QList<QMap<QString, QVariant>>& logs, int pageIndex, int pageSize,
                                 const QString& deviceName, const QString& startTime, const QString& endTime)
{
    QSqlDatabase db = getDB();
    if (!db.open()) {
        qDebug() << "数据库打开失败：" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    QString sql = "SELECT id, device_name, operation, result, operator_name, create_time FROM operation_log WHERE 1=1";
    QList<QVariant> params;

    if (!deviceName.isEmpty()) {
        sql += " AND device_name LIKE ?";
        params.append("%" + deviceName + "%");
    }
    if (!startTime.isEmpty()) {
        sql += " AND create_time >= ?";
        params.append(startTime);
    }
    if (!endTime.isEmpty()) {
        sql += " AND create_time <= ?";
        params.append(endTime + " 23:59:59");
    }

    sql += " ORDER BY create_time DESC LIMIT ? OFFSET ?";
    params.append(pageSize);
    params.append(pageIndex * pageSize);

    query.prepare(sql);
    for (int i = 0; i < params.size(); i++) {
        query.addBindValue(params[i]);
    }

    if (!query.exec()) {
        qDebug() << "读取操作日志失败：" << query.lastError().text();
        db.close();
        return false;
    }

    logs.clear();
    while (query.next()) {
        QMap<QString, QVariant> log;
        log["id"] = query.value(0).toInt();
        log["device_name"] = query.value(1).toString();
        log["operation"] = query.value(2).toString();
        log["result"] = query.value(3).toString();
        log["operator_name"] = query.value(4).toString();
        log["create_time"] = query.value(5).toString();
        logs.append(log);
    }

    db.close();
    return true;
}

int DBHelper::getOperationLogCount(const QString& deviceName, const QString& startTime, const QString& endTime)
{
    QSqlDatabase db = getDB();
    if (!db.open()) {
        qDebug() << "数据库打开失败：" << db.lastError().text();
        return 0;
    }

    QSqlQuery query;
    QString sql = "SELECT COUNT(*) FROM operation_log WHERE 1=1";
    QList<QVariant> params;

    if (!deviceName.isEmpty()) {
        sql += " AND device_name LIKE ?";
        params.append("%" + deviceName + "%");
    }
    if (!startTime.isEmpty()) {
        sql += " AND create_time >= ?";
        params.append(startTime);
    }
    if (!endTime.isEmpty()) {
        sql += " AND create_time <= ?";
        params.append(endTime + " 23:59:59");
    }

    query.prepare(sql);
    for (int i = 0; i < params.size(); i++) {
        query.addBindValue(params[i]);
    }

    if (!query.exec() || !query.next()) {
        qDebug() << "统计操作日志数量失败：" << query.lastError().text();
        db.close();
        return 0;
    }

    int count = query.value(0).toInt();
    db.close();
    return count;
}

bool DBHelper::addTempHistory(double temp, double humi)
{
    QSqlDatabase db = getDB();
    if (!db.open()) {
        qDebug() << "数据库打开失败：" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.prepare(R"(
        INSERT INTO temp_humidity_history (temperature, humidity, create_time)
        VALUES (?, ?, ?)
    )");
    query.addBindValue(temp);
    query.addBindValue(humi);
    query.addBindValue(currentTime);

    if (!query.exec()) {
        qDebug() << "添加温湿度历史失败：" << query.lastError().text();
        db.close();
        return false;
    }

    db.close();
    return true;
}

bool DBHelper::getTempHistory(QList<QMap<QString, QVariant>>& records)
{
    QSqlDatabase db = getDB();
    if (!db.open()) {
        qDebug() << "数据库打开失败：" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT id, temperature, humidity, create_time FROM temp_humidity_history ORDER BY create_time DESC");
    if (!query.exec()) {
        qDebug() << "读取温湿度历史失败：" << query.lastError().text();
        db.close();
        return false;
    }

    records.clear();
    while (query.next()) {
        QMap<QString, QVariant> record;
        record["id"] = query.value(0).toInt();
        record["temperature"] = query.value(1).toDouble();
        record["humidity"] = query.value(2).toDouble();
        record["create_time"] = query.value(3).toString();
        records.append(record);
    }

    db.close();
    return true;
}

bool DBHelper::getTempHistory(QList<QMap<QString, QVariant>>& records, int pageIndex, int pageSize,
                               const QString& startTime, const QString& endTime)
{
    QSqlDatabase db = getDB();
    if (!db.open()) {
        qDebug() << "数据库打开失败：" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    QString sql = "SELECT id, temperature, humidity, create_time FROM temp_humidity_history WHERE 1=1";
    QList<QVariant> params;

    if (!startTime.isEmpty()) {
        sql += " AND create_time >= ?";
        params.append(startTime);
    }
    if (!endTime.isEmpty()) {
        sql += " AND create_time <= ?";
        params.append(endTime + " 23:59:59");
    }

    sql += " ORDER BY create_time DESC LIMIT ? OFFSET ?";
    params.append(pageSize);
    params.append(pageIndex * pageSize);

    query.prepare(sql);
    for (int i = 0; i < params.size(); i++) {
        query.addBindValue(params[i]);
    }

    if (!query.exec()) {
        qDebug() << "读取温湿度历史失败：" << query.lastError().text();
        db.close();
        return false;
    }

    records.clear();
    while (query.next()) {
        QMap<QString, QVariant> record;
        record["id"] = query.value(0).toInt();
        record["temperature"] = query.value(1).toDouble();
        record["humidity"] = query.value(2).toDouble();
        record["create_time"] = query.value(3).toString();
        records.append(record);
    }

    db.close();
    return true;
}

int DBHelper::getTempHistoryCount(const QString& startTime, const QString& endTime)
{
    QSqlDatabase db = getDB();
    if (!db.open()) {
        qDebug() << "数据库打开失败：" << db.lastError().text();
        return 0;
    }

    QSqlQuery query;
    QString sql = "SELECT COUNT(*) FROM temp_humidity_history WHERE 1=1";
    QList<QVariant> params;

    if (!startTime.isEmpty()) {
        sql += " AND create_time >= ?";
        params.append(startTime);
    }
    if (!endTime.isEmpty()) {
        sql += " AND create_time <= ?";
        params.append(endTime + " 23:59:59");
    }

    query.prepare(sql);
    for (int i = 0; i < params.size(); i++) {
        query.addBindValue(params[i]);
    }

    if (!query.exec() || !query.next()) {
        qDebug() << "统计温湿度历史数量失败：" << query.lastError().text();
        db.close();
        return 0;
    }

    int count = query.value(0).toInt();
    db.close();
    return count;
}

QString DBHelper::getConfig(const QString& key, const QString& defaultValue)
{
    QSqlDatabase db = getDB();
    if (!db.open()) {
        qDebug() << "数据库打开失败：" << db.lastError().text();
        return defaultValue;
    }

    QSqlQuery query;
    query.prepare("SELECT value FROM system_config WHERE key = ?");
    query.addBindValue(key);
    if (!query.exec()) {
        qDebug() << "读取配置失败：" << query.lastError().text();
        db.close();
        return defaultValue;
    }

    QString value = defaultValue;
    if (query.next()) {
        value = query.value(0).toString();
    }

    db.close();
    return value;
}

bool DBHelper::setConfig(const QString& key, const QString& value)
{
    QSqlDatabase db = getDB();
    if (!db.open()) {
        qDebug() << "数据库打开失败：" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.prepare(R"(
        INSERT OR REPLACE INTO system_config (key, value)
        VALUES (?, ?)
    )");
    query.addBindValue(key);
    query.addBindValue(value);

    if (!query.exec()) {
        qDebug() << "保存配置失败：" << query.lastError().text();
        db.close();
        return false;
    }

    db.close();
    return true;
}