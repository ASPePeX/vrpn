// vrpn_Dial.h
//	This implements a Dial class. A dial is an object that spins,
// possibly without bound. It returns the fraction of a revolution that
// it has turned as its message type.

#ifndef VRPN_DIAL
#define VRPN_DIAL
#define vrpn_DIAL_MAX 128

#include "vrpn_Connection.h"

class vrpn_Dial {
public:
	vrpn_Dial (const char * name, vrpn_Connection * c = NULL);

	// Called once through each main loop iteration to handle
	// updates.
	virtual void mainloop (const struct timeval * timeout = NULL) = 0;

	// Report changes to conneciton
        vrpn_Connection *connectionPtr();

  protected:
	vrpn_Connection *connection;
	vrpn_float64	dials[vrpn_DIAL_MAX];
	vrpn_int32	num_dials;
	struct timeval	timestamp;
	vrpn_int32 my_id;		// ID of this device to connection
	vrpn_int32 change_m_id;		// change message id

	virtual vrpn_int32 encode_to(char *buf, vrpn_int32 buflen,
		vrpn_int32 dial, vrpn_float64 delta);
        virtual void report_changes (void);  // send report iff changed
	virtual void report (void);  // send report
};

//----------------------------------------------------------
//************** Users deal with the following *************

// User routine to handle a change in analog values.  This is called when
// the analog callback is called (when a message from its counterpart
// across the connetion arrives).


typedef	struct {
	struct timeval	msg_time;	// Timestamp when change happened
	vrpn_int32	dial;		// which dial changed
	vrpn_float64	change;		// Fraction of a revolution it changed
} vrpn_DIALCB;

typedef void (*vrpn_DIALCHANGEHANDLER) (void * userdata,
					  const vrpn_DIALCB info);

// Open a dial device that is on the other end of a connection
// and handle updates from it.  This is the type of device
// that user code will deal with.

class vrpn_Dial_Remote: public vrpn_Dial {
  public:
	// The name of the device to connect to.
        // Optional argument to be used when the Remote MUST listen on
        // a connection that is already open.
	vrpn_Dial_Remote (const char * name, vrpn_Connection * c = NULL);

	// This routine calls the mainloop of the connection it's on
	virtual void mainloop(const struct timeval * timeout = NULL);

	// (un)Register a callback handler to handle dial updates
	virtual int register_change_handler(void *userdata,
		vrpn_DIALCHANGEHANDLER handler);
	virtual int unregister_change_handler(void *userdata,
		vrpn_DIALCHANGEHANDLER handler);

  protected:
	typedef	struct vrpn_RDCS {
		void				*userdata;
		vrpn_DIALCHANGEHANDLER		handler;
		struct vrpn_RDCS		*next;
	} vrpn_DIALCHANGELIST;
	vrpn_DIALCHANGELIST	*change_list;

	static int handle_change_message(void *userdata, vrpn_HANDLERPARAM p);
};

#endif










