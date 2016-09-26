typedef enum {
	USAGE_G = 0,
	USAGE_RG = 1,
	OPEN_FILE = 2,
	DATA_FILE = 3,
	GRAMM_FILE = 4,
	MEMORY = 5,
	VAR_NAME = 6,
	STRATEGY = 7,
	BEAM = 8,
	DEPTH = 9,
	HEURISTIC = 10,
	STOPPING = 12,
	USAGE_S = 13,
	MULTISTART = 14,
	MAX_DISTANCE = 15
} Error;

extern void eerror(Error e, char *s);
