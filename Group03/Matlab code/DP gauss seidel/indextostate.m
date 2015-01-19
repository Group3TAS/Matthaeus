function [ x,y,theta ] = indextostate(xi,yi,thetai )
% INDEXTOSTATE Computes the continuous state that corresponds to the index
% of a discrete state.
    global consts
    
    x=consts.xstart+(xi-0.5)*consts.DX;
    y=consts.ystart+(yi-0.5)*consts.DY;
    theta=(thetai-1)*consts.DTHETA;
end

