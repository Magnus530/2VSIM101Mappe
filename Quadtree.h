#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <utility>
#include <list>
#include <string>
#include <iterator>
#include "trophy.h"
#include "gamemode.h"
#include "renderwindow.h"
#include "visualobject.h"

namespace gsml
{

typedef std::pair<double, double> Point2D;

struct GameObject
{
    Point2D mPosition;
    std::string mName;
    GameObject() {}
    GameObject(const Point2D& position, std::string name)
        : mPosition{position}, mName{name} {}
};

template<class ID, class T>
class QuadTree;

template<class ID, class T>
class iterator : public std::iterator<std::output_iterator_tag, T>
{
private:
    size_t mIndex{0};
    QuadTree<ID, T>& quadTree;

public:
    explicit iterator(QuadTree<ID, T>& quadtree, size_t index = 0);
    T operator*() const;
    iterator & operator++();
    iterator operator++(T);
};

template <class ID, class T>
class QuadTree
{
private:
  Point2D mA;
  Point2D mB;
  Point2D mC;
  Point2D mD;

  QuadTree* mSw;
  QuadTree* mSe;
  QuadTree* mNe;
  QuadTree* mNw;

  std::list<std::pair<ID, T>> mObjects;

  std::vector<T> mAllObjects;

  bool isLeaf() const;
  void traverseAll(std::vector<T>& allObjects);

public:
    QuadTree();
    QuadTree(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D &v4);
    void init(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D &v4);
    void subDivide(int n);
    void print() const;
    void printAll() const;
    std::pair<float, float> getPosition2D();

    QuadTree* insert(const Point2D& point, const ID& id, const T& object);
    QuadTree* find(const Point2D& p);
    const QuadTree* find(const Point2D& p) const;
    T& find(const ID& id);
    T& operator[](ID id);
    void erase(std::string name, VisualObject* vPtr);
    bool checkInteractCollision(VisualObject* vPtr1, VisualObject* vPtr2, float extent);
    int checkStaticCollision(VisualObject* vPtr1, VisualObject* vPtr2, float extent);
    bool checkBombCollision(VisualObject* vPtr1, VisualObject* vPtr2, float extent);

    typename std::vector<T>::iterator begin()
    {
//        std::cout << "calling traversal" << "\n";
//        mAllObjects.erase(mAllObjects.begin());

        mAllObjects.clear();
        traverseAll(mAllObjects);
        return mAllObjects.begin();
    }

