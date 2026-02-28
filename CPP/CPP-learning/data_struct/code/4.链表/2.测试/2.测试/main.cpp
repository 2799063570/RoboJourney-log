#include <iostream>
using namespace std;

#define eleType int

struct ListNode {
    eleType val;
    ListNode* next;
    ListNode() : val(0), next(NULL) {}
};

void ListInit(ListNode* list, const eleType* numList, int size)
{
    if (size <= 0) throw std::invalid_argument("Invalid Size");
    ListNode* LinkList = new ListNode[size - 1];
    list->val = numList[0];
    ListNode* cur = list;
    for (int i = 0; i < size - 1; i++)
    {
        LinkList[i].val = numList[i+1];
        cur->next = &LinkList[i];
        cur = cur->next;
    }
    cur->next = nullptr;
}
void print(ListNode* list)
{
    ListNode* curr = list;
    while (curr != NULL)
    {
        cout << curr->val << " ";
        curr = curr->next;
    }
    cout << endl;
}


int main()
{
    ListNode head;
    int nums[] = { 1, 2, 3, 3, 2, 1 };
    ListInit(&head, nums, 6);
    print(&head);

    ListNode* curr = &head;
    while (curr)
    {
        int num = curr->val;
        ListNode* prev = curr;
        while (prev->next)
        {
            if (prev->next->val == num)
            {
                prev->next = prev->next->next;
            }
            else prev = prev->next;
        }
        curr = curr->next;
    }
    print(&head);

    return 0;
}


