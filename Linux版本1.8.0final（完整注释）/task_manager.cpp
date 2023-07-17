// task_manager.cpp

#include "task_manager.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <sstream> 
#include <iomanip> 
#include <thread>
#include <chrono>

std::string taskfilename;

// 检查是否退出线程的标志
extern int exitflag;
// 初始化类的成员变量taskIdCounter
TaskManager::TaskManager() : taskIdCounter(0) {}

// 重新排序
void TaskManager::updateTaskIds() {
    int newTaskId = 0;
    for (Task& task : tasks) {
        task.taskId = newTaskId++;
    }
    taskIdCounter = newTaskId;
}

// 设置默认的 attention_time 为 start_time 的前十分钟
void TaskManager::setDefaultAttentionTime(Task& task) {

    // 解析本地时间并计算前十分钟的std::tm结构体形式
    std::tm startTime = parseTime(task.start_time);
    std::time_t startTimeUnix = std::mktime(&startTime);
    std::time_t tenMinutesBefore = startTimeUnix - 600;
    std::tm* attentionTime = std::localtime(&tenMinutesBefore);

    // 将格式化后的时间字符串赋值给 task 的 attention_time
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y:%m:%d:%H:%M:%S", attentionTime);
    task.attention_time = buffer;
}

// 解析时间字符串，返回对应的 std::tm 结构体
std::tm TaskManager::parseTime(const std::string& timeStr) {

    //将时间字符串解析到std::tm结构体中
    std::tm time = {};
    std::stringstream ss(timeStr);
    ss >> std::get_time(&time, "%Y:%m:%d:%H:%M:%S");

    // 检查解析是否失败
    if (ss.fail()) {
        throw std::runtime_error("时间解析失败");
    }
    return time;
}

// 更新 attention_time
void TaskManager::updateAttentionTime(Task& task) {

    // 将当前时间和提醒时间解析为atd::tm结构体
    std::time_t currentTimeUnix = std::time(nullptr);
    std::tm* currentTime = std::localtime(&currentTimeUnix);
    std::tm taskAttentionTime = parseTime(task.attention_time);

    // 如果 attention_time 比当前时间早，则将当前时间赋值给 attention_time
    if (std::mktime(&taskAttentionTime) < std::mktime(currentTime)) {
        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%Y:%m:%d:%H:%M:%S", currentTime);
        task.attention_time = buffer;

    }
}

// 检查时间并进行提醒
void TaskManager::checkAttentionTime() {
    // 获取当前时间点
    std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();

    // 迭代器用于遍历任务列表
    auto it = tasks.begin(); 
    while (it != tasks.end()) {
        Task& task = *it;
        
        //检查退出线程的标志
        if (exitflag == 1) {
            exitflag = 0;
            pthread_exit(nullptr);
        } 

        // 将开始时间转换为时间戳
        std::tm taskStartTime = parseTime(task.start_time);
        std::chrono::system_clock::time_point taskStartTimePoint = std::chrono::system_clock::from_time_t(std::mktime(&taskStartTime));
        
        // 计算当前时间和任务开始时间的时间差
        std::chrono::duration<double> timeDiff = taskStartTimePoint - currentTime;
        int minutesDiff = static_cast<int>(std::chrono::duration_cast<std::chrono::minutes>(timeDiff).count());
 
        // 如果任务开始时间已经早于当前时间，不再提醒
        if (taskStartTimePoint < currentTime) {
            it = tasks.erase(it);
            uniqueTaskIdentifiers.erase(task.task_name + task.start_time);
            continue; 
        }

        // 如果开始时间在当前时间的十分钟以内，并且还未打印过提醒，打印并设置标志为已打印
        if (minutesDiff >= 0 && minutesDiff <= 10 && !task.printedReminder) {
            std::cout << "Attention: Task \"" << task.task_name << "\" requires attention at " << task.start_time << std::endl;
            task.printedReminder = true; 
        }

         // 如果 attention_time 和当前时间匹配，并且还未打印过提醒，打印并设置标志为已打印
        if (!task.printedReminder) {
            std::tm taskAttentionTime = parseTime(task.attention_time);
            std::chrono::system_clock::time_point taskAttentionTimePoint = std::chrono::system_clock::from_time_t(std::mktime(&taskAttentionTime));
            if (taskAttentionTimePoint <= currentTime) {
                std::cout << "Attention: Task \"" << task.task_name << "\" requires attention at " << task.attention_time << std::endl;
                task.printedReminder = true; 
            }
        }

        ++it;
    }

    if (exitflag == 1) {
        exitflag = 0;
        pthread_exit(nullptr);
    }
}

