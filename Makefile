
VPP_AGENT_PROJ_DIR=/home/vagrant/download
INCLUDE_PREFIX=github.com/ligato/vpp-agent

VPP_AGENT_API_DIR=${VPP_AGENT_PROJ_DIR}/${INCLUDE_PREFIX}

VPP_MODEL_PATH=${INCLUDE_PREFIX}/plugins/vpp/model
LINUX_MODEL_PATH=${INCLUDE_PREFIX}/plugins/linux/model

VPP_PROTO_OUT=`pwd`/${VPP_MODEL_PATH}
LINUX_PROTO_OUT=`pwd`/${LINUX_MODEL_PATH}

VPP_API=${VPP_AGENT_PROJ_DIR}/${VPP_MODEL_PATH}
LINUX_API=${VPP_AGENT_PROJ_DIR}/${LINUX_MODEL_PATH}

vpp_plugins=\
	 acl	\
	 bfd	\
	 ipsec	\
	 interfaces \
	 l2 \
	 l3 \
	 l4 \
	 nat \
	 stn \

linux_plugins=\
	interfaces \
	l3 \

vpp_files = $(foreach f, $(vpp_plugins), ${VPP_PROTO_OUT}/${f}/${f}.grpc-c.c)
linux_files = $(foreach f, $(linux_plugins), ${LINUX_PROTO_OUT}/${f}/${f}.grpc-c.c)

rpc_source = ${VPP_PROTO_OUT}/rpc/rpc.grpc-c.c
incl=/home/vagrant/workspace/grpc-client/github.com/ligato/vpp-agent/plugins/vpp/model

all:
	gcc  -I. -o client client.c -I$(incl) $(linux_files) $(vpp_files) $(rpc_source) \
		-lgrpc -lgpr -lgrpc-c -lprotobuf-c -lprotobuf

$(addsuffix .vpp, $(vpp_plugins)):
	mkdir -p ${VPP_PROTO_OUT}/$(basename $@)
	# protoc-c --c_out=${VPP_PROTO_OUT}/$(basename $@) -I${VPP_API}/$(basename $@) \
	# 	${VPP_API}/$(basename $@)/$(basename $@).proto
	#
	protoc -I ${VPP_API}/$(basename $@) --grpc-c_out=${VPP_PROTO_OUT}/$(basename $@) \
				--plugin=protoc-gen-grpc-c=/usr/local/bin/protoc-gen-grpc-c \
				${VPP_API}/$(basename $@)/$(basename $@).proto

$(addsuffix .linux, $(linux_plugins)):
	mkdir -p ${LINUX_PROTO_OUT}/$(basename $@)
	# protoc-c --c_out=${LINUX_PROTO_OUT}/$(basename $@) -I${LINUX_API}/$(basename $@) \
	# 	${LINUX_API}/$(basename $@)/$(basename $@).proto
	protoc -I ${LINUX_API}/$(basename $@) --grpc-c_out=${LINUX_PROTO_OUT}/$(basename $@) \
				--plugin=protoc-gen-grpc-c=/usr/local/bin/protoc-gen-grpc-c \
				${LINUX_API}/$(basename $@)/$(basename $@).proto

gen-proto: $(addsuffix .vpp, $(vpp_plugins)) $(addsuffix .linux, $(linux_plugins))
	mkdir -p ${VPP_PROTO_OUT}/rpc
	mkdir -p ${LINUX_PROTO_OUT}
	#protoc-c --c_out=. -I${VPP_AGENT_PROJ_DIR} -I${VPP_API} ${VPP_API}/rpc/rpc.proto
	protoc -I ${VPP_API}  -I${VPP_AGENT_PROJ_DIR} --grpc-c_out=${VPP_PROTO_OUT} \
				--plugin=protoc-gen-grpc-c=/usr/local/bin/protoc-gen-grpc-c \
				${VPP_API}/rpc/rpc.proto

.PHONY: all gen-proto
