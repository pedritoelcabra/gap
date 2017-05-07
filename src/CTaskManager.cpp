#include "CTaskManager.h"
#include "CGame.h"

extern CGame GAP;

void CTaskManager::AddTask(task_shared_ptr task){
    taskCount++;
    task->SetId(taskCount);
    Tasks.push_back(task);
    return;
}

void CTaskManager::DestroyTask(int id){
    std::vector<task_shared_ptr>::iterator iter = Tasks.begin();
    while (iter != Tasks.end()){
        if((*iter)->GetId() == id){
            Tasks.erase(iter);
            return;
        }
        iter++;
    }
}

void CTaskManager::Update(){
    std::vector<task_shared_ptr>::iterator iter = Tasks.begin();
    while (iter != Tasks.end()){
        if((*iter)->GetCompleted()){
            if(auto s = (*iter)->GetPickUp().lock()){
                s->RemoveOutgoing((*iter)->GetId());
            }
            if(auto s = (*iter)->GetDropOff().lock()){
                s->RemoveOutgoing((*iter)->GetId());
            }
            Tasks.erase(iter);
        }else{
            iter++;
        }
    }
}
