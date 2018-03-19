#ifndef BINFILE_H
#define BINFILE_H

#include <stdio.h>
#include <sstream>
#include <node.h>
#include <locale.h>

int StrToInt(string str);
bool NodeMerge(Node **first, Node **end, char *data, int &NRows);
bool checkName(string word);
char *str_read(FILE *fin);
bool IsInt(QString ToCheck);
char *str_readB(FILE *fin);
short Readfile(Node **first, Node **end, int &NRows, bool Bin, QString Filename = "Firms.csv");

#endif // BINFILE_H
