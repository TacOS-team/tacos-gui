#ifndef __PRONLIB_RESPONSES_H__
#define __PRONLIB_RESPONSES_H__


/**
 * Welcome response.
 * Sent by the server in response to a Hello request.
 */
struct RespWelcome {
	/** Constructor. */
	RespWelcome(int rootWindow, int startId, int endId) {
		this->type = RESP_WELCOME;
		this->rootWindow = rootWindow;
		this->startId = startId;
		this->endId = endId;
	}

	MessageType type; /**< message type, always first */
	int rootWindow; /**< id of the root window */
	int startId; /**< first usable resource id */
	int endId; /**< last usable resource id */
};



/**
 * WindowAttributes response.
 * Sent by the server in response to a GetWindowAttributes request.
 */
struct RespWindowAttributes {
	/** Constructor. */
	RespWindowAttributes(PronWindowAttributes attributes) {
		this->type = RESP_WELCOME;
		this->attributes = attributes;
	}

	MessageType type; /**< message type, always first */
	PronWindowAttributes attributes; /**< Attributes of the window */
};


#endif // __PRONLIB_RESPONSES_H__
