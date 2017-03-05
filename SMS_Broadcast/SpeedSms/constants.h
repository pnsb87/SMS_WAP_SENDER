#pragma once
#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

// For developer build, must removed in final build, to reduce time
//#define DEV_BUILD

// For FFA Device, comment this to use HUAWEI
#define  FFA_DEVICE

#define START_SLOT_PBK	2000
#define START_SLOT_SMS	2000

// Define for wait device busy
//#define WAIT_DEVICE_BUSY
// Post signal to UI from PhoneGateway
//#define _POST_SIGNAL

//#define DATA_FOLDER "data\\"
//#define LOG_FOLDER "log\\"

#define BUFFER_SIZE 32768
#define RX_BUFFER_SIZE BUFFER_SIZE * 2

#define PATH_SIZE MAX_PATH * 4

#define DATA_FOLDER "\\Viettel\\"
#define LOG_FOLDER ""
#define PBK_FILENAME "phonebook.ved"
#define GROUP_FILENAME "group.ved"
#define DATABASE_FILENAME "DCOM3G.db"
#define DATABASE_BAK_FILENAME "DCOM3G.db"
#define GROUP_BAK_FILENAME "group_bak.ved"
#define PBK_BAK_FILENAME "phonebook_bak.ved"
#define SMS_FILENAME "sms.ved"
#define SMS_BAK_FILENAME "sms_bak.ved"
#define CALL_FILENAME	"calllog.ved"
#define PROFILE_FILENAME "profile.ved"
#define DATA_FILENAME "data.ved"
#define EFS_TMP_FILENAME "tmp.efs"
#define ATLOG_FILENAME "atlog.txt"
#define PHONELOG_FILENAME "phonelog.txt"
#define CONNLOG_FILENAME "connlog.txt"
#define HUAWEI_LOG "huawei.log"
#define VIETTEL_LOG "viettel.log"
#define SMS_SEPARATE "\t"
#define SMS_REPLACE_N "%n%"
#define SMS_REPLACE_R "%r%"
#define SMS_REPLACE_QUOTATION "\f\""
#define DEFAULT_PROFILE "\"VIETTEL\",\"e-connect\",\"*99#\",\"\",\"\""
#define DEFPROFILE_KEY "DefProfile"
#define DEFAULT_APN "e-connect"
#define SQLITELOG_FILENAME "sqlite.log"


#define MAX_TRY_GETSIMSTATUS 50
#define MAX_TO_TRY_REPLUG 10
#define  MAX_TRY_SENDAT 3
//#define  for max try send SMS
#define MAX_TRY_SENDSMS 3
#define RELAX_SENDSMS_TIME 5000

// Define for sms content length
#define SMSLENGTH_UNICODE 70
#define SMSLENGTH_NONUNICODE 160
#define MAX_LONGSMS	5

#define  MAX_NUMBER_LEN		20

// Max group name
#define MAX_GROUP_NAME 40

// Max number of group entries
#define MAX_GROUP_ENTRIES 100

#define MAX_SMS_ENTRIES 5000

// Lower bound signal for 2G
#define MIN_LOWER_2G	-102
#define MIN_LOWER_3G	-107


#define EFS_LAST_FILE_REPLACE	"last.rep"

#define EFS_MAX_DATA_FILE		10
#define EFS_MAX_IMSI_LENGTH		40

// dungtx: add 09-06-10
#define SMS_DLG_NEWMSG		0
#define SMS_DLG_REPLYSMS	1
#define SMS_DLG_FORWARDSMS	2
#define SMS_DLG_RESENDSMS	3
// dungtx: end

// Parameter for signal error

// reconnect timer in second
#define RECONNECT_TIME 10
// MAX try to reconnect
#define MAX_RECONNECT	3

// Signal threshold
#define SIGNAL_THRESHOLD -100

// Drop time count allowed
#define MAXSIGNALCOUNT 1

//#define SMS_INBOX0 0
//#define SMS_INBOX1 1
#define SMS_INBOX 1


// define max try open
#define MAX_OPEN 10


#ifndef SMS_OUTBOX
#define  SMS_OUTBOX 2
#endif // SMS_OUTBOX

#ifndef SMS_SENT
#define  SMS_SENT 3
#endif // SMS_SENT

#define SMS_DRAFT 4
#define SMS_TRASH 5
#define SMS_LOCAL_ALL 6
#define SMS_SIM_ALL 7


// define for call value
#define CALL_DIAL	2
#define CALL_RECV	1
#define CALL_MISS	0
#define CALL_ALL	-1
#define CALL_MAX_LOG_COUNT	20	// save only 20 log call

#define SIM	"SIM"
#define PC	"Local"
#define ALL "ALL"
#define EXPORT_ALL "ALL"
#define EXPORT_SEL "SEL"
#define MODEM_ERROR			0

