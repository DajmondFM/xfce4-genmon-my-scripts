#!/bin/bash

#TOTAL
total=$(free --human --mega | awk 'NR==2 {printf "%d", $2}')
# echo "Total: $total"

#AVAILABLE
available=$(free --human --mega | awk 'NR==2 {printf "%d", $7}')
# echo "Available: $available"

#USED
used=$(($total-$available))
# echo "Used: $used"

#Used in %
used_percent=$(($used*100/$total))
# echo "Memory: $used_percent%"

color='lightgrey'

if [ $used_percent -gt 80 ]; then
  color='red'
elif [ $used_percent -gt 60 ]; then
  color='orange'
elif [ $used_percent -gt 40 ]; then
  color='yellow'
elif [ $used_percent -gt 20 ]; then
  color='lightgreen'
fi

echo "<txt><span color='$color'>Mem: $used_percent%</span></txt>"
# echo "<bar>$used_percent</bar>"
echo -e "<tool>Mem:\t   $used_percent% \nTotal:\t   $total GB\nAvailable: $available GB\nUsed:\t   $used GB</tool>"
