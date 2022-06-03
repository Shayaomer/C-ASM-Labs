#! /usr/bin/awk -f

BEGIN	{cnt = 0} 
{
    if($4 > 87)
        print $0
}
END {}