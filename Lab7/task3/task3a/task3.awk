#! /usr/bin/awk -f

BEGIN{
    FS = ","
}

/spring/ {print $2 "---" $3 "---" $4}
