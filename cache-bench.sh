#!/bin/bash -xe

OUTPUT_FILE="cache-lat.csv"

WORKLOAD_SIZE=100000

function output_data()
{
	for d in $1; do
		printf "%s," $d >> $OUTPUT_FILE
	done
	echo >> $OUTPUT_FILE
}

function mds_conf_run()
{
	local hostnum=$1
	local mdsperhost=$2

	# Modify configuration in source file
	sed -i "s/^\(\#define HOST_SIZE\s*\).*$/\1$hostnum/" Control.h
	sed -i "s/^\(\#define MDS_SIZE_PER_HOST\s*\).*$/\1$mdsperhost/" Control.h

	# Build
	make -C build/

	# Run
	single_round_data=$(./build/SubtreeVSHashingSim | grep "Average" | awk '{print $6}')
	output_data "$hostnum*$mdsperhost,$single_round_data"

	# Clear log each time
	> Simulator.log
}

function single_mddist_strategy()
{
	local strategy=$1

	output_data "===Dispatch:$strategy==="
	
	# Modify strategy in source file
	sed -i "s/^\(\#define METADATA_STRATEGY\s*\).*$/\1\"$strategy\"/" Control.h

	mds_conf_run 5 5
}

function single_round()
{
	single_mddist_strategy "subtree"
	single_mddist_strategy "hashing"
	single_mddist_strategy "hybrid"
}

function single_clientcache_strategy()
{
	local cachesize=$1
	output_data "=====ClientCache:$cachesize====="

	if [ $cachesize -eq 0 ]; then
		sed -i "s/^\(\#define ENABLE_CLIENT_CACHE\s*\).*$/\10/" Control.h
	else
		sed -i "s/^\(\#define ENABLE_CLIENT_CACHE\s*\).*$/\11/" Control.h
		sed -i "s/^\(\#define CLIENT_LRU_SIZE\s*\).*$/\1$cachesize/" Control.h
	fi
	
	single_round
}

function single_network_strategy()
{
	for ((i=0;i<10;i++)); do
		single_clientcache_strategy $[$i * 10000]
	done
}

function main()
{
	# Set TCP/IP + IPC
	output_data "=======Network:TCP/IP+IPC======="
	sed -i "s/^\(\#define NETWORK_LAT_SAME_HOST\s*\).*$/\1\"ipc\"/" Control.h
	sed -i "s/^\(\#define NETWORK_LAT_OTHER_HOST\s*\).*$/\1\"tcp\"/" Control.h
	single_network_strategy

	# Set IB + IPC
	output_data "=======Network:IPoIB+IPC======="
	sed -i "s/^\(\#define NETWORK_LAT_SAME_HOST\s*\).*$/\1\"ipc\"/" Control.h
	sed -i "s/^\(\#define NETWORK_LAT_OTHER_HOST\s*\).*$/\1\"IPoIB\"/" Control.h
	single_network_strategy
}

# Clear log
> Simulator.log

# Clear output
> ${OUTPUT_FILE}

# Start
main
