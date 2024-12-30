#ifndef RIDE_TOGUI_H_
#define RIDE_TOGUI_H_

class ToGuiSender
{
public:

	ToGuiSender();
	virtual ~ToGuiSender();

	virtual void SendToGui(bool togui) = 0;
};

#endif	// RIDE_TOGUI_H_
