// task_manager.cpp

#include "task_manager.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <QDateTime>
#include <stdio.h>
#include <QMessageBox>
#include <QThread>


std::string taskfilename;

TaskManager::TaskManager() : taskIdCounter(0) {}

bool TaskManager::isStartTimeUnique(const std::string& startTime) {
    for (const Task& task : tasks) {
        if (task.start_time == startTime) {
            return false;
        }
    }
    return true;
}

void TaskManager::updateTaskIds() {
    int newTaskId = 0;
    for (Task& task : tasks) {
        task.taskId = newTaskId++;
    }
    taskIdCounter = newTaskId;
}

void TaskManager::addTask(const std::string& task_name, const std::string& start_time, int priority, const std::string& category, const std::string& attention_time) {
	readFromFile(taskfilename);
    // 检查开始时间是否唯一
    if (!isStartTimeUnique(start_time)) {
   //     std::cerr << "Error: Start time already used by another task. Please provide a unique start time.\n";
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

    // 检查任务名称+开始时间是否唯一
    std::string taskIdentifier = newTask.task_name + newTask.start_time;
    if (uniqueTaskIdentifiers.count(taskIdentifier) > 0) {
     //   std::cerr << "Error: Task with the same name and start time already exists. Please provide a unique combination of name and start time.\n";
        return;
    }

    // 添加任务到任务列表和唯一性集合中
    tasks.push_back(newTask);
    uniqueTaskIdentifiers.insert(taskIdentifier);

  //  std::cout << "Task added successfully.\n";
    
	write2File(taskfilename);
}


void TaskManager::deleteTask(int taskId) {
	readFromFile(taskfilename);
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
  //  std::cerr << "Error: Task with ID " << taskId << " not found.\n";
}

bool TaskManager::compareTasksByStartTime(const Task& task1, const Task& task2) {
    return task1.start_time < task2.start_time;
}

void TaskManager::showTask(const std::string& date, const std::string& month) {
	readFromFile(taskfilename);

    std::vector<Task> filteredTasks;

    // 根据传入的参数来筛选任务
    if (!date.empty()) {
        // 显示指定日期的任务
        for (const Task& task : tasks) {
            if (task.start_time.find(date) == 0) {
                filteredTasks.push_back(task);
            }
        }
    } else if (!month.empty()) {
        // 显示指定月份的任务
        for (const Task& task : tasks) {
            if (task.start_time.find(month) == 0) {
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

    // 判断分类是否为默认值
    if (!task.category.empty()) {
        std::cout << "Category: " << task.category << "\n";
    }

    // 判断提醒时间是否为默认值
    if (task.attention_time != "00:00:00:00:00:00") {
        std::cout << "Attention time: " << task.attention_time << "\n";
    }

    std::cout << "---------------------------\n";
} 
}

void TaskManager::readFromFile(const std::string& userfilename){

	tasks.clear();
	std::fstream fin(userfilename, std::ios::in);
	
	fin>>taskIdCounter;
	for (int i=0;i<taskIdCounter;i++){
		Task task_tmp;
		fin>>task_tmp.taskId>>task_tmp.task_name>>task_tmp.start_time>>task_tmp.priority>>task_tmp.category>>task_tmp.attention_time;
		tasks.push_back(task_tmp);
	}
    fin.close();
}

void TaskManager::write2File(const std::string& userfilename){

	std::ofstream fout(userfilename);
	fout<<taskIdCounter<<std::endl;
	for (int i=0;i<tasks.size();i++){
		fout<<tasks[i].taskId<<" "<<tasks[i].task_name<<" "<<tasks[i].start_time<<" "<<tasks[i].priority<<" "<<tasks[i].category<<" "<<tasks[i].attention_time<<std::endl;
	}
    fout.close();
}





