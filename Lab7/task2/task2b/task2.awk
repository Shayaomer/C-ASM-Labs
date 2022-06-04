#! /usr/bin/awk -f

BEGIN{
    FS = ","
    std_bach = 0
}

{
    if ($3 == "bachelor's degree"){
        std_bach ++
    }
}

END{
    print std_bach
}