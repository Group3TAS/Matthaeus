% plots the map

global consts
global legal
[lx,ly]=size(legal);

samples=100000;

points=[];

for i=1:samples
    x=randi(lx);
    y=randi(ly);
    if not(legal(x,y))
       p=[0.05*(lx-x) 0.05*y];
       if (p(1)>consts.ystart)&&(p(1)<consts.yend)&&(p(2)>consts.xstart)&&(p(2)<consts.xend)
            points=[points;p];
       end 
    end
end

hold on
scatter(points(:,2), points(:,1),0.1)
