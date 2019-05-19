/*
 *	Compass abstraction layer
 */

class COMPASS {

public:
	virtual ~COMPASS() {}
	virtual void getHeading(float &heading) = 0;
};
