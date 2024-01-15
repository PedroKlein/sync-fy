#!/bin/bash

# List of ports to check
PORTS=("8765" "8766" "8767")

# Check each port
for port in "${PORTS[@]}"
do
    echo "Checking for processes using port $port:"
    # Find processes using the specific port
    lsof -i TCP:$port
done