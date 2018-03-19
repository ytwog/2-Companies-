#include <binfile.h>

using namespace std;

char *str_read(FILE *fin)
{
    char *res = NULL;
    char ch = 0;
    fscanf(fin, "%c", &ch);
    while(!feof(fin))
    {
        if(res)
        {
            char *tmp = new char[strlen(res) + 2];
            strcpy(tmp, res);
            tmp[strlen(res)] = ch;
            tmp[strlen(res) + 1] = '\0';
            delete[] res;
            res = tmp;
        }
        else
        {
            res = new char[2];
            res[0] = ch;
            res[1] = '\0';
        }
        fscanf(fin, "%c", &ch);
    }
    return res;
}


char *str_readB(FILE *fin)
{
    char *res = NULL;
    char ch = 0;
    fread(&ch, sizeof(char), 1, fin);
    while(!feof(fin))
    {
        if(res)
        {
            char *tmp = new char[strlen(res) + 2];
            strcpy(tmp, res);
            tmp[strlen(res)] = ch;
            tmp[strlen(res) + 1] = '\0';
            delete[] res;
            res = tmp;
        }
        else
        {
            res = new char[2];
            res[0] = ch;
            res[1] = '\0';
        }
        fread(&ch, sizeof(char), 1, fin);
    }
    return res;
}
bool checkName(string word)
{
    for(int i = 0; i < word.length(); i++)
    {
        if((word[i] == '\n') || (word[i] == '"'))
        {
            return 0;
        }
    }
    return 1;
}

int StrToInt(string str)
{
    int res = 0;
    short Spaces = 0;
    for(int i = 0; i < str.length(); i++)
    {
        if((str[i] == ' ') && (Spaces == 0))
        {
            continue;
        }
        if((str[i] == ' ') && (Spaces == 1))
        {
            Spaces = 2;
            continue;
        }
        if((Spaces < 2) && (str[i] >= '0') && (str[i] <= '9'))
        {
            Spaces = 1;
            res *= 10;
            res += str[i] - 48;
        }
        else
        {
            return -1;
        }
    }
    return res;
}


bool NodeMerge(Node **first, Node **end, char *data, int &NRows)
{
    bool Err = false, Bracket = false, Next = true, StartV = false, ignore = false, wNoBrack = false;
    int pos = 0, budget = -1, members = -1;
    short step = -1;
    string tmp = "", Strname = "", Strtax = "";
    stringstream strS;
    while((!Err) && (pos < strlen(data)))
    {
        if(!StartV)
        {
            if(data[pos] == '\n')
            {
                StartV = true;
            }
            pos++;
        }
        else
        {
            switch(data[pos])
            {
            case ';':
                if(Next)
                {
                    //Err = true;
                }
                else
                {
                    if(wNoBrack)
                    {

                        if(step == 1)
                        {
                            if(!checkName(tmp))
                            {
                                Err = true;
                            }
                            else
                            {
                                Strname = tmp;
                            }
                        }
                        else if(step == 2)
                        {
                            budget = StrToInt(tmp);
                            if(budget == -1)
                            {
                                Err = true;
                            }
                        }
                        else if(step == 3)
                        {
                            members = StrToInt(tmp);
                            if(members == -1)
                            {
                                Err = true;
                            }
                        }
                        else if(step == 4)
                        {
                            Strtax = tmp;
                            if(!IsInt(Strtax.c_str()))
                            {
                                Err = true;
                            }
                            else
                            {
                                Strtax = tmp;
                            }
                        }
                        tmp = "";
                        wNoBrack = false;
                    }
                    Next = true;
                }
                pos++;
                break;
            case '"':
                if(Bracket)
                {
                    if(step == 1)
                    {
                        if(!checkName(tmp))
                        {
                            Err = true;
                        }
                        else
                        {
                            Strname = tmp;
                        }
                    }
                    else if(step == 2)
                    {
                        budget = StrToInt(tmp);
                        if(budget == -1)
                        {
                            Err = true;
                        }
                    }
                    else if(step == 3)
                    {
                        members = StrToInt(tmp);
                        if(members == -1)
                        {
                            Err = true;
                        }
                    }
                    else if(step == 4)
                    {
                        Strtax = tmp;
                        if(!IsInt(Strtax.c_str()))
                        {
                            Err = true;
                        }
                        else
                        {
                            Strtax = tmp;
                        }
                    }
                    tmp = "";
                    Next = false;
                    Bracket = false;
                }
                else
                {
                    if(Next && !wNoBrack)
                    {
                        Bracket = true;
                        step++;
                        if(step > 4)
                        {
                            ignore = true;
                        }
                    }
                    else
                    {
                        Err = true;
                        pos++;
                        break;
                    }
                }
                pos++;
                break;
            case '\n':
                if(!Bracket)
                {
                    if(wNoBrack)
                    {
                        wNoBrack = false;
                        Strtax = tmp;
                    }
                    add(first, *end, end, Strname, budget, members, Strtax);
                    NRows++;
                    Next = true;
                    tmp = Strtax = Strname = "";
                    budget = members = step = -1;
                    ignore = false;
                }
                else
                {
                    Err = true;
                }
                pos++;
                break;
            default:
                if(ignore)
                {
                    pos++;
                    break;
                }
                else if(Bracket)
                {
                    string temp = " ";
                    temp[0] = data[pos];
                    tmp.insert(tmp.length(), temp);
                }
                else if(data[pos] == '/r')
                {

                }
                else if(((wNoBrack) && (data[pos] == ' ')) || (data[pos] != ' '))
                {
                    if(Next || wNoBrack)
                    {
                        if(Next)
                        {
                            Next = false;
                            step++;
                            wNoBrack = true;
                        }
                        string temp = " ";
                        temp[0] = data[pos];
                        tmp.insert(tmp.length(), temp);
                    }
                    else
                    {
                        Err = true;
                    }
                }
                //tmp.append(data[pos]);
                pos++;
                break;
            }
        }
    }
    if(Err)
    {
        return true;
    }
    else
    {
        return false;
    }
}

short Readfile(Node **first, Node **end, int &NRows, bool Bin, QString Filename)
{
    FILE *fin;
    if(Bin)
    {
        fin = fopen(Filename.toStdString().c_str(), "rb");
    }
    else
    {
        fin = fopen(Filename.toStdString().c_str(), "r");
    }
    if(fin)
    {
        char *data;
        if(Bin)
        {
            data = str_readB(fin);
        }
        else
        {
            data = str_read(fin);
        }
        if(NodeMerge(first, end, data, NRows))
        {
            return 2;
        }
    }
    else
    {
        fclose(fin);
        return 1;
    }
    fclose(fin);
    return 0;
}


bool IsInt(QString ToCheck)
{
    string Check = ToCheck.toStdString();
    for(int i = 0; i < ToCheck.length(); i++)
    {
        if((Check[i] > '9') || (Check[i] < '0'))
        {
            return false;
        }
    }
    return true;
}
