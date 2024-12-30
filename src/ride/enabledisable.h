// Copyright (2015) Gustav

#ifndef RIDE_ENABLEDISABLE_H_
#define RIDE_ENABLEDISABLE_H_

class wxWindowBase;

class EnableDisable
{
public:

	explicit EnableDisable(bool enable);
	const EnableDisable& operator<<(wxWindowBase* b) const;

private:

	bool enable_;
};

#endif	// RIDE_ENABLEDISABLE_H_
