#!/bin/bash

# List of TCP ports to close
PORTS=("8765" "8766" "8767" "8768")

# Apply iptables rules to close the ports
for port in "${PORTS[@]}"
do
    sudo iptables -A INPUT -p tcp --dport $port -j DROP
    echo "Port $port closed"
done

echo "All specified ports are now closed."