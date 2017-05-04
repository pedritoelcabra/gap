#ifndef CTaskManager_H
#define CTaskManager_H

#include <memory>
#include <vector>
#include <algorithm>
#include "CTransportTask.h"

typedef std::shared_ptr<CTransportTask> task_shared_ptr;
typedef std::weak_ptr<CTransportTask> task_weak_ptr;

class CTaskManager
{
    public:
        CTaskManager(){};
        virtual ~CTaskManager(){};

        void                                AddTask(task_shared_ptr ptr);
        void                                DestroyTask(int id);

    protected:
    private:
        std::vector<unit_shared_ptr>        Tasks;
        int                                 taskCount = 0;
};

#endif // CTaskManager_H
