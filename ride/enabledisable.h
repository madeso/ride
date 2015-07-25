#ifndef RIDE_ENABLEDISABLE_H
#define RIDE_ENABLEDISABLE_H

class wxWindowBase;

class EnableDisable
{
public:
  EnableDisable(bool enable);
  const EnableDisable& operator()(wxWindowBase* b) const;
private:
  bool enable_;
};


#endif  // RIDE_ENABLEDISABLE_H
