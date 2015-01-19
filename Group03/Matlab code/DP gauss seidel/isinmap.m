function [ out ] = isinmap(x,y,theta)
%ISINMAP Checks if a coordinate is within the legal part of the map
    global legal
 
    [lx,ly]=size(legal);

    xa=lx-round(y/0.05);
    ya=round(x/0.05);
    
    if (x<8)||(y<4)||(y>22)||(x>25)
        out=0;
    else
        if legal(xa,ya)
            out=1;
        else
            out=0;
        end
    end  
end

