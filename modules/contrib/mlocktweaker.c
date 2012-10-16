/*
 * Copyright (c) 2007 William Pitcock
 * Rights to this code are as documented in doc/LICENSE.
 *
 * mlocktweaker.c: A module which tweaks mlock on registration.
 *
 */

#include "atheme.h"

/*
 * Set this to the string of mlock changes you want to make.
 * This is in addition to the default mlock, so -nt if you want to
 * remove those mlocks, etcetera.
 */
#define MLOCK_CHANGE "-t+c"

DECLARE_MODULE_V1
(
	"contrib/mlocktweaker", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	"William Pitcock <nenolod@atheme.org>"
);

static void handle_channel_register(hook_channel_req_t *hdata);

void _modinit(module_t *m)
{
	hook_add_event("channel_register");
	hook_add_first_channel_register(handle_channel_register);
}

void _moddeinit(module_unload_intent_t intent)
{
	hook_del_channel_register(handle_channel_register);
}

static void handle_channel_register(hook_channel_req_t *hdata)
{
	mychan_t *mc = hdata->mc;
	unsigned int *target;
	char *it, *str = MLOCK_CHANGE;

	if (mc == NULL)
		return;

	target = &mc->mlock_on;
	it = str;

	while (*it != '\0')
	{
		switch (*it)
		{
			case '+':
				target = &mc->mlock_on;
				break;
			case '-':
				target = &mc->mlock_off;
				break;
			default:
				*target |= mode_to_flag(*it);
				break;
		}
		++it;
	}

	mc->mlock_off &= ~mc->mlock_on;
}