// 增加任务功能
void TaskManager::addTask(const std::string& task_name, const std::string& start_time, int priority, const std::string& category, const std::string& attention_time) {
	readFromFile(taskfilename);

    // 检查开始时间格式
    std::tm start_tm = {};
    if (strptime(start_time.c_str(), "%Y:%m:%d:%H:%M:%S", &start_tm) == nullptr) {
        std::cerr << "Error: Invalid start time. Please provide time in the format 'YYYY:MM:DD:HH:MM:SS' and provide a valid date and time.\n";
        return;
    }

    // 检查开始时间是否早于当前时间
    auto currentTime = std::chrono::system_clock::now();
    std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
    std::tm* currentTM = std::localtime(&currentTimeT);
    if (std::mktime(&start_tm) < std::mktime(currentTM)) {
        std::cerr << "Error: Start time cannot be earlier than the current time.\n";
        return;
    }

    // 检查 attention_time 格式
    std::tm attention_tm = {};
    if (attention_time != " ") {
        if (strptime(attention_time.c_str(), "%Y:%m:%d:%H:%M:%S", &attention_tm) == nullptr) {
        std::cerr << "Error: Invalid attention time. Please provide time in the format 'YYYY:MM:DD:HH:MM:SS' and provide a valid date and time.\n";
        return;
    }
    }
    
    // 检查提醒时间是否晚于开始时间
    if (attention_time != " " && std::mktime(&attention_tm) > std::mktime(&start_tm)) {
        std::cerr << "Error: Attention time cannot be later than the start time.\n";
        return;
    }

    // 检查任务分类是否有效
    std::string categoryLowerCase = category;
    std::transform(categoryLowerCase.begin(), categoryLowerCase.end(), categoryLowerCase.begin(), ::tolower);
    std::vector<std::string> validCategories = {"personal", "work", "study", "home", "social", "others"};
    if (std::find(validCategories.begin(), validCategories.end(), categoryLowerCase) == validCategories.end()) {
        std::cerr << "Error: Invalid task category. Please choose from the following categories: Personal, Work, Study, Home, Social or let it empty.\n";
        return;
    }

    // 构造任务对象
    Task newTask;
    newTask.taskId = taskIdCounter++;
    newTask.task_name = task_name;
    newTask.start_time = start_time;
    newTask.priority = priority;
    newTask.category = category;
    newTask.attention_time = attention_time;
    newTask.printedReminder = false; // 标识是否已经打印过提醒

    // 检查任务开始时间的唯一性
    for (const Task& task : tasks) {
        if (task.start_time == start_time) {
            std::cerr << "Error: Task with the same start time already exists. Please provide a unique start time.\n";
            return;
        }
    }

    // 检查任务名称+开始时间是否唯一
    std::string taskIdentifier = newTask.task_name + newTask.start_time;
    if (uniqueTaskIdentifiers.count(taskIdentifier) > 0) {
        std::cerr << "Error: Task with the same name and start time already exists. Please provide a unique combination of name and start time.\n";
        return;
    }

    // 设置默认的 attention_time 为 start_time 的前十分钟
    if (attention_time == " ") {
        setDefaultAttentionTime(newTask);
    } else {
            newTask.attention_time = attention_time;
    }

    // 比较 attention_time 和当前时间，如果 attention_time 比当前时间早，则将当前时间赋值给 attention_time
    updateAttentionTime(newTask);

    // 添加任务到任务列表和唯一性集合中
    tasks.push_back(newTask);
    uniqueTaskIdentifiers.insert(taskIdentifier);

    std::cout  << "Task added successfully.\n";

	write2File(taskfilename);
}

