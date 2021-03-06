#ifndef CONTAINERAUXILIARYTOOLS_H
#define CONTAINERAUXILIARYTOOLS_H
#include <memory>
#include <algorithm>

class ContainerAuxiliaryTools
{
public:
    template <class T> static bool checkForDuplicates(const T &container)
    {
        for(int index = 0; index < container.size(); index++)
        {
            for(int index2 = index + 1; index2 < container.size(); index2++)
            {

                if(container[index] == container[index2])
                {
                    return true;
                }
            }
        }

        return false;
    }

    template <class T> static bool containersAreDisjoint(const T &container1, const T &container2)
    {
        for(int index1 = 0; index1 < container1.size(); index1++)
        {
            for(int index2 = 0; index2 < container2.size(); index2++)
            {
                if(container1[index1] == container2[index2])
                {
                    return false;
                }
            }
        }

        return true;
    }

    template <class T1, class T2> static void adaptToSmartPointerContainer(const T1 &source, T2 &target)
    {
        //WARNING This is probably VERY dangerous and broken and should't be used!
        std::for_each(source.begin(), source.end(), [&target](const typename T1::value_type &content)
        {
            target.push_back(typename T2::value_type(content));
        });
    }

    template <class SmartPointerContainer, class RawPointerContainer>
    static RawPointerContainer adaptFromSmartPointerContainer(const SmartPointerContainer &source)
    {
        RawPointerContainer target;
        for(auto const smartPtr : source)
        {
            target.push_back(smartPtr.get());
        }
        return target;
    }

    template <class T1, class T2> static void serializeSmartPointerContainer(T1 &stream, const T2 &container)
    {
        std::for_each(container.begin(), container.end(), [&stream](const typename T2::value_type &content)
        {
             stream << *content;
        });
    } //TODO Refactor to accept only smart pointers if possible

    template <class T1, class T2> static void deserializeSmartPointerContainer(T1 &stream, T2 &container)
    {
        std::for_each(container.begin(), container.end(), [&stream](typename T2::value_type &content)
        {
            stream >> *content;
        });
    }//TODO Refactor to accept only smart pointers if possible

    template <class Container> static Container sharedPointerContainerDeepCopy(const Container &source)
    {
        Container copy;
        for(const auto &ptr : source)
        {
            copy.push_back(std::make_shared(*ptr));
        }
        return copy;
    }

private:

};

#endif // CONTAINERAUXILIARYTOOLS_H
