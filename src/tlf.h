/*
 * Tlf - contest logging program for amateur radio operators
 * Copyright (C) 2001-2002-2003 Rein Couperus <pa0rct@amsat.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */


#ifndef TLF_H
#define TLF_H

#include <stdbool.h>
#include <time.h>

#include "hamlib/rig.h"
#include "bandmap.h"
#include "debug.h"
#include "dxcc.h"

enum {
    NO_KEYER,
    NET_KEYER,
    MFJ1278_KEYER,
    GMFSK,
    FLDIGI,
    HAMLIB_KEYER,
};

#define SINGLE 0        /* single op */
#define MULTI 1         /* multi op / single tx */

#define TELNET_INTERFACE 1
#define TNC_INTERFACE 2
#define NETWORK_INTERFACE 3
#define FIFO_INTERFACE 4

#define NOCLUSTER 0     /*  no cluster info  */
#define MAP 1           /*  show spots */
#define CLUSTER 3 		/*  full cluster info  */
#define FREQWINDOW 4    /* M/M frequencies */

#define SHORTCW 1       /*  short  cw characters in  exchange  (e.g. 0 = T,  9 = N) */
#define LONGCW 0

typedef enum {
    CQ,         // Run
    S_P,        // Search and Pounce
    AUTO_CQ,    // temporary, used in autocq.c
    NONE        // used in trx_memory to signal empty memory
} cqmode_t;

extern bool keyboard_mode;

#define FILTER_ALL 0 	/*  filter announcements */
#define FILTER_ANN 1
#define FILTER_TALK 2
#define FILTER_DX 3

#define CWMODE 0
#define SSBMODE 1
#define DIGIMODE 2

#define  BAND60 512
#define  BAND30 256
#define  BAND17 128
#define  BAND12 64
#define  BAND160 32
#define  BAND80 16
#define  BAND40 8
#define  BAND20 4
#define  BAND15 2
#define  BAND10 1
#define  BANDOOB 0  // out of band


enum {
    BANDINDEX_ANY = -1,
    BANDINDEX_160 = 0,
    BANDINDEX_80,
    BANDINDEX_60,
    BANDINDEX_40,
    BANDINDEX_30,
    BANDINDEX_20,
    BANDINDEX_17,
    BANDINDEX_15,
    BANDINDEX_12,
    BANDINDEX_10,
    BANDINDEX_OOB,	/* out of band */
    NBANDS
};


/* display color sets */
enum {
    C_HEADER = 2,
    C_BORDER,
    C_INPUT,
    C_DUPE,
    C_WINDOW,
    C_LOG
};

#define NORMCOLOR C_INPUT
#define ISDUPE 1
#define NODUPE 0

#define MAX_CALL_LENGTH 13
#define MAX_QSOS 20000          /* internal qso array */
#define MAX_DATALINES 1000      /* from ctydb.dat  */
#define MAX_CALLS 5000          /* max nr of calls in dupe array */
#define MAX_MULTS 1000          /* max nr of mults in mults array */
#define	MAX_SPOTS 200		/* max nr. of spots in spotarray */
#define CQ_ZONES 40
#define ITU_ZONES 90
#define MAX_ZONES (ITU_ZONES + 1) /* size of zones array */

#define MAX_SECTION_LENGTH 5

enum {
    MULT_NONE,      // multiplier not used
    MULT_ALL,       // multiplier counted once on all bands
    MULT_BAND,      // multiplier counted once per each band
};

#define EXCLUDE_NONE 0
#define EXCLUDE_CONTINENT 1
#define EXCLUDE_COUNTRY 2

#define LOGLINELEN (88)		/* Length of logline in logfile
				   (including linefeed) */
#define MINITEST_DEFAULT_PERIOD 600
/* ignore dupe state when MINITEST is set
 * and last QSO was not in actual period */

/* special message numbers */
enum {
    SP_TU_MSG = 12,
    CQ_TU_MSG = 13,
    SP_CALL_MSG = 24
};

/** my station info
 *
 * contains all information about my station */
typedef struct {
    char call[20];
    int countrynr;
    char continent[3];
    int cqzone;
    char qra[7];
    double Lat;     // +: north, -: south
    double Long;    // +: west,  -: east
} mystation_t;

/** worked station
 *
 * contains all information about an already worked station */
