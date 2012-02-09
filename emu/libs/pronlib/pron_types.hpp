enum MessageType {
	RQ_WELCOME,
	RESP_WELCOME,
	RQ_CREATE_WINDOW,
	RQ_MAP_WINDOW,
	RQ_CREATE_GC,
	RQ_DRAW_LINE
};

enum EventType {
	CREATE_NOTIFY = 1,
};

struct RqWelcome {
	RqWelcome(int protoVersion) {
		this->type = RQ_WELCOME;
		this->protoVersion = protoVersion;
	}

	int type;
	int protoVersion;
};

struct RespWelcome{
	RespWelcome(int rootWinIdent, int identStart, int identEnd) {
		this->type = RESP_WELCOME;
		this->rootWinIdent = rootWinIdent;
		this->identStart = identStart;
		this->identEnd = identEnd;
	}

	int type;
	int rootWinIdent;
	int identStart;
	int identEnd;
};

struct RqCreateWindow {
	
};

struct RqMapWindow {

};

struct RqCreateGC {
	int type;
	int gc;
};

struct RqDrawLine {
	int type;
	int gc;
	int drawable;
	int x1, y1;
	int x2, y2;
};

struct Display {

};

struct Window {

};

struct GC {

};
