function [ dx, dy, dtheta ] = dynamic( x,y,theta,v,radius,dt )
% DYNAMIC Implements the dynamic of the car. It is a very simple
% approximation assuming that the car always drives in circles. The radius
% of the car is given with radius, the velocity with v and the length of
% the timestep with dt. If radius is 0 the car drives in a straight line.

    ydirection=sign(radius); % -=right(neg y) +=left(pos y)
    
    radius=abs(radius);

    if radius~=0
        phi=dt*v/radius;

        dtheta=phi*ydirection;

        dxcar=sin(phi)*radius;
        dycar=(1-cos(phi))*radius*ydirection;

        dx=dxcar*cos(theta)+dycar*sin(-theta);
        dy=dxcar*sin(theta)+dycar*cos(theta);
    else
        dtheta=0;
        dxcar=v*dt;
        dycar=0;

        dx=dxcar*cos(theta)+dycar*sin(-theta);
        dy=dxcar*sin(theta)+dycar*cos(theta);
        
    end
end