typedef struct {
    char call[20]; 		/**< call of the station */
    char exchange[24]; 		/**< the last exchange */
    int band; 			/**< bitmap for worked bands */
    prefix_data *ctyinfo;	/**< pointer to country info from cty.dat */
    long qsotime[3][NBANDS];	/**< last timestamp of qso in gmtime
				  for all modes and bands */
} worked_t;

#define MULT_SIZE   12
/** worked mults
 *
 * all information about worked multis */
typedef struct {
    char name[MULT_SIZE];	/**< Multiplier */
    int band;			/**< bitmap with bands the multi was worked */
} mults_t;


#define MAXPFXNUMMULT 30
#define PFXNUMBERS 10
typedef struct {
    int countrynr;
    int qsos[PFXNUMBERS];
} pfxnummulti_t;

#define COMMENT_SIZE    80
#define CALL_SIZE       20

/* represents different parts of a qso line */
struct qso_t {
    char *logline;
    bool is_comment;
    int band;
    int bandindex;
    int mode;
    char day;
    char month;
    int year;
    int hour;
    int min;
    time_t timestamp;
    int qso_nr;
    char *call;
    int rst_s;
    int rst_r;
    char *comment;
    freq_t freq;
    int tx;
    int qsots;
    char *mult1_value;
    char *callupdate;           // transient field, used in checkexchange
    char *normalized_comment;   // transient field
    char *section;              // transient field
};


void refreshp();

extern const char *backgrnd_str;

typedef enum {
    QSO,
    DXPED,
    WPX,
    CQWW,
    SPRINT,
    ARRLDX_USA,
    ARRLDX_DX,
    ARRL_SS,
    ARRL_FD,
    PACC_PA,
    STEWPERRY,
    FOCMARATHON,
    UNKNOWN
} contest_type_t;

#define QSO_MODE ("qso")

typedef enum {
    DEFAULT = 0,	/* undefined -> DEFAULT */
    FIXED,
    FUNCTION
} points_type_t;


/** contest configuration
 */
typedef struct {
    contest_type_t	id;
    char		*name;
    bool		recall_mult;
    bool		exchange_serial;
    int                 exchange_width;
    struct {
	points_type_t type;
	union {
	    int point;
	    int (*fn)(struct qso_t *);
	};
    }			points;
    bool (*is_multi)(spot *data);

} contest_config_t;

/**< Bitmask for Hamlib CAT PTT
 * bit 0 set: CAT PTT wanted--RIGPTT in logcfg.dat (set in parse_logcfg)
 * bit 1 set: CAT PTT available--from rig caps (set in sendqrg)
 * bit 2 set: PTT active (set/unset in gettxinfo)
 * bit 3 set: PTT On (set/unset in callinput)
 * bit 4 set: PTT Off (set/unset in callinput)
 */
enum {
    CAT_PTT_WANTED      = (1 << 0),
    CAT_PTT_AVAILABLE   = (1 << 1),
    CAT_PTT_USE         = CAT_PTT_WANTED | CAT_PTT_AVAILABLE,
    CAT_PTT_ACTIVE      = (1 << 2),
    CAT_PTT_ON          = (1 << 3),
    CAT_PTT_OFF         = (1 << 4),
};

/* Enums for Xplanet interface */
enum {
    MARKER_NONE,
    MARKER_ALL,		/* DOTS, CALLS, last cluster line */
    MARKER_DOTS,	/* only DOTS */
    MARKER_CALLS,	/* DOTS & CALLS */
};

/* Enums for RESEND_CALL feature */
enum {
    RESEND_NOT_SET,		/* Resend feature not set */
    RESEND_PARTIAL,		/* Resend partial call */
    RESEND_FULL,		/* Resend full call again */
};

#define FREE_DYNAMIC_STRING(p)  if (p != NULL) {g_free(p); p = NULL;}

#define LEN(array) (sizeof(array) / sizeof(array[0]))

#define QSOS(n)    (((struct qso_t*)g_ptr_array_index(qso_array, n))->logline)
#define NR_QSOS	   (qso_array->len)

/* Fix for changed rigerror() behaviour */
#if HAMLIB_VERSION > 450
#define tlf_rigerror(errcode) rigerror2(errcode)
#else
#define tlf_rigerror(errcode) rigerror(errcode)
#endif

#endif /* TLF_H */

