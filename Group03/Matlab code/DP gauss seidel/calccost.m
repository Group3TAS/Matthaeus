function [ cost ] = calccost( xsim,ysim,x,y )
%CALCCOST computes the cost of a transition from (x,y) to (xsim,ysim).

    reward=0;
    punishment=0;
 
    if (x<17)&&(xsim>17)
        if (y<21)&&(y>18)
            punishment=1.1;
        end
        
        if (y<8)&&(y>5)
            reward=1;
        end
    end
    
    if (x>17)&&(xsim<17)
        if (y<21)&&(y>18)
            reward=1;
        end
        if (y<8)&&(y>5)
            punishment=1.1;
        end
    end
  
    if (y<13)&&(ysim>13)
        if (x>21)&&(x<25)
            reward=1;
        end
        
        if (x<13)&&(x>9)
            punishment=1.1;
        end
    end
    
    if (y>13)&&(ysim<13)
        if (x<25)&&(x>21)
            punishment=1.1;
        end
        if (x<13)&&(x>9)
            reward=1;
        end
    end

    illegal=not(isinmap(xsim,ysim,0));
    cost=illegal-reward+punishment;
end

