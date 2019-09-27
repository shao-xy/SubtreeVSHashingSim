#ifndef __CLUSTER_MESSAGES_MSG_OPTYPES_H__
#define __CLUSTER_MESSAGES_MSG_OPTYPES_H__

enum class msg_op_fs_t {
	_NULL = 0,
	LOOKUP,
	CREATE,
	READ,
	WRITE,
	RM,
	MKDIR,
	RMDIR,
	LISTDIR,
};

#endif /* cluster/messages/msg_optypes.h */
