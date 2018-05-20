#ifndef CONTAINERAUXILIARYTOOLS_H
#define CONTAINERAUXILIARYTOOLS_H

template<class T>
class ContainerAuxiliaryTools
{
public:
    static bool checkForDuplicates(const T &container)
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

    static bool containersAreDisjoint(const T &container1, const T &container2)
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

private:

};

#endif // CONTAINERAUXILIARYTOOLS_H
