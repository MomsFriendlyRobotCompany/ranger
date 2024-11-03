#!/bin/bash
# ls
killbg() {
    # for p in "${pids[@]}" ; do
    #     kill "$p";
    # done
    # kill "$1"
    # kill $pids
    echo "killbg pids: ${pids}"
}

# Start programs in the background
./main &
./lidar &
./imu &

# Get PIDs of background jobs
pids=$(jobs -p)

# Wait for some time (e.g., 10 seconds)
sleep 3

# Shut down the programs
# kill $pids
# killbg $pids
# echo "${pids}"

for p in "${pids[@]}" ; do
    echo "kill ${p}"
done
kill $pids