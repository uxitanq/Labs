
#ifndef first_h
#define first_h
#include "student.h"

class first : public Student{
protected:
   int gr[4];
public:
   first(const char* _name,int _course, int _group, int _CCN, int _gr[4]);
   first(const first &other);
   first() = delete;
   ~first();
   int getGrade(int index) const;
   void setGrade(int index, int grr);
   double average() const override;
   friend std::ostream& operator<<(std::ostream& out, const first &student);
};

#endif
