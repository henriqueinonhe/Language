#include "typeparsingtreenode.h"
#include "typeparsingtree.h"
#include <QDataStream>
#include "qtclassesdeserialization.h"
#include "containerauxiliarytools.h"

TypeParsingTreeNode::TypeParsingTreeNode(const TypeParsingTree *tree, const TypeParsingTreeNode *parent, const unsigned int typeBeginIndex, const unsigned int typeEndIndex, const MainOperator mainOperator) :
    tree(tree),
    parent(parent),
    typeBeginIndex(typeBeginIndex),
    typeEndIndex(typeEndIndex),
    mainOperator(mainOperator)
{
}

TypeParsingTreeNode::TypeParsingTreeNode(const TypeParsingTreeNode &other, const TypeParsingTree *tree, const TypeParsingTreeNode *parent) :
    tree(tree),
    parent(parent),
    typeBeginIndex(other.typeBeginIndex),
    typeEndIndex(other.typeEndIndex),
    mainOperator(other.mainOperator),
    children(deepCopyChildren(other.children, tree, this))
{
}

void TypeParsingTreeNode::printNodeToString(QString &str) const
{
    /* The printing convention consists on two things:
     * 1st - (Parent Node Index, This Node Index)
     * 2nd - The Type Sort. */

    const auto coordinates = getCoordinates();
    str += "(";
    if(coordinates.size() >= 2)
    {
        str += QString::number(coordinates[coordinates.size() - 2]);
    }
    if(!coordinates.isEmpty())
    {
        str += ",";
        str += QString::number(coordinates.back());
    }
    str += "){";
    str += mainOperatorToString();
    str += ",\"";
    str += getTypeString().toString();
    str += "\"} ";
}

QString TypeParsingTreeNode::mainOperatorToString() const
{
    if(mainOperator == MainOperator::Primitive)
    {
        return "()";
    }
    else if(mainOperator == MainOperator::Composition)
    {
        return "(->)";
    }
    else if(mainOperator == MainOperator::Product)
    {
        return "([])";
    }
    else
    {
        throw std::logic_error("Main Operator is not set!");
    }
}

void TypeParsingTreeNode::appendChild()
{
    const auto stubIndex = 0;
    children.push_back(shared_ptr<TypeParsingTreeNode>(new TypeParsingTreeNode(this->tree, this, stubIndex, stubIndex)));
}

QVector<shared_ptr<TypeParsingTreeNode> > TypeParsingTreeNode::deepCopyChildren(const QVector<shared_ptr<TypeParsingTreeNode> > &source,
                                                                                const TypeParsingTree *tree,
                                                                                const TypeParsingTreeNode *parent) const
{
    QVector<shared_ptr<TypeParsingTreeNode>> copy;
    for(const auto &ptr : source)
    {
        copy.push_back(make_shared<TypeParsingTreeNode>(*ptr, tree, parent));
    }
    return copy;
}

unsigned int TypeParsingTreeNode::getTypeEndIndex() const
{
    return typeEndIndex;
}

bool TypeParsingTreeNode::operator==(const TypeParsingTreeNode &other) const
{
    if(this->typeBeginIndex != other.typeBeginIndex ||
       this->typeEndIndex != other.typeEndIndex ||
       this->mainOperator != other.mainOperator)
    {
       return false;
    }

    if(this->children.size() != other.children.size())
    {
        return false;
    }

    for(auto index = 0; index < children.size(); index++)
    {
        if(*children[index] != *other.children[index])
        {
            return false;
        }
    }

    return true;
}

bool TypeParsingTreeNode::operator!=(const TypeParsingTreeNode &other) const
{
    return !(*this == other);
}

TypeParsingTreeNode::TypeParsingTreeNode(QDataStream &stream,
                                         const TypeParsingTree *tree,
                                         const TypeParsingTreeNode *parent) :
    tree(tree),
    parent(parent),
    typeBeginIndex(QtDeserialization::deserializeUInt(stream)),
    typeEndIndex(QtDeserialization::deserializeUInt(stream)),
    mainOperator(deserializeMainOperator(stream)),
    children(QtDeserialization::deserializeSharedPointerQVector<TypeParsingTreeNode>(stream, tree, static_cast<const TypeParsingTreeNode *>(this)))
{
}

unsigned int TypeParsingTreeNode::getTypeBeginIndex() const
{
    return typeBeginIndex;
}

