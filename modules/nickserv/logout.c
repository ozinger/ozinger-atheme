/*
 * Copyright (c) 2005 William Pitcock, et al.
 * Rights to this code are as documented in doc/LICENSE.
 *
 * This file contains code for the CService LOGOUT functions.
 *
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"nickserv/logout", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	"Atheme Development Group <http://www.atheme.org>"
);

static void ns_cmd_logout(sourceinfo_t *si, int parc, char *parv[]);

command_t ns_logout = { "LOGOUT", N_("Logs your services session out."), AC_NONE, 2, ns_cmd_logout, { .path = "nickserv/logout" } };

void _modinit(module_t *m)
{
	service_named_bind_command("nickserv", &ns_logout);
}

void _moddeinit(module_unload_intent_t intent)
{
	service_named_unbind_command("nickserv", &ns_logout);
}

static void ns_cmd_logout(sourceinfo_t *si, int parc, char *parv[])
{
	user_t *u = si->su;
	mowgli_node_t *n, *tn;
	mynick_t *mn;
	char *user = parv[0];
	char *pass = parv[1];

	if ((!si->smu) && (!user || !pass))
	{
		command_fail(si, fault_nochange, _("You are not logged in."));
		return;
	}

	if (user)
	{
		u = user_find(user);

		if (!u)
		{
			command_fail(si, fault_nosuch_target, _("\2%s\2 is not online."), user);
			return;
		}

		if (!u->myuser)
		{
			command_fail(si, fault_nosuch_target, _("\2%s\2 is not logged in."), user);
			return;
		}

		if (u->myuser == si->smu || (pass != NULL && verify_password(u->myuser, pass)))
			notice(nicksvs.nick, u->nick, "You were logged out by \2%s\2.", si->su->nick);
		else
		{
			command_fail(si, fault_authfail, _("Authentication failed. Invalid password for \2%s\2."), entity(u->myuser)->name);
			bad_password(si, u->myuser);
			return;
		}
	}
	else if (si->su == NULL)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "LOGOUT");
		command_fail(si, fault_needmoreparams, _("Syntax: LOGOUT <target> <password>"));
		return;
	}


	if (is_soper(si->smu))
		logcommand(si, CMDLOG_ADMIN, "DESOPER: \2%s\2 as \2%s\2", si->su->nick, entity(si->smu)->name);

	if (si->su != u)
	{
		logcommand(si, CMDLOG_LOGIN, "LOGOUT: \2%s\2", u->nick);
		command_success_nodata(si, _("\2%s\2 has been logged out."), si->su->nick);
	}
	else
	{
		logcommand(si, CMDLOG_LOGIN, "LOGOUT");
		command_success_nodata(si, _("You have been logged out."));
	}

	si->smu->lastlogin = CURRTIME;
	if (si->su != NULL)
	{
		mn = mynick_find(si->su->nick);
		if (mn != NULL && mn->owner == si->smu)
			mn->lastseen = CURRTIME;

		if (!ircd_on_logout(si->su, entity(si->smu)->name))
		{
			MOWGLI_ITER_FOREACH_SAFE(n, tn, si->smu->logins.head)
			{
				if (n->data == si->su)
				{
					mowgli_node_delete(n, &si->smu->logins);
					mowgli_node_free(n);
					break;
				}
			}
			si->su->myuser = NULL;
		}
	}
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */
