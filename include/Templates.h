
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

