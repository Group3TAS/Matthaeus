% This is one of eight savepolicy scripts. These scripts iterate over the
% discrete state space and call the function rolloutpolicy, which updates
% value and policy of the respective discrete state.

% This script iterates right on the lower side

global policy
global consts

for xi=2:(consts.nx-1)
    for yi=2:100
        for thetai=1:consts.ntheta
            x=consts.rangex(xi);
            y=consts.rangey(yi);
            theta=consts.rangetheta(thetai);
            [ v,radius ] = rolloutpolicy( x,y,theta);
            policy(xi,yi,thetai)=radius;
        end
    end
end

    