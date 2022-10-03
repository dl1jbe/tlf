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

/* ------------------------------------------------------------
 *        Cleanup call input field
 *
 *--------------------------------------------------------------*/


#include <stdbool.h>

#include "getexchange.h"
#include "globalvars.h"
#include "change_rst.h"
#include "recall_exchange.h"
#include "tlf_curses.h"
#include "ui_utils.h"
#include "write_keyer.h"

gchar *comment_backup = NULL;
gchar *call_backup = NULL;

/* reset comment */
void cleanup_comment(void) {
    g_free(comment_backup);
    comment_backup = g_strdup(comment);

    comment[0] = '\0';
    normalized_comment[0] = '\0';
}

/* restore comment */
void restore_comment(void) {
    if (comment_backup) {
	g_strlcpy(comment, comment_backup, sizeof(comment));
	g_free(comment_backup);
	comment_backup = NULL;
    }
    checkexchange(comment, true);
}

/* reset hiscall */
void cleanup_hiscall(void) {
    g_free(call_backup);
    call_backup = g_strdup(hiscall);

    hiscall[0] = '\0';	    /* reset hiscall and comment */
    proposed_exchange[0] = '\0';
}

/* restore call */
void restore_hiscall(void) {
    if (call_backup) {
	g_strlcpy(hiscall, call_backup, sizeof(hiscall));
	g_free(comment_backup);
	comment_backup = NULL;
    }
    get_proposed_exchange();
}


void cleanup_qso(void) {
    cleanup_hiscall();
    cleanup_comment();

    g_free(comment_backup);
    comment_backup = NULL;

    g_free(call_backup);
    call_backup = NULL;

    rst_reset();;	    /* reset to 599 */
}

void cleanup(void) {
    extern int defer_store;

    cleanup_qso();
    defer_store = 0;
    keyer_flush();
}
