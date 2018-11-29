
#include <stdio.h>
#include "vpp/model/rpc/rpc.grpc-c.h"

int main()
{
    grpc_c_client_t *client;
    grpc_c_init(GRPC_THREADS, NULL);

    client = grpc_c_client_init_by_host("localhost:9111", "strongswan", NULL,
            NULL);

    grpc_c_status_t st;
    memset(&st, 0, sizeof(st));

    Rpc__PutResponse *rsp = NULL;
    Rpc__DataRequest rq = RPC__DATA_REQUEST__INIT;
    Ipsec__TunnelInterfaces__Tunnel t = IPSEC__TUNNEL_INTERFACES__TUNNEL__INIT;

    t.local_ip = "192.168.66.2";
    t.local_spi = 2222;
    t.remote_ip = "192.168.66.1";
    t.has_local_spi = 1;
    t.remote_spi = 1111;
    t.has_remote_spi = 1;

    t.crypto_alg = IPSEC__CRYPTO_ALGORITHM__AES_CBC_128;
    t.has_crypto_alg = 1;
    t.local_crypto_key = "4a506a794f574265564551694d653768";
    t.remote_crypto_key = "4a506a794f574265564551694d653768";

    t.integ_alg = IPSEC__INTEG_ALGORITHM__SHA1_96;
    t.has_integ_alg = 1;
    t.local_integ_key = "4339314b55523947594d6d3547666b45764e6a58";
    t.remote_integ_key = "4339314b55523947594d6d3547666b45764e6a58";

    rq.n_tunnels = 1;
    rq.tunnels = calloc(1, sizeof(void *));
    rq.tunnels[0] = &t;

    int status = rpc__data_change_service__put(client, NULL, 0, &rq, &rsp, &st, -1);
    if (rsp) {
        printf("reply\n");
    }
    printf("Finished with %d\n", status);
    printf("gcs code: %d\n", st.gcs_code);
    printf("gcs msg: %s\n", st.gcs_message);

    return 0;
}
