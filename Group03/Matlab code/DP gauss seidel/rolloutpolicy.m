function [ v,radius ] = rolloutpolicy( x,y,theta )
% ROLLOUTPOLICY This function computes the one step opimal policy for a
% given sate and a given value function. It also updates the value of this
% state using the global variable value. This is the essential minimization
% step of the entire algorithm.

    global value;
    global consts;
  
    time=0.25;
    dt=time;
    
    legalradius1=[-0.8 0 0.8];
    legalradius2=[0 -2 0];
    legalradius3=[-0.4 2 0.4];

    simv=1;
    cost=99999999999999;
    
    [ xi,yi,thetai ] = statetoindex(x,y,theta );

    for ri=1:3
        simradius1=legalradius1(ri);
        simradius2=legalradius2(ri);
        simradius3=legalradius3(ri);
        
        [ dx1, dy1, dtheta1 ] = dynamic( x,y,theta,simv,simradius1,dt );
        xsim1=x+dx1;
        ysim1=y+dy1;
        thetasim1=theta+dtheta1;  
        
        [ dx2, dy2, dtheta2 ] = dynamic( x,y,theta,simv,simradius2,dt );
        xsim2=x+dx2;
        ysim2=y+dy2;
        thetasim2=theta+dtheta2; 
        
        [ dx3, dy3, dtheta3 ] = dynamic( x,y,theta,simv,simradius3,dt );
        xsim3=x+dx3;
        ysim3=y+dy3;
        thetasim3=theta+dtheta3; 

        %compute cost  
        
        [ newxi1,newyi1,newthetai1 ] = statetoindex(xsim1,ysim1,thetasim1 );
        cost1=consts.alpha*value(newxi1,newyi1,newthetai1)+calccost( xsim1,ysim1,x,y);
        
        [ newxi2,newyi2,newthetai2 ] = statetoindex(xsim2,ysim2,thetasim2 );
        cost2=consts.alpha*value(newxi2,newyi2,newthetai2)+calccost( xsim2,ysim2,x,y);
        
        [ newxi3,newyi3,newthetai3 ] = statetoindex(xsim3,ysim3,thetasim3 );
        cost3=consts.alpha*value(newxi3,newyi3,newthetai3)+calccost( xsim3,ysim3,x,y);
        
        
        newcost=0.5*cost1+0.25*cost3+0.25*cost2;

        if newcost<cost
            cost=newcost;
            v=simv;
            radius=simradius1;
            value(xi,yi,thetai)=cost;
        end                          
    end          
end

