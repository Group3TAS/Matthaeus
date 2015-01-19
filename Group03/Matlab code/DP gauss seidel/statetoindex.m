function [ xi,yi,thetai ] = statetoindex(x,y,theta )
%STATETOINDEX Transforms a continuous state into the indices of the
%discrete states
    global consts

    while theta<0
        theta=theta+2*pi;
    end
    while theta>2*pi
        theta=theta-2*pi;
    end
    
    if (theta<=consts.DTHETA/2)||(theta>=(2*pi-consts.DTHETA/2))
        thetai=1;
    else
        thetai=round(theta/consts.DTHETA+1);
    end
        
    xi=round((x-consts.xstart-consts.DX/2)/consts.DX+1);
    yi=round((y-consts.ystart-consts.DY/2)/consts.DY+1);
        
    xi=min(xi,consts.nx);
    xi=max(xi,1);
    yi=min(yi,consts.ny);
    yi=max(yi,1);
end




