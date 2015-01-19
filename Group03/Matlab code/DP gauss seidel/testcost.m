% Displays the cost of a transition from (xref,yref) to any point on the
% map

x=[];
y=[];
cost=[];

i=1;

xref=22;    % set xref here
yref=12.5;  % set yref here

for xi=5:0.1:25
    for yi=5:0.1:25
        x(i)=xi;
        y(i)=yi;
        [ cost(i) ] =calccost( xi,yi,xref,yref );
        i=i+1;       
    end
end

plot3(x,y,cost,'.','MarkerSize',2)
hold on
scatter([xref xref],[yref yref+1])
