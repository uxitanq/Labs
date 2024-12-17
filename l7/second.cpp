#include "D:\labs\l7\second.h"
#include "D:\labs\l7\student.h"
#include "D:\labs\l7\first.h"

second::second(const char* _name, int _course, int _group, int _creditCardNumber,int _gr1[4],  int _gr2[5]):
first(_name, _course, _group, _creditCardNumber, _gr1){
    for(int i = 0; i < 5; i++)
        gr[i] = _gr2[i];
};

second::second(const second &other) : first(other), gr{other.gr[0], other.gr[1], other.gr[2], other.gr[3], other.gr[4]}{};

second::~second(){};

int second::getGrade(int index) const{
    if(index >= 0 && index < 5)
        return gr[index];
    else
        return 1;
};

void second::setGrade(int index, int grr){
    if(index >= 0 && index < 5)
       gr[index] = grr;  
};

double second::average() const {
    double sum1 = 0;
    for(int i = 0; i < 5; i++){
        sum1 +=gr[i];
    }
    return (first::average() * 4 + sum1) / 9;
};

std::ostream& operator<<(std::ostream& out, const second &student){
    out << "ID: " << student.getID() << std::endl;
    out << "Name: " << student.getName() << std::endl;
    out << "Course: " << student.getCourse() << std::endl;
    out << "Group: " << student.getGroup() << std::endl;
    out << "Credit card number: " << student.getCCN() << std::endl;
    out << "Grades after 2nd session: " << std::endl;
    for(int i = 0; i < 5; i++)
        out << student.getGrade(i) << " " << std::endl;
    out << "Average of grades after 2 sessions: " << student.average();
    out << std::endl;
    return out;
};
