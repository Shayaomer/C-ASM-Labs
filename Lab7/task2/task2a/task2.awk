#! /usr/bin/awk -f

BEGIN{
    FS = ","
    std_to_calc = 0
    math_total = 0
    reading_total = 0
    writing_total = 0
}

{
    if ($4 == "standard"){
        std_to_calc ++
        math_total = math_total + $6
        reading_total = reading_total + $7
        writing_total = writing_total + $8
    }
}

END{
    print "Math average:" (math_total/std_to_calc)
    print "Reading average:" (reading_total/std_to_calc)
    print "Writing average:" (writing_total/std_to_calc)
}