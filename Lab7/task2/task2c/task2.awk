#! /usr/bin/awk -f

BEGIN{
    FS = ","
    print "========="
    print "Success Student List"
    print "========="
    suc_std = 0
}

{
    if ($6 >= 80 && $7 >= 80 && $8 >= 80){
        std_bach ++
        $1 = sprintf("%-10s", $1)
        $4 = sprintf("%-10s", $4)
        $3 = sprintf("%-30s", $3)
        $2 = sprintf("%-17s", $2)
        printf $1 $2 $3 $4 "\n" 
    }
}

END{
    print "The number of students : " std_bach
}