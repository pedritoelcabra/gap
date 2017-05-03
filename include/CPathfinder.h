#ifndef CPATHFINDER_H
#define CPATHFINDER_H

#include <vector>
#include <set>
#include <functional>
#include <unordered_map>
#include <algorithm>

struct Coord{
    Coord(){};
    Coord(int x_, int y_);

    int x;
    int y;

    bool operator == (const Coord& coord_) const;
};

struct CoordHasher{
      std::size_t operator()(const Coord& k) const
      {
            using std::size_t;
            using std::hash;

            return (hash<int>()(k.x) ^ (hash<int>()(k.y) << 1)) >> 1 ;
      }
};

typedef std::vector<Coord> CoordList;

struct Node{
        float G, H;
        Coord coords;
        Node *parent;

        Node(Coord coord_, Node *parent_ = nullptr);
    float getScore();
};


class CPathfinder{

    public:



        using CoordMap = std::unordered_map<Coord, float, CoordHasher>;
        using NodeSet = std::set<Node*>;


        CPathfinder();
        virtual ~CPathfinder();

        void            Init();

        void            SetCost(int x, int y, float cost);
        float           GetCost(int x, int y);
        float           getMoveCost(Coord coords_);
        CoordList       FindPath(Coord source_, Coord target_, float minv_, float maxv_, bool diagonal = true);

        const float     diagonalCost = 1.414;


        CoordList       getAdyacentTiles(int x, int y, bool diagonal = true);

    protected:

    private:
        float               minv;
        float               maxv;
        unsigned int        directions;
        CoordList           direction;

        CoordMap            CollisionMap;

        Node*               findNodeOnList(NodeSet& nodes_, Coord coords_);
        Coord               getDelta(Coord source_, Coord target_);
        float               Heuristic(Coord source_, Coord target_);
        void                releaseNodes(NodeSet& nodes_);


};

#endif // CPATHFINDER_H
