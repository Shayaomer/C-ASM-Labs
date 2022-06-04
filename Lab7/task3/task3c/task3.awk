#! /usr/bin/awk -f

BEGIN{
    FS = ","
    counter = 0
}

$2 ~/happy/ {print $2 "---" $3 "---" $4}

END {print "The number of poem that have fun or fan is : "counter}
