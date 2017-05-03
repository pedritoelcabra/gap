#ifndef CACTION_H
#define CACTION_H

class CAction
{

    public:
        enum EActions { idleAction , moveTile, dropGoods, pickUpGoods, gatherResource, buildBuilding };
        enum EAssignments { idleAssignment , followUnit, gatherResources, builderAssignment };

        CAction();
		CAction(int type, int arga, int argb);
        virtual ~CAction();

		int 		type;
		int 		arga;
		int 		argb;
    protected:

    private:
};

#endif // CACTION_H

