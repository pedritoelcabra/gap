#include "CPathfinder.h"
#include "CLog.h"

CPathfinder::CPathfinder(){
    direction = {
        { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 },
        { -1, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }
	};
	directions = 8;
}

CPathfinder::~CPathfinder(){
    //dtor
}

Coord::Coord(int x_, int y_){
    x = x_;
    y = y_;
}

bool Coord::operator == (const Coord& coord_) const{
    return (x == coord_.x && y == coord_.y);
}

Coord operator + (const Coord& left_, const Coord& right_){
    return{ left_.x + right_.x, left_.y + right_.y };
}

Node::Node(Coord coords_, Node *parent_){
    parent = parent_;
    coords = coords_;
    G = H = 0;
}

float Node::getScore(){
    return G + H;
}

void CPathfinder::Init(){

}

void CPathfinder::SetCost(int x, int y, float cost){
    CollisionMap[Coord(x, y)] = cost;
}

float CPathfinder::GetCost(int x, int y){
	CoordMap::iterator it = CollisionMap.find(Coord(x, y));
	if ( it == CollisionMap.end() ) {
	  	return 10.0f;
	}
    return it->second;
}

float CPathfinder::getMoveCost(Coord coords_){
	CoordMap::iterator it = CollisionMap.find(coords_);
	if ( it == CollisionMap.end() ) {
	  	return 0;
	}
	if(it->second < minv || it->second >= maxv ){
		return 0;
	}
	return 1.0f - (it->second - static_cast<int>(it->second));
}

CoordList CPathfinder::FindPath(Coord source_, Coord target_, float minv_, float maxv_, bool diagonal){

    Coord distC = getDelta(source_, target_);
    unsigned int distance = distC.x + distC.y;
    if(distance < 10){
        distance = 10;
    }
	minv = minv_;
	maxv = maxv_;
    CoordList path;

    if(source_ == target_){
        return path;
    }
    if(getMoveCost(target_) == 0.0f){
        return path;
    }
    Node *current = nullptr;
    NodeSet openSet, closedSet;
    openSet.insert(new Node(source_));
	float currentCost = 0.0f, totalCost = 0.0f;

    	while (!openSet.empty()) {
        	current = *openSet.begin();
        	for (auto node : openSet) {
                if (node->getScore() <= current->getScore()) {
                    current = node;
                }
        	}

        	if (current->coords == target_) {
                break;
        	}

            if(closedSet.size() > distance * 10){
                CLog::Write("Aborting path... too long");
                CLog::Write(static_cast<int>(distance));
                return path;
            }

        	closedSet.insert(current);
        	openSet.erase(std::find(openSet.begin(), openSet.end(), current));

            for (unsigned int i = 0; i < (diagonal ? 8 : 4); ++i) {
                Coord newCoords = Coord(current->coords + direction[i]);
                currentCost = getMoveCost(newCoords);
                if ( currentCost == 0.0f ||
                    findNodeOnList(closedSet, newCoords)) {
                    continue;
                }

                currentCost = ((i < 4) ? 1.0f : 1.4f ) / currentCost;
                totalCost = current->G + currentCost;

                Node *successor = findNodeOnList(openSet, newCoords);
                if (successor == nullptr) {
                    successor = new Node(newCoords, current);
                    successor->G = totalCost;
                    successor->H = Heuristic(successor->coords, target_);
                    successor->H = successor->H + static_cast<float>(i)/100;
                    openSet.insert(successor);
                }
                else if (totalCost < successor->G) {
                    successor->parent = current;
                    successor->G = totalCost;
                }
        	}
    	}

    	while (current != nullptr) {
        	path.push_back(current->coords);
        	current = current->parent;
    	}
    	path.pop_back();
        std::reverse(path.begin(), path.end());

    	releaseNodes(openSet);
    	releaseNodes(closedSet);

	return path;
}

Node* CPathfinder::findNodeOnList(NodeSet& nodes_, Coord coords_){
    for (auto node : nodes_) {
        if (node->coords == coords_) {
            return node;
        }
    }
    return nullptr;
}

Coord CPathfinder::getDelta(Coord source_, Coord target_){
    	return{ abs(source_.x - target_.x),  abs(source_.y - target_.y) };
}

float CPathfinder::Heuristic(Coord source_, Coord target_){
    auto delta = std::move(getDelta(source_, target_));
    return (abs(delta.x-delta.y) + (1.4f * ( delta.x > delta.y ? delta.y : delta.x) ));
    // return static_cast<float>(1 * (delta.x + delta.y));
	// return static_cast<float> (sqrt(pow(delta.x, 2) + pow(delta.y, 2)));

	// return 10 * (delta.x + delta.y) + (-6) * std::min(delta.x, delta.y);
}

void CPathfinder::releaseNodes(NodeSet& nodes_){
    for (auto it = nodes_.begin(); it != nodes_.end();) {
        delete *it;
        it = nodes_.erase(it);
    }
}

std::vector<Coord> CPathfinder::getAdyacentTiles(int tileX, int tileY, bool diagonal){
    std::vector<Coord> adyacent;
    adyacent.push_back(Coord(tileX - 1, tileY ));
    adyacent.push_back(Coord(tileX , tileY - 1 ));
    adyacent.push_back(Coord(tileX + 1, tileY ));
    adyacent.push_back(Coord(tileX , tileY + 1 ));
    if(!diagonal){
        return adyacent;
    }
    adyacent.push_back(Coord(tileX - 1, tileY - 1 ));
    adyacent.push_back(Coord(tileX - 1, tileY + 1 ));
    adyacent.push_back(Coord(tileX + 1, tileY - 1 ));
    adyacent.push_back(Coord(tileX + 1, tileY + 1 ));
    return adyacent;
}