enum STORAGETYPE
{
	_PC = 0,
	_SIM,
	_ALL
};

#define NETMODE_AUTO 0
#define NETMODE_2G 1
#define NETMODE_3G 2

// Mode: 0: Auto
//		 1: 2G
//		 2: 3G

#define SMS_TERMINATOR_CHARACTER "(end)"
/* reference for event type *******
#define MODEM_NOTIFICATION	1
#define MODEM_DEVICE		2
#define MODEM_SMS			3
#define MODEM_CONNECTION	4
#define MODEM_PHONEBOOK		5
**********************************/

#define STATUS_CHANGED		1
#define DEVICE_ERROR		2
#define SMS_ERROR			3
#define CONN_ERROR			4
#define PHONEBOOK_ERROR		5
#define NETWORK_ERROR		6

// Modem status here, must be in order
#define STATUS_REMOVED		11	// Device is physically removed
#define STATUS_NOTREADY		12	// Device inserted but not free, another holded, driver failed, firmware run malfunctioned
#define STATUS_PINREQUEST	13	// Device inserted but not SIM PIN/PUK is requested
#define STATUS_SIMERROR		14	// Device inserted, but sim ERROR, or PUK locked 
#define STATUS_INIT			15	// Device inserted but not SIM PIN/PUK is requested
#define STATUS_REGISTER		16	// Device is searching for network 
#define STATUS_READY		17	// Device is ok and idle
#define STATUS_ONOPERATION	18	// Device is on operation of connecting/disconnecting to internet/ sending SMS
#define STATUS_CONNECTED	19	// Device is ok and connected to internet
#define STATUS_IMSI_LOADED	20	// IMSI has been loaded, then parse imsi to ConnManager to load data from EFS


#define DEVICE_INSERTED		21
#define DEVICE_REMOVED		22
#define DEVICE_OPENED		23
#define DEVICE_CLOSED		24
#define DEVICE_SIGNAL		25
#define DEVICE_PINREQUEST	26
#define DEVICE_BUSY			27
#define DEVICE_SIMLOCK		28
#define DEVICE_LOCKCHANGE	29

#define SMS_SENTSUCCESS		31
#define SMS_DELIVERED		32
#define SMS_NEWSMS			32
#define SMS_READ			33
#define SMS_DELETED			34
#define SMS_SENDING			35
#define CUSD_NOTIFICATION	36
#define SMS_SENTFAILED		37
#define SMS_SOMESENTFAILED	38
#define SMS_FAILED_ADD_SENT	39
#define SMS_FAILED_ADD_OUTBOX	40

#define CONN_CONNECTIONDATA	41
#define CONN_CONNECTED		42
#define CONN_DISCONNECTED	43
#define CONN_LINKSPEED		44
#define CONN_UPLOADED		45
#define CONN_DOWNLOADED		46
#define CONN_DURATION		47
#define CONN_CONNECTING		48
#define CONN_DISCONNECTING	49
#define CONN_UPSPEED		50
#define CONN_DOWNSPEED		51
//#define CONN_HANGUP			52
#define CONN_DROP				53
//#define CONN_SIGNALDROP		54
#define CONN_LIMIT			55

// Definition for voice call
#define CALL_INCOMING		61
#define CALL_INCOMING_CONTINUE 62
#define CALL_CONNECTED		63
#define CALL_END			64
#define CALL_SOUND_DISABLE_OK 65
#define CALL_SOUND_DISABLE_FAILED 66

#define CALL_SOUND_ENABLE_OK 67
#define CALL_SOUND_ENABLE_FAILED 68
#define CALL_SOUND_PROCESSING 69

#define GUI_CHANGECAT		60
#define DEVICE_NOT_REGISTER 70


#define MINIMIZED_GUI_MINIMIZING	90
#define MINIMIZED_GUI_MAXIMIZING	91
#define MINIMIZED_GUI_CLOSING		92

//#define _ENABLE_OUTPUT_DEBUG_

#define _ATLOG

// Use char array as queue instead of std::queue
#define _CHARQUEUE

// Use post window directly instead of Event handler
#define _POSTWINDOW

// Set the connection as default gateway
#define _DEFAULT_GATEWAY


// HaTT: macro remarking whether automatic APN spinning is enable
#define ENABLE_APN_SPINNING

// Define write log signal or not
//#define _LOG_SIGNAL

// Define to tesk leak
#define _TEST_LEAK

// Define max profile name
#define MAX_PROFILE_NAME 20

// Define max number of profile list
#define MAX_PROFILE_ENTRY 100

// Define for closing port after close application
//#define _CLOSE_PORT

#endif //__CONSTANTS_H__