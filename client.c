
#include <stdio.h>
#include "github.com/ligato/vpp-agent/plugins/vpp/model/rpc/rpc.grpc-c.h"

int main()
{
    grpc_c_client_t *client;
    grpc_c_init(GRPC_THREADS, NULL);

    client = grpc_c_client_init_by_host("localhost:9111", "strongswan", NULL,
            NULL);

    Rpc__PutResponse *rsp = NULL;
    Rpc__DataRequest rq = RPC__DATA_REQUEST__INIT;
    Interfaces__Interfaces__Interface iface = INTERFACES__INTERFACES__INTERFACE__INIT;

    iface.name = "tap0";
    iface.has_enabled = 1;
    iface.enabled = 1;
    iface.has_type = 1;
    iface.type = INTERFACES__INTERFACE_TYPE__TAP_INTERFACE;
    iface.phys_address = "12:E4:0E:D5:BC:DC";
    iface.n_ip_addresses = 1;
    iface.ip_addresses = calloc(1, sizeof(char *));
    iface.ip_addresses[0] = "192.168.20.3/24";

    iface.tap = calloc(1, sizeof(Interfaces__Interfaces__Interface__Tap));
    interfaces__interfaces__interface__tap__init(iface.tap);
    iface.tap->host_if_name = "tap-host";

    rq.n_interfaces = 1;
    rq.interfaces = calloc(1, sizeof(Interfaces__Interfaces__Interface *));
    rq.interfaces[0] = &iface;

    int status = rpc__data_change_service__put(client, NULL, 0, &rq, &rsp, NULL, -1);
    if (rsp) {
        printf("reply\n");
    }
    printf("Finished with %d\n", status);

    return 0;
}