    typename std::vector<T>::iterator end()
    {
        return mAllObjects.end();
    }
};

template<class ID, class T>
QuadTree<ID, T>::QuadTree()
    : mSw{nullptr}, mSe{nullptr}, mNe{nullptr}, mNw{nullptr}
{}

template<class ID, class T>
QuadTree<ID, T>::QuadTree(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D &v4)
    : mA{v1}, mB{v2}, mC{v3}, mD{v4}, mSw{nullptr}, mSe{nullptr}, mNe{nullptr}, mNw{nullptr}
{}

template<class ID, class T>
void QuadTree<ID, T>::init(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D &v4)
{
    mA = v1;
    mB = v2;
    mC = v3;
    mD = v4;
}

template<class ID, class T>
void QuadTree<ID, T>::subDivide(int n)
{
    if(n > 0)
    {
        Point2D v1 = Point2D{(mA.first + mB.first) / 2, (mA.second + mB.second) / 2};
        Point2D v2 = Point2D{(mB.first + mC.first) / 2, (mB.second + mC.second) / 2};
        Point2D v3 = Point2D{(mC.first + mD.first) / 2, (mC.second + mD.second) / 2};
        Point2D v4 = Point2D{(mD.first + mA.first) / 2, (mD.second + mA.second) / 2};
        Point2D m = Point2D{(mA.first + mC.first) / 2, (mA.second + mB.second) / 2};

        mSw = new QuadTree(mA, v1, m, v4);
        mSw->subDivide(n-1);
        mSe = new QuadTree(v1, mB, v2, m);
        mSe->subDivide(n-1);
        mNe = new QuadTree(m, v2, mC, v3);
        mNe->subDivide(n-1);
        mNw = new QuadTree(v4, m, v3, mD);
        mNw->subDivide(n-1);
    }
}

template<class ID, class T>
void QuadTree<ID, T>::print() const
{
    Point2D c = Point2D{(mA.first + mC.first) / 2.f, (mA.second + mC.second) / 2.f};

    std::cout << "senter = (" << c.first << ", " << c.second << ")" << "\n";

    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
    {
        std::cout << it->first << ", " << it->second->getName() << "\n";
    }
}

template<class ID, class T>
void QuadTree<ID, T>::printAll() const
{
    print();
    if(mNe)
    {
        mNe->printAll();
    }
    if(mNw)
    {
        mNw->printAll();
    }
    if(mSw)
    {
        mSw->printAll();
    }
    if(mSe)
    {
        mSe->printAll();
    }
}

template<class ID, class T>
bool QuadTree<ID, T>::isLeaf() const
{
    return mSw == nullptr && mSe == nullptr && mNe == nullptr && mNw == nullptr;
}

template<class ID, class T>
QuadTree<ID, T>* QuadTree<ID, T>::insert(const Point2D& point, const ID& id, const T& gameObject)
{
    if(isLeaf())
    {
        mObjects.push_back(std::pair<ID, T>{id, gameObject});
        return this;
    }
    else
    {
        Point2D m = Point2D{(mA.first + mC.first) / 2, (mA.second + mC.second) / 2};

        if(point.second < m.second)
        {
            if(point.first < m.first)
            {
                mSw->insert(point, id, gameObject);
            }
            else
            {
                mSe->insert(point, id, gameObject);
            }
        }
        else
        {
            if(point.first < m.first)
            {
                mNw->insert(point, id, gameObject);
            }
            else
            {
                mNe->insert(point, id, gameObject);
            }
        }
    }
}

template<class ID, class T>
QuadTree<ID, T> *QuadTree<ID, T>::find(const Point2D &p)
{
    if(isLeaf())
    {
        return this;
    }
    else
    {
        Point2D m = {(mA.first + mC.first) / 2, (mA.second + mC.second) / 2};

        if(p.second < m.second)
        {
            if(p.first < m.first)
            {
                mSw->find(p);
            }
            else
            {
                mSe->find(p);
            }
        }
        else
        {
            if(p.first < m.first)
            {
                mNw->find(p);
            }
            else
            {
                mNe->find(p);
            }
        }
    }
}

template<class ID, class T>
const QuadTree<ID, T>* gsml::QuadTree<ID, T>::find(const Point2D& p) const
{
    if(isLeaf())
    {
        return this;
    }
    else
    {
        Point2D m = {(mA.first + mC.first) / 2, (mA.second + mC.second) / 2};

        if(p.second < m.second)
        {
            if(p.first < m.first)
            {
                mSw->find(p);
            }
            else
            {
                mSe->find(p);
            }
        }
        else
        {
            if(p.first < m.first)
            {
                mNw->find(p);
            }
            else
            {
                mNe->find(p);
            }
        }
    }
}

template <class ID, class T>
T& QuadTree<ID, T>::operator[](ID id)
{
    if(isLeaf())
    {
        for (auto it = mObjects.begin(); it != mObjects.end(); it++)
        {
            if(it->first == id)
            {
                return it->second;
            }
        }
    }
}

template <class ID, class T>
void QuadTree<ID, T>::traverseAll(std::vector<T>& allObjects)
{
//    allObjects.clear();

    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
    {
        allObjects.push_back(it->second);
    }
    if(mNe)
    {
        mNe->traverseAll(allObjects);
    }
    if(mNw)
    {
        mNw->traverseAll(allObjects);
    }
    if(mSw)
    {
        mSw->traverseAll(allObjects);
    }
    if(mSe)
    {
        mSe->traverseAll(allObjects);
    }
}

template <class ID, class T>
void QuadTree<ID, T>::erase(std::string name, VisualObject* vPtr)
{
    auto tempObj = std::pair<std::string, VisualObject*>(name, vPtr);
    auto it = std::find(mObjects.begin(), mObjects.end(), tempObj);
//    std::cout << "thename: " << name << "\n";

    if (it != mObjects.end())
    {
        mObjects.erase(it);
    }

//    std::cout << "mAllObject size: " << mAllObjects.size() << " this erase: " << this << "\n";
}

template<class ID, class T>
bool QuadTree<ID, T>::checkInteractCollision(VisualObject* vPtr1, VisualObject* vPtr2, float extent)
{
    QVector3D tempV1 = vPtr1->getPos3D();
    QVector3D tempV2 = vPtr2->getPos3D();

    if((tempV1.x() < tempV2.x() + extent && tempV1.x() > tempV2.x() - extent) &&
            (tempV1.z() < tempV2.z() + extent && tempV1.z() > tempV2.z() - extent))
    {
//        std::cout << "ptr1: " << vPtr1 << ", pos1: " << tempV1.x() << ", " << tempV1.y() << ", " << tempV1.z() << ", extent " << extent << "\n";
//        std::cout << "ptr2: " << vPtr2 << ", pos2: " << tempV2.x() << ", " << tempV2.y() << ", " << tempV2.z() << ", extent " << extent << "\n";
        return true;
    }
    else
    {
        return false;
    }
}

template<class ID, class T>
int QuadTree<ID, T>::checkStaticCollision(VisualObject* vPtr1, VisualObject* vPtr2, float extent)
{
    float oE = 0.1;

    QVector3D tempV1 = vPtr1->getPos3D();
    QVector3D tempV2 = vPtr2->getPos3D();

//    std::cout << "playerpos: " << tempV1.x() << ", " << tempV1.y() << ", " << tempV1.z() << "\n";
//    std::cout << "housepos + extent: " << tempV2.x() + extent << ", " << tempV2.y() << ", plus z: " <<
//                 tempV2.z() + extent << ", minus z: " << tempV2.z() - extent << "\n";

    // East side
    if (tempV1.x() > tempV2.x() + extent - oE && tempV1.x() < tempV2.x() + extent + oE && tempV1.z() < tempV2.z() + extent
            && tempV1.z() > tempV2.z() - extent)
    {
//        std::cout << "test ewall\n";
        return 0;
    }
    // North side
    else if (tempV1.z() < tempV2.z() - extent + oE && tempV1.z() > tempV2.z() - extent - oE && tempV1.x() > tempV2.x() - extent
            && tempV1.x() < tempV2.x() + extent)
    {
//        std::cout << "test nwall\n";
        return 1;
    }
    // West side
    else if (tempV1.x() < tempV2.x() - extent + oE && tempV1.x() > tempV2.x() - extent - oE && tempV1.z() < tempV2.z() + extent
            && tempV1.z() > tempV2.z() - extent)
    {
//        std::cout << "test wwall\n";
        return 2;
    }
    // South side
    else if (tempV1.z() > tempV2.z() + extent - oE && tempV1.z() < tempV2.z() + extent + oE && tempV1.x() > tempV2.x() - extent
            && tempV1.x() < tempV2.x() + extent)
    {
//        std::cout << "test swall\n";
        return 3;
    }
}

//Oppgave 7
template<class ID, class T>
bool QuadTree<ID, T>::checkBombCollision(VisualObject* vPtr1, VisualObject* vPtr2, float extent)
{
    QVector3D tempV1 = vPtr1->getPos3D();
    QVector3D tempV2 = vPtr2->getPos3D();

    if((tempV1.x() < tempV2.x() + extent && tempV1.x() > tempV2.x() - extent) &&
            (tempV1.z() < tempV2.z() + extent && tempV1.z() > tempV2.z() - extent) &&
            (tempV1.y() < tempV2.y() + extent && tempV1.y() > tempV2.y() - extent*7))
    {
//        std::cout << "ptr1: " << vPtr1 << ", pos1: " << tempV1.x() << ", " << tempV1.y() << ", " << tempV1.z() << ", extent " << extent << "\n";
//        std::cout << "ptr2: " << vPtr2 << ", pos2: " << tempV2.x() << ", " << tempV2.y() << ", " << tempV2.z() << ", extent " << extent << "\n";
        return true;
    }
    else
    {
        return false;
    }
}


}

#endif // QUADTREE_H
