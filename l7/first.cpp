#include "D:\labs\l7\second.h"
#include "D:\labs\l7\student.h"
#include "D:\labs\l7\first.h"

first::first(const char* _name,int _course, int _group, int _CCN, int _gr[4]):
Student(_name, _course, _group, _CCN){
    for(int i = 0; i < 4; i++)
        gr[i] = _gr[i];
};

first::first(const first &other):Student(other){
    for(int i = 0; i < 4; i++)
        gr[i] = other.gr[i];
}

first::~first(){};

int first::getGrade(int index) const{
    if(index >=0 && index < 4)
        return gr[index];
    else 
        return 1;
};

void first::setGrade(int index, int grr){
    if(index >= 0 && index < 4)
        gr[index] = grr;
};

double first::average() const{
    double sum = 0;
    for(int i = 0; i < 4; i++)
        sum +=gr[i];
    return sum/4;
};

std::ostream& operator<<(std::ostream& out, const first &student){
    out << "ID: " << student.getID() << std::endl;
    out << "Name: " << student.getName() << std::endl;
    out << "Course: " << student.getCourse() << std::endl;
    out << "Group: " << student.getGroup() << std::endl;
    out << "Credit card number: " << student.getCCN() << std::endl;
    out << "Grades after 1st session: ";
    for(int i = 0; i < 4; i++)
        out << student.getGrade(i) << " " << std::endl;
    out << std::endl;
    return out;
}