#include "UtilityFunctions.hpp"

void UtilityFunctions::PrintVertexList(const VertexList &printList)
{
    cout << "[";
    if(printList.size() != 0)
    {
        cout << printList[0];
        if(printList.size() > 1)
        {
            for(int i = 1; i < printList.size(); ++i)
            {
                cout << ", ";
                cout << printList[i];
            }
        }
    }
    cout << "]";
    cout << endl;
}