TypeParsingTreeNode::MainOperator TypeParsingTreeNode::getMainOperator() const
{
    return mainOperator;
}

void TypeParsingTreeNode::setMainOperator(const MainOperator &value)
{
    mainOperator = value;
}

QVector<unsigned int> TypeParsingTreeNode::getCoordinates() const
{
    QVector<unsigned int> coordinates;

    const auto *ptr = this;
    while(!ptr->isRoot())
    {
        coordinates.prepend(ptr->getOwnChildNumber());
        ptr = ptr->parent;
    }

    return coordinates;
}

QString TypeParsingTreeNode::coordinatesToString() const
{
    QString coordinatesString;
    const auto coordinates = getCoordinates();

    coordinatesString += "(";
    if(!coordinates.empty())
    {
        std::for_each(coordinates.begin(), coordinates.end() - 1, [&](unsigned int e) {
            coordinatesString += QString::number(e);
            coordinatesString += ",";
        });
        coordinatesString += QString::number(coordinates.back());
    }
    coordinatesString += ")";

    return coordinatesString;
}

bool TypeParsingTreeNode::isRoot() const
{
    return parent == nullptr;
}

bool TypeParsingTreeNode::isChildless() const
{
    return children.isEmpty();
}

TypeTokenString TypeParsingTreeNode::getTypeString() const
{
    const auto typeStringSize = typeEndIndex - typeBeginIndex + 1;
    return tree->typeString.mid(typeBeginIndex, typeStringSize);
}

unsigned int TypeParsingTreeNode::getHeight() const
{
    auto height = 0;
    const TypeParsingTreeNode *ptr = this;
    while(!ptr->isRoot())
    {
        ptr = ptr->parent;
        height++;
    }

    return height;
}

unsigned int TypeParsingTreeNode::getChildrenNumber() const
{
    return static_cast<unsigned int>(children.size());
}

unsigned int TypeParsingTreeNode::getOwnChildNumber() const
{
    const TypeParsingTreeNode *ptr = parent;

    auto ownChildNumber = 0;
    while(ptr->children[ownChildNumber].get() != this)
    {
        ownChildNumber++;
    }

    return static_cast<unsigned int>(ownChildNumber);
}

unsigned int TypeParsingTreeNode::getGreatestDescendantHeight() const
{
    if(this->isChildless())
    {
        return getHeight();
    }
    else
    {
        QVector<unsigned int> greatestDescendantHeightVector;

        for(const auto &node : children)
        {
            greatestDescendantHeightVector.push_back(node->getGreatestDescendantHeight());
        }

        return *std::max_element(greatestDescendantHeightVector.begin(),
                                greatestDescendantHeightVector.end());
    }
}

TypeParsingTreeNode::MainOperator TypeParsingTreeNode::deserializeMainOperator(QDataStream &stream)
{
    MainOperator mainOperator;
    stream >> mainOperator;
    return mainOperator;
}

void TypeParsingTreeNode::appendChild(const unsigned int typeBeginIndex, const unsigned int typeEndIndex)
{
    children.push_back(shared_ptr<TypeParsingTreeNode>(new TypeParsingTreeNode(this->tree, this, typeBeginIndex, typeEndIndex))); //NOTE Be aware! (Later) Be aware of what??
}


QDataStream &operator <<(QDataStream &stream, const TypeParsingTreeNode &node)
{
    stream << node.typeBeginIndex << node.typeEndIndex << node.mainOperator;
    stream << node.getChildrenNumber();
    for(const auto &child : node.children)
    {
        stream << *child;
    }
    return stream;
}

QDataStream &operator <<(QDataStream &stream, const TypeParsingTreeNode::MainOperator mainOperator)
{
    if(mainOperator == TypeParsingTreeNode::MainOperator::Primitive)
    {
        stream << 0;
    }
    else if(mainOperator == TypeParsingTreeNode::MainOperator::Composition)
    {
        stream << 1;
    }
    else
    {
        stream << 2;
    }
    return stream;
}

QDataStream &operator >>(QDataStream &stream, TypeParsingTreeNode::MainOperator &mainOperator)
{
    unsigned int number;
    stream >> number;
    if(number == 0)
    {
        mainOperator = TypeParsingTreeNode::MainOperator::Primitive;
    }
    else if(number == 1)
    {
        mainOperator = TypeParsingTreeNode::MainOperator::Composition;
    }
    else
    {
        mainOperator = TypeParsingTreeNode::MainOperator::Product;
    }
    return stream;
}
