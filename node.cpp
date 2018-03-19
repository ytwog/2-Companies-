#include "node.h"

using namespace std;

Node::Node()
{
    name = "";
    next = NULL;
    prev = NULL;
    budget = 0;
    members = 0;
    tax = "";
}

bool IsIndividual(Node *obj, string Name, Node *stop)
{
    while(obj && (obj != stop))
    {
        if(Name == obj->Getname())
        {
            return 0;
        }
        obj = obj->next;
    }
    return 1;
}

Node add(Node **first, Node *last, Node **end)
{
    Node *tmp = NULL;
    tmp = new Node();
    if(*first)
    {
        if(last->next)
        {
            (last->next)->prev = tmp;
            tmp->next = last->next;
        }
        tmp->prev = last;
        last->next = tmp;
        if(*end == last)
        {
            *end = tmp;
        }
    }
    else
    {
        *first = tmp;
        *end = tmp;
    }
}


void delNode(Node **first, Node **end, int targ)
{
    Node *tmp = *first;
    targ--;
    while((tmp) && (targ))
    {
        tmp = tmp->next;
        targ--;
    }
    if((!tmp) && (targ))
    {
        return;
    }
    if(tmp)
    {
        if(tmp->prev)
        {
            (tmp->prev)->next = tmp->next;
        }
        else
        {
            *first = tmp->next;
        }
        if(tmp->next)
        {
            (tmp->next)->prev = tmp->prev;
        }
        else
        {
            *end = tmp->prev;
        }
    }
    delete tmp;
}


void Node::operator=(Node &ToCopy)
{
    Setname(ToCopy.Getname());
    Setbudget(ToCopy.Getbudget());
    Setmembers(ToCopy.Getmembers());
    Settax(ToCopy.Gettax());
    next = ToCopy.next;
    prev = ToCopy.prev;
}

Node::Node(const string _name,const int _budget,const int _members,const string _tax)
{
    Setname(_name);
    Setbudget(_budget);
    Setmembers(_members);
    Settax(_tax);
    next = NULL;
    prev = NULL;
}

Node::~Node()
{

}



void add(Node **first, Node *last, Node **end, string _name, int _budget, int _members, string _tax)
{
    Node *tmp = NULL;
    tmp = new Node(_name, _budget, _members, _tax);
    if(*first)
    {
        if(last->next)
        {
            (last->next)->prev = tmp;
            tmp->next = last->next;
        }
        tmp->prev = last;
        last->next = tmp;
        if(*end == last)
        {
            *end = tmp;
        }
    }
    else
    {
        *first = tmp;
        *end = tmp;
    }
}

void Node::Setname(const string _name)
{
    name = _name;
}

void Node::Settax(const string _tax)
{
    tax = _tax;
}

void Node::Setbudget(const int _budget)
{
    budget = _budget;
}

void Node::Setmembers(const int _members)
{
    members = _members;
}

string Node::Getname()
{
    return name;
}

string Node::Gettax()
{
    return tax;
}

int Node::Getbudget()
{
    return budget;
}

int Node::Getmembers()
{
    return members;
}
