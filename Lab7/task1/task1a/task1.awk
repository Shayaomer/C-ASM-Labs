#! /usr/bin/awk -f

BEGIN {
    for(i=0; i < ARGC; i++)
        print "ARGV["i"] = " ARGV[i]
    FS = ","
    OFS = " | "
}

{
    $4 = sprintf("%-22s", $4)
    $3 = sprintf("%-5s", $3)
    $2 = sprintf("%-5s", $2)
    print $4,  $3,  $2
}