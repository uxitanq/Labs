#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>

class Student{
private: 
   static int next_id;
protected:
   int id;
   char* name;
   int course;
   int group;
   int CCN;
public:
   Student(const char* _name,int _course, int _group, int _CCN);
   Student(const Student &other);
   Student() = delete;
   ~Student();
   void setName(const char* _name);
   const char* getName() const;
   void setCourse(int _course);
   int getCourse() const;
   void setGroup(int _group);
   int getGroup() const;
   int getCCN() const;
   int getID() const;
   virtual double average() const = 0;
   friend std::ostream& operator<<(std::ostream &out, const Student &student);
};

#endif 
