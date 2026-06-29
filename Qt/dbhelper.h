// dbhelper.h
#ifndef DBHELPER_H
#define DBHELPER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QDebug>

class DBHelper
{
public:
    // 获取数据库单例
    static QSqlDatabase getDB();
    // 初始化数据库（创建表）
    static bool initDB();
    // 设备状态操作
    static bool getDeviceStatus(const QString& deviceName, bool& status); // 读取设备状态
    static bool updateDeviceStatus(const QString& deviceName, bool status); // 更新设备状态
    static int getDeviceExtraValue(const QString& deviceName); // 读取设备扩展值（灯光强度/空调温度）
    static bool updateDeviceExtraValue(const QString& deviceName, int value); // 更新设备扩展值
    // 操作日志操作
    static bool addOperationLog(const QString& deviceName, const QString& operation, const QString& result, const QString& operatorName = "admin"); // 新增操作日志
    static bool getOperationLogs(QList<QMap<QString, QVariant>>& logs); // 读取操作日志列表
    static bool getOperationLogs(QList<QMap<QString, QVariant>>& logs, int pageIndex, int pageSize, 
                                 const QString& deviceName = "", const QString& startTime = "", const QString& endTime = ""); // 分页查询
    static int getOperationLogCount(const QString& deviceName = "", const QString& startTime = "", const QString& endTime = ""); // 获取记录数
    // 温湿度历史操作
    static bool addTempHistory(double temp, double humi); // 添加温湿度历史记录
    static bool getTempHistory(QList<QMap<QString, QVariant>>& records); // 读取温湿度历史记录
    static bool getTempHistory(QList<QMap<QString, QVariant>>& records, int pageIndex, int pageSize,
                               const QString& startTime = "", const QString& endTime = ""); // 分页查询
    static int getTempHistoryCount(const QString& startTime = "", const QString& endTime = ""); // 获取记录数
    // 系统配置操作
    static QString getConfig(const QString& key, const QString& defaultValue = ""); // 读取配置
    static bool setConfig(const QString& key, const QString& value); // 保存配置
};

#endif // DBHELPER_H