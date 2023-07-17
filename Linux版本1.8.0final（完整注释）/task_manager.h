// task_manager.h

#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <vector>
#include <string>
#include <unordered_set>
#include <ctime>

extern std::string taskfilename;

struct Task {
    int taskId;
    std::string task_name;
    std::string start_time;
    int priority;
    std::string category;
    std::string attention_time;
    bool printedReminder; // 标识是否已经打印过提醒
};

class TaskManager {
private:
    std::vector<Task> tasks;
    int taskIdCounter;
    std::unordered_set<std::string> uniqueTaskIdentifiers; // 存储任务名称+开始时间的组合，用于检查唯一性

    void updateTaskIds();

    // 比较函数，用于按开始时间排序
    static bool compareTasksByStartTime(const Task& task1, const Task& task2);
    // 设置默认的注意时间
    void setDefaultAttentionTime(Task& task);
    // 更新注意时间
    void updateAttentionTime(Task& task);
    // 将时间解析为std::tm结构体
    std::tm parseTime(const std::string& timeStr); 
public:
    TaskManager();
    void addTask(const std::string& task_name, const std::string& start_time, int priority, const std::string& category, const std::string& attention_time);
    void deleteTask(int taskId);
    void showTask(const std::string& date = "", const std::string& month = "", const std::string& category = "");
    void readFromFile(const std::string& userfilename);
    void write2File(const std::string& userfilename);
    void checkAttentionTime();
};

#endif // TASK_MANAGER_H