//删除任务功能
void TaskManager::deleteTask(int taskId) {
	readFromFile(taskfilename);

    // 遍历任务列表查找匹配的任务
    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        if (it->taskId == taskId) {
            tasks.erase(it);
            updateTaskIds();

            // 删除任务后，还需要从唯一性集合中删除对应的任务名称+开始时间
            std::string taskIdentifier = it->task_name + it->start_time;
            uniqueTaskIdentifiers.erase(taskIdentifier);
		    write2File(taskfilename);
            return;
        }
    }
    std::cerr << "Error: Task with ID " << taskId << " not found.\n";
}

// 比较任务的开始时间
bool TaskManager::compareTasksByStartTime(const Task& task1, const Task& task2) {
    return task1.start_time < task2.start_time;
}

// 显示任务
void TaskManager::showTask(const std::string& date, const std::string& month, const std::string& category) {
	readFromFile(taskfilename);

    // 创建一个存储筛选后的任务的容器
    std::vector<Task> filteredTasks;

    // 根据传入的参数来筛选任务
    if (!date.empty()) {
        // 验证日期格式
        std::tm date_tm = {};
        if (strptime(date.c_str(), "%Y:%m:%d", &date_tm) == nullptr) {
            std::cerr << "Error: Invalid date format. Please provide the date in the format 'YYYY:MM:DD'.\n";
            return;
        }

        // 显示指定日期的任务
        for (const Task& task : tasks) {
            if (task.start_time.find(date) == 0) {
                filteredTasks.push_back(task);
            }
        }
    } else if (!month.empty()) {
        // 验证月份格式
        std::tm month_tm = {};
        if (strptime(month.c_str(), "%Y:%m", &month_tm) == nullptr) {
            std::cerr << "Error: Invalid month format. Please provide the month in the format 'YYYY:MM'.\n";
            return;
        }

        // 显示指定月份的任务
        for (const Task& task : tasks) {
            if (task.start_time.find(month) == 0) {
                filteredTasks.push_back(task);
            }
        }
    } else if (!category.empty()) {
        // 验证分类
        std::vector<std::string> validCategories = { "personal", "work", "study", "home", "social", "others" };
            if (std::find(validCategories.begin(), validCategories.end(), category) == validCategories.end()) {
            std::cerr << "Error: Invalid category. Please provide one of the following categories: 'personal', 'work', 'study', 'home', 'social', 'others'.\n";
                    return;
                }

        // 显示指定分类的任务
        for (const Task& task : tasks) {
            if (task.category == category) {
                filteredTasks.push_back(task);
            }
        }
    } else {
         // 显示所有任务
         filteredTasks = tasks;
    }

    // 按开始时间排序
    std::sort(filteredTasks.begin(), filteredTasks.end(), compareTasksByStartTime);

    // 显示任务
    for (const Task& task : filteredTasks) {
    std::cout << "Task ID: " << task.taskId << "\n";
    std::cout << "Name: " << task.task_name << "\n";
    std::cout << "Start Time: " << task.start_time << "\n";

       // 判断优先级是否为默认值
       if (task.priority != 0) {
          std::cout << "Priority: " << task.priority << "\n";
        }
    std::cout << "Category: " << task.category << "\n";
    std::cout << "Attention time: " << task.attention_time << "\n";
    std::cout << "---------------------------\n";
    }     
}

// 从文件中读取信息
void TaskManager::readFromFile(const std::string& userfilename){
	tasks.clear();
	std::fstream fin(userfilename, std::ios::in);
	fin>>taskIdCounter;
	for (int i=0;i<taskIdCounter;i++){
		Task task_tmp;
		fin>>task_tmp.taskId>>task_tmp.task_name>>task_tmp.start_time>>task_tmp.priority>>task_tmp.category>>task_tmp.attention_time>>task_tmp.printedReminder;
		tasks.push_back(task_tmp);
	}
	fin.close();
}

// 将任务列表写入文件
void TaskManager::write2File(const std::string& userfilename){
	std::ofstream fout(userfilename);
	fout<<taskIdCounter<<std::endl;
	for (int i=0;i<tasks.size();i++){
		fout<<tasks[i].taskId<<" "<<tasks[i].task_name<<" "<<tasks[i].start_time<<" "<<tasks[i].priority<<" "<<tasks[i].category<<" "<<tasks[i].attention_time<<" "<<tasks[i].printedReminder<<std::endl;
	}
	fout.close();
}



