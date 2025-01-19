#include "D:\labs\l7\second.h"
#include "D:\labs\l7\student.h"
#include "D:\labs\l7\first.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>

int Student::uniqueIDCounter = 0;

Student::Student(const std::string& name, int course, const std::string& group, const std::string& studentCardNumber)
    : id(++uniqueIDCounter), name(name), course(course), group(group), studentCardNumber(studentCardNumber) {}

Student::Student(const Student& other)
    : id(++uniqueIDCounter), name(other.name), course(other.course), group(other.group), studentCardNumber(other.studentCardNumber) {}

Student::~Student(){

};

void Student::setName(const std::string& name) {
    this->name = name;
}

void Student::setCourse(int course) {
    this->course = course;
}

void Student::setGroup(const std::string& group) {
    this->group = group;
}

std::string Student::getName() const {
    return name;
}

int Student::getCourse() const {
    return course;
}

std::string Student::getGroup() const {
    return group;
}

int Student::getID() const {
    return id;
}

std::string Student::getStudentCardNumber() const {
    return studentCardNumber;
}

std::ostream& operator<<(std::ostream& os, const Student& student) {
    os << "ID: " << student.id << ", Name: " << student.name << ", Course: " << student.course
       << ", Group: " << student.group << ", Student Card Number: " << student.studentCardNumber;
    return os;
}
