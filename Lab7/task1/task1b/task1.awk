#! /usr/bin/awk -f

BEGIN {
    for(i=0; i < ARGC; i++)
        print "ARGV["i"] = " ARGV[i]
    FS = ","
    OFS = ", "
}

{
    if (FNR != 1){
        print "Actor Name: "$4
        printf "Movie Name: "
        for(i=5; i <= NF; i++) 
            if(i > 5)  
                printf ","$i  
            else
                printf$i
        printf "\n"
        print "------------------------"
    } else{
        print "------------------------"
    }
        
}