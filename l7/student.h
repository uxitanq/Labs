#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>

class Student {
protected:
    static int uniqueIDCounter;
    const int id;
    std::string name;
    int course;
    std::string group;
    const std::string studentCardNumber;

public:
    Student(const std::string& name, int course, const std::string& group, const std::string& studentCardNumber);
    Student(const Student& other);

    void setName(const std::string& name);
    void setCourse(int course);
    void setGroup(const std::string& group);
    Student() = delete;
   ~Student();

    std::string getName() const;
    int getCourse() const;
    std::string getGroup() const;
    int getID() const;
    std::string getStudentCardNumber() const;

    virtual void printAverageGrade() const = 0;

    virtual double getAverage() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Student& student);
};

#endif
