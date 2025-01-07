#include <iostream>
#include "TaskManager.h"
#define NOT_FOUND -1
TaskManager::TaskManager() : idCounter(0), numOfEmployees(0){}

int TaskManager::isEmployeeExist(const string& personName){
    int currentEmployee = NOT_FOUND;
    for (int i = 0; i < numOfEmployees; i++){
        if (employees[i].getName() == personName){
            currentEmployee = i;
        }
    }
    return currentEmployee;
}

void TaskManager::assignTask(const string &personName, const Task &task){
    int currentEmployee = isEmployeeExist(personName);
    if(currentEmployee == NOT_FOUND){
        if(numOfEmployees == MAX_PERSONS){
            throw std::runtime_error("TaskManager::assignTask: you have reached the maximum number of employees");
        }
        Person newEmployee(personName);
        currentEmployee = numOfEmployees;
        employees[numOfEmployees++] = newEmployee;
    }
    try{
        Task newTask(task.getPriority(), task.getType(), task.getDescription());
        newTask.setId(idCounter++);
        employees[currentEmployee].assignTask(newTask);
    }
    catch (std::bad_alloc& e){
        throw;
    }
}

void TaskManager::completeTask(const string &personName){
    int currentEmployee = isEmployeeExist(personName);
    if(currentEmployee == NOT_FOUND){
       return;
    }
    try
    {
        employees[currentEmployee].completeTask();
    }
    catch (const std::runtime_error& e)
    {
        throw;
    }
}

const Task TaskManager::setPriority(const Task& tasks, TaskType type, int priority){
    if(tasks.getType() == type){
        Task resultTasks(tasks.getPriority() + priority, tasks.getType(), tasks.getDescription());
        resultTasks.setId(tasks.getId());
        return resultTasks;
    }
    return tasks;
}

void TaskManager::bumpPriorityByType(TaskType type, int priority){
    if(priority <= 0){
        return;
    }
    for(int i = 0; i < numOfEmployees; i++){
        SortedList<Task> oldTasks = this->employees[i].getTasks();
        try{
            SortedList<Task> newTasks = oldTasks.apply([type, priority](Task task){ return setPriority(task, type, priority); });
             this->employees[i].setTasks(newTasks);
        }
        catch (std::bad_alloc& e){
            throw;
        } 
    }

}

void TaskManager::printAllEmployees() const{
    for(int i = 0; i < numOfEmployees; i++){
        std::cout << employees[i] << std::endl;        
    }
}
void TaskManager::printTasks(const SortedList<Task>& tasks){
    for(Task currentTask : tasks){
        std::cout << currentTask << std::endl;
    }
}
void TaskManager::printAllTasks() const{
    SortedList<Task> allTasks = getAllEmployeesTasks();
    printTasks(allTasks);
}

void TaskManager::printTasksByType(TaskType type) const{
    SortedList<Task> allTasks = getAllEmployeesTasks();
    try{
        SortedList<Task> tasksByType = allTasks.filter([type](Task task) { return (task.getType() == type);});
        printTasks(tasksByType);
    }
    catch (std::bad_alloc& e){
        throw;
    }
    
}
SortedList<Task> TaskManager::getAllEmployeesTasks() const{
    SortedList<Task> allTasks;
    for(int i = 0; i < this->numOfEmployees; i++){
        SortedList<Task> tasks(this->employees[i].getTasks());
        for(Task currentTask : tasks){
            allTasks.insert(currentTask);
        }
    }
    return allTasks;
}

