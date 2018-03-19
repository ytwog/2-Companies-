#ifndef NODE_H
#define NODE_H

#include <QMainWindow>
#include <string.h>
#include <string>

using namespace std;

class Node
{
public:
    Node();
    Node(const string _name,const int _budget,const int _members,const string _tax);
    ~Node();
    void operator=(Node &ToCopy);
    Node *next;
    Node *prev;

    string Getname();
    int Getbudget();
    int Getmembers();
    string Gettax();
    void Setname(const string _name);
    void Setbudget(const int _budget);
    void Setmembers(const int _members);
    void Settax(const string _tax);
private:
    string name;
    int budget;
    int members;
    string tax;
};

void delNode(Node **first, Node **end, int targ);
bool IsIndividual(Node *obj, string Name, Node *stop);
Node add(Node **first, Node *last, Node **end);
void add(Node **first, Node *last, Node **end, string _name, int _budget, int _members, string _tax);

#endif // NODE_H
