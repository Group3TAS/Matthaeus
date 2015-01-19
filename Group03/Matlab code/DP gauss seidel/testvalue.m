% This script displays the computed value function. Also the minimum and
% maximum of the current value function are displayed in the command
% window.

maxval=25; % set maximum dislayed value
anglei=1;  % set direction index the car is facing (one equals zero degrees)

[lx, ly, ltheta]=size(value);

smallest=99999;
for x=1:lx
    for y=1:ly
        for theta=1:36
        actval=value(x,y,theta);
            if actval<smallest
                smallest=actval;
                xsmalli=x;
                ysmalli=y;
                thetasmalli=theta;
            end
        end
    end
    
end

points=zeros(lx*ly,3);
i=1;
for x=1:1:lx
    for y=1:1:ly
        points(i,:)=[x y value(x,y,anglei)];
        i=i+1;
    end
    
end

points2=zeros(round((lx*ly)),3);

for i=1:((lx*ly))
    if points(i,3)<50
        points2(i,:)=points(i,:);
    end
end

scatter3(points(find(points(:,3)<maxval),1),points(find(points(:,3)<maxval),2),points(find(points(:,3)<maxval),3),1,points(find(points(:,3)<maxval),3))

'max'
max(max(max(value)))
'min'
min(min(min(value)))