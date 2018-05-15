//#ifndef PARSINGTREE_H
//#define ABSTRACTPARSINGTREE_H

//#include <QString>

////Only by means of composition!

//template <class T1>
//class AbstractParsingTree;

//template <class T2>
//class AbstractParsingTreeNode;

//template <class T3>
//class AbstractParsingTreeIterator;

//template <class T>

//class AbstractParsingTree
//{
//public:
//    AbstractParsingTree(const T &string) :
//        root(this, nullptr, QVector<unsigned int>(), 0, string.size() - 1),
//        height(0),
//        string(string)
//    {

//    }

//    AbstractParsingTree(const AbstractParsingTree &other) = delete;
//    AbstractParsingTree &operator=(const AbstractParsingTree &other) = delete;

//    unsigned int getHeight() const
//    {
//        return height;
//    }

//    QString print()
//    {
//        AbstractParsingTreeIterator<T> iter(this);
//        QVector<AbstractParsingTreeNode<T> *> nextLevelNodes{&(*iter)};
//        QString str;

//        for(int currentLevel = 0; currentLevel <= (int) this->getHeight(); currentLevel++)
//        {
//            QVector<AbstractParsingTreeNode<T> *> nextLevelNodes2;
//            std::for_each(nextLevelNodes.begin(),
//                          nextLevelNodes.end(),
//                          [&str, &nextLevelNodes2](AbstractParsingTreeNode<T> *node) {

//                node->printNodeToString(str);

//                std::for_each(node->children.begin(),
//                              node->children.end(),
//                              [&nextLevelNodes2](const shared_ptr<AbstractParsingTreeNode<T>> &node)
//                {
//                    nextLevelNodes2.push_back(node.get());
//                });
//            });
//            str += "\n\n";
//            nextLevelNodes.swap(nextLevelNodes2);
//        }

//        str += "------------------------------\n";

//        return str;
//    }

//    virtual bool operator==(const AbstractParsingTree &other) const = 0;
//    virtual bool operator!=(const AbstractParsingTree &other) const = 0;

//private:
//    AbstractParsingTreeNode<T> root;
//    unsigned int height;
//    T string;

//    friend class AbstractParsingTreeIterator<T>;
//    friend class AbstractParsingTreeNode<T>;

//};

//template <class T>
//class AbstractParsingTreeNode
//{
//public:
//    void appendChild(const unsigned int typeBeginIndex, const unsigned int typeEndIndex)
//    {
//        QVector<unsigned int> coordinates = this->coordinates;
//        coordinates.push_back(this->children.size());

//        children.push_back(make_shared<TypeParsingTreeNode>(TypeParsingTreeNode(this->tree, this, coordinates, typeBeginIndex, typeEndIndex)));
//    }

//    QVector<unsigned int> getCoordinates() const;
//    QString coordinatesToString() const;

//    bool isRoot() const;
//    bool isChildless() const;

//    TypeTokenString getTypeString() const;

//    unsigned int getHeight() const;
//    unsigned int getChildrenNumber() const;
//    unsigned int getOwnChildNumber() const;

//    unsigned int getTypeBeginIndex() const;
//    unsigned int getTypeEndIndex() const;

//private:
//    TypeParsingTreeNode(TypeParsingTree *tree,
//                        TypeParsingTreeNode *parent,
//                        const QVector<unsigned int> &coordinates,
//                        const unsigned int typeBeginIndex,
//                        const unsigned int typeEndIndex,
//                        const MainOperator mainOperator = MainOperator::Primitive);

//    void printNodeToString(QString &str) const;

//    void updateTreeHeight();

//    AbstractParsingTree<T> *tree;
//    AbstractParsingTreeNode *parent;
//    QVector<unsigned int> coordinates;
//    QVector<shared_ptr<AbstractParsingTreeNode<T>>> children;
//    unsigned int typeBeginIndex;
//    unsigned int typeEndIndex;

//    friend class AbstractParsingTreeIterator<T>;
//    friend class AbstractParsingTree<T>;
//};

//#endif // PARSINGTREE_H
