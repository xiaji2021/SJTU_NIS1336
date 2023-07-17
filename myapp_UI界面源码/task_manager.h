// task_manager.h

#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <vector>
#include <string>
#include <unordered_set>

extern std::string taskfilename;

struct Task {
    int taskId;
    std::string task_name;
    std::string start_time;
    int priority;
    std::string category;
    std::string attention_time;
    // 其他任务属性
};

class TaskManager {
public:
    std::vector<Task> tasks;
    int taskIdCounter;
    std::unordered_set<std::string> uniqueTaskIdentifiers; // 存储任务名称+开始时间的组合，用于检查唯一性

    bool isStartTimeUnique(const std::string& start_time);
    void updateTaskIds();

    // 比较函数，用于按开始时间排序
    static bool compareTasksByStartTime(const Task& task1, const Task& task2);


public:
    TaskManager();
    void addTask(const std::string& task_name, const std::string& start_time, int priority, const std::string& category, const std::string& attention_time);
    void deleteTask(int taskId);
    void showTask(const std::string& date = "", const std::string& month = "");
    void readFromFile(const std::string& userfilename);
    void write2File(const std::string& userfilename);
    void attention();
};

#endif // TASK_MANAGER_H
