#include "D:\labs\l7\second.h"
#include "D:\labs\l7\student.h"
#include "D:\labs\l7\first.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>

int Student::next_id = 1;

int randID(){
    srand(time(0));
    return rand()^time(0);
};


Student::Student(const char* _name,int _course, int _group, int _CCN): 
 course(_course), group(_group), CCN(_CCN), name(new char[strlen(_name) + 1]) {
   strcpy(name, _name);
   id = next_id;
   next_id++;
}


Student::Student(const Student& other):id(other.id), course(other.course), group(other.group), CCN(other.CCN){
name = new char[strlen(other.name) + 1];
strcpy(name, other.name);
};

Student::~Student(){
    delete[]name;
};

void Student::setName(const char* _name){
    delete[]name;
    name = new char[strlen(_name) + 1];
    strcpy(name, _name);
};

const char* Student::getName() const{
    return name;
};

void Student::setCourse(int _course){
    course = _course;
};

int Student::getCourse() const{
    return course;
};

void Student::setGroup(int _group){
    group = _group;
};

int Student::getGroup() const{
    return group;
};
int Student::getCCN() const{
    return CCN;
};
int Student::getID() const{
    return id;
};

std::ostream& operator<<(std::ostream& out, const Student &student){
   out << "ID: " << student.getID() << std::endl;
   out << "Name of student: " << student.getName() << std::endl;
   out << "Course: " << student.getCourse() << std::endl;
   out << "Group: " << student.getGroup() << std::endl;
   out << "Credit card number: " << student.getCCN() << std::endl;
   return out;
};