/*
 * ivi_nf.c :
 *  IVI Netfilter Address Translation Kernel Module
 *
 * by haoyu@cernet.edu.cn 2008.10.19
 *
 * Changes:
 *	Wentao Shang	:	Upgrade to 2.6.35 kernel and remove multicast translation functionality.
 */

#include "ivi_nf.h"

static struct net_device *v4_dev, *v6_dev;

static int running;

unsigned int nf_hook4(unsigned int hooknum, struct sk_buff *skb,
		const struct net_device *in, const struct net_device *out,
		int (*okfn)(struct sk_buff *)) {

	if ((!running) || (in != v4_dev)) {
		return NF_ACCEPT;
	}

	return ivi_v4v6_xmit(skb);
}

unsigned int nf_hook6(unsigned int hooknum, struct sk_buff *skb,
		const struct net_device *in, const struct net_device *out,
		int (*okfn)(struct sk_buff *)) {
	
	if ((!running) || (in != v6_dev)) {
		return NF_ACCEPT;
	}

	return ivi_v6v4_xmit(skb);
}

struct nf_hook_ops v4_ops = {
	list	:	{ NULL, NULL },
	hook	:	nf_hook4,
	owner	:	THIS_MODULE,
	pf	:	PF_INET,
	hooknum	:	NF_INET_PRE_ROUTING,
	priority:	NF_IP_PRI_FIRST,
};

struct nf_hook_ops v6_ops = {
	list	:	{ NULL, NULL },
	hook	:	nf_hook6,
	owner	:	THIS_MODULE,
	pf	:	PF_INET6,
	hooknum	:	NF_INET_PRE_ROUTING,
	priority:	NF_IP6_PRI_FIRST,
};

int nf_running(const int run) {
	running = run;
#ifdef IVI_DEBUG
	printk(KERN_DEBUG "IVI: set running state to %d.\n", running);
#endif
	return running;
}

int nf_getv4dev(struct net_device *dev) {
	v4_dev = dev;
	return 0;
}

int nf_getv6dev(struct net_device *dev) {
	v6_dev = dev;
	return 0;
}

int ivi_nf_init(void) {
	running = 0;
	v4_dev = NULL;
	v6_dev = NULL;

	nf_register_hook(&v4_ops);
	nf_register_hook(&v6_ops);

#ifdef IVI_DEBUG
	printk(KERN_DEBUG "IVI: ivi_nf loaded.\n");
#endif
	return 0;
}

void ivi_nf_exit(void) {
	running = 0;

	nf_unregister_hook(&v4_ops);
	nf_unregister_hook(&v6_ops);
	
	if (v4_dev)
		dev_put(v4_dev);

	if (v6_dev)
		dev_put(v6_dev);

#ifdef IVI_DEBUG
	printk(KERN_DEBUG "IVI: ivi_nf unloaded.\n");
#endif
}
