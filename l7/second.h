#ifndef second_h
#define second_h
#include "first.h"

class second : public first{
protected:
  int gr[5];
public:
  second(const char* _name, int _course, int _group, int _creditCardNumber,int _gr1[4], int _gr2[5]);
  second(const second &other);
  second() = delete;
  ~second();
  int getGrade(int index) const;
  void setGrade(int index, int grr);
  double average() const override;
  friend std::ostream& operator<<(std::ostream& out, const second &student);
};

#endif 