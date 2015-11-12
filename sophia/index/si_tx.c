
/*
 * sophia database
 * sphia.org
 *
 * Copyright (c) Dmitry Simonenko
 * BSD License
*/

#include <libss.h>
#include <libsf.h>
#include <libsr.h>
#include <libsv.h>
#include <libsl.h>
#include <libsd.h>
#include <libsi.h>

void si_begin(sitx *x, si *index, int ro)
{
	x->ro = ro;
	x->index = index;
	ss_listinit(&x->nodelist);
	si_lock(index);
}

void si_commit(sitx *x)
{
	/* reschedule nodes */
	sslist *i, *n;
	ss_listforeach_safe(&x->nodelist, i, n) {
		sinode *node = sscast(i, sinode, commit);
		ss_listinit(&node->commit);
		if (! x->ro) {
			si_plannerupdate(&x->index->p, SI_BRANCH, node);
		} else {
		}
	}
	si_unlock(x->index);
}
