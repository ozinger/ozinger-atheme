#ifndef INLINE_HOSTSERV_H
#define INLINE_HOSTSERV_H

typedef struct {
        char *host;
	sourceinfo_t *si;
	int approved;
	char *target;
} hook_host_request_t;

/*
 * do_sethost(user_t *u, char *host)
 *
 * Sets a virtual host on a single nickname/user.
 *
 * Inputs:
 *      - User to set a vHost on
 *      - a vHost
 * 
 * Outputs:
 *      - none
 *
 * Side Effects:
 *      - The vHost is set on the user.
 */
static inline void do_sethost(user_t *u, char *host)
{
	service_t *svs;

        if (!strcmp(u->vhost, host ? host : u->host))
                return;

	svs = service_find("hostserv");

	strshare_unref(u->vhost);
	u->vhost = strshare_get(host ? host : u->host);

        user_sethost(svs->me, u, u->vhost);
}

/*
 * do_sethost_all(myuser_t *mu, char *host)
 *
 * Sets a virtual host on all nicknames in an account.
 *
 * Inputs:
 *      - an account name
 *      - a vHost
 * 
 * Outputs:
 *      - none
 *
 * Side Effects:
 *      - The vHost is set on all users logged into
 *        the account.
 */
static inline void do_sethost_all(myuser_t *mu, char *host)
{
	mowgli_node_t *n;
        user_t *u;

        MOWGLI_ITER_FOREACH(n, mu->logins.head)
        {
                u = n->data;

                do_sethost(u, host);
        }
}
#endif
