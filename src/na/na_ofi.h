/* Copyright (C) 2017 Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted for any purpose (including commercial purposes)
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the
 *    documentation and/or materials provided with the distribution.
 *
 * 3. In addition, redistributions of modified forms of the source or binary
 *    code must carry prominent notices stating that the original code was
 *    changed and the date of the change.
 *
 *  4. All publications or advertising materials mentioning features or use of
 *     this software are asked, but not required, to acknowledge that it was
 *     developed by Intel Corporation and credit the contributors.
 *
 * 5. Neither the name of Intel Corporation, nor the name of any Contributor
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef NA_OFI_H
#define NA_OFI_H

#include "na.h"
#include "na_private.h"
#include "na_error.h"

#include "mercury_queue.h"
#include "mercury_list.h"
#include "mercury_thread_mutex.h"
#include "mercury_time.h"
#include "mercury_atomic.h"

#include <rdma/fabric.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_errno.h>

#include <arpa/inet.h>
#include <ifaddrs.h>

#ifdef __cplusplus
extern		"C" {
#endif

/**
 * FI VERSION provides binary backward and forward compatibility support.
 * Specify the version of OFI is coded to, the provider will select struct
 * layouts that are compatible with this version.
 */
#ifndef NA_OFI_VERSION
#define NA_OFI_VERSION FI_VERSION(1, 5)
#endif

#ifndef NA_OFI_MAX_URI_LEN
#define NA_OFI_MAX_URI_LEN (128)
#endif

#ifndef NA_OFI_MAX_NODE_LEN
#define NA_OFI_MAX_NODE_LEN (64)
#endif

#ifndef NA_OFI_MAX_PORT_LEN
#define NA_OFI_MAX_PORT_LEN (16)
#endif

enum na_ofi_prov_type {
    NA_OFI_PROV_SOCKETS,
    NA_OFI_PROV_PSM2,
    NA_OFI_PROV_VERBS,
};

/**
 * NA OFI plugin configuration:
 * 1. environment variable - "OFI_PORT"
 *    Set it as a port number, when creating NA classes it will use the
 *    consecutive port numbers start from it.
 *    When it is not set, then will use random port number.
 * 2. environment variable - "OFI_INTERFACE"
 *    Set is as the network device name to be used for OFI communication, for
 *    example "eth0", "ib0" or "ens33" etc.
 *
 * TODO: the below can be removed after libfabric 1.5, as we need not to insert
 *       all nodes' addresses to AV after the version.
 * 3. environment variable - "OFI_PORT_CLI"
 *    Set it as a port number, when creating client NA classes it will use the
 *    consecutive port numbers start from it.
 *    When it is not set, then will use random port number.
 * 4. environment variable - "OFI_NODE_LIST"
 *    Set it as the path of a file which includes all the nodes' addresses, with
 *    the format of:
 *    10.10.22.34:4566
 *    10.32.22.[1-32]:[4566-4573]
 *    which is the IP addresses and port numbers.
 */
struct na_ofi_config {
    /* flag of using consecutive port number for NA classes */
    na_bool_t noc_port_cons;
    /* if (noc_port_cons == true), then (noc_port + 1) is next available port */
    hg_atomic_int32_t noc_port;
    /* the NIC interface (for domain_attr->name hint of fi_getinfo */
    char *noc_interface;

    /* TODO: the below fields can be removed after libfabric 1.5 */

    /* if (noc_port_cons == true), then (noc_port_cli + 1) is next available
     * port for client */
    hg_atomic_int32_t noc_port_cli;
    /* path of the node list file */
    char *noc_node_list_file;
    /* IP addr str for the noc_interface */
    char noc_ip_str[INET_ADDRSTRLEN];
};

na_return_t na_ofi_config_init();
void na_ofi_config_fini();
na_return_t na_ofi_config_node_list(struct fid_av *av_hdl);

extern struct na_ofi_config na_ofi_conf;

#ifdef __cplusplus
}
#endif

#endif /* NA_OFI_H */
