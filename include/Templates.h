
template <class Container>
void DeleteUnusedWeak(const Container& cont) {
    auto iter = cont->begin();
    while (iter != cont->end()){
        if((*iter).lock()){
            iter++;
        }else{
            cont->erase(iter);
        }
    }
}

template <class Container>
void DeleteById(const Container& cont, int id) {
    auto iter = cont->begin();
    while (iter != cont->end()){
        if(auto s = (*iter).lock()){
            if(s->GetId() == id){
                cont->erase(iter);
                return;
            }
        }
        iter++;    
    }
}
