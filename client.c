
#include <stdio.h>
#include <protobuf-c-rpc/protobuf-c-rpc.h>
#include "github.com/ligato/vpp-agent/plugins/vpp/model/rpc/rpc.pb-c.h"

static void
do_nothing (ProtobufCRPCDispatch *dispatch, void *unused)
{
    /* empty */
}

static void
run_main_loop_without_blocking (ProtobufCRPCDispatch *dispatch)
{
    protobuf_c_rpc_dispatch_add_idle (dispatch, do_nothing, NULL);
    protobuf_c_rpc_dispatch_run (dispatch);
}

int main()
{
    ProtobufC_RPC_Client *client;
    ProtobufCService *service;
    ProtobufC_RPC_AddressType address_type=0;
    char *name = "127.0.0.1:9111";

    service = protobuf_c_rpc_client_new (address_type, name,
            &rpc__data_change_service__descriptor, NULL);
    if (service == NULL) {
        fprintf(stderr, "service error!\n");
        return 1;
    }
    client = (ProtobufC_RPC_Client *) service;
    protobuf_c_rpc_client_set_autoreconnect_period (client, 1);
    fprintf (stderr, "Connecting... ");

    while (!protobuf_c_rpc_client_is_connected (client))
        protobuf_c_rpc_dispatch_run (protobuf_c_rpc_dispatch_default ());

    fprintf (stderr, "done.\n");

    run_main_loop_without_blocking (protobuf_c_rpc_dispatch_default ());

    return 0;
}
