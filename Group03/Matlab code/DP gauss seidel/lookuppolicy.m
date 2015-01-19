function [ v,radius  ] = lookuppolicy( x,y,theta )
% LOOKUPPOLICY This function looks up the steering angle from the policy
% matrix for a given continuous state (x,y,theta).
    global policy;
    if isinmap(x,y,theta)
        v=1;
    else
        v=1;% change to zero if crash stops car
    end
    [ xi,yi,thetai ] = statetoindex(x,y,theta );
    radius=policy(xi,yi,thetai);
end

