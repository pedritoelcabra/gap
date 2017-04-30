#include "CAction.h"

CAction::CAction()
{
    //ctor
}

CAction::CAction( int type_, int arga_, int argb_){
    type = type_;
    arga = arga_;
    argb = argb_;
}

CAction::~CAction()
{
    //dtor
}
