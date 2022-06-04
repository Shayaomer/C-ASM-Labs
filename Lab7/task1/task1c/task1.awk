#! /usr/bin/awk -f

BEGIN {
    for(i=0; i < ARGC; i++)
        print "ARGV["i"] = " ARGV[i]
    FS = ","
    OFS = ", "
}

{
    if (FNR >= 51 && FNR <= 71){
        printf "Movie Name: "
        for(i=5; i <= NF; i++) 
            if(i > 5)  
                printf ","$i  
            else
                printf$i
        printf "\n"
    }
    
}