#! /usr/bin/awk -f

BEGIN{
    FS = ","
    counter = 0
}

/f[ua]n/ {counter++}

END {print "The number of poem that have fun or fan is : "counter}
