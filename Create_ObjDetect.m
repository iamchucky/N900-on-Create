function [P,varargout] = Create_ObjDetect(host,N)
%Create_Location Retrieve Create robot pose from overhead localization
% system.
%
%  P = Create_ObjDetect(host,N) returns
%    * N should be a row vector consisting of the Id's of the object for 
%      which you wish to get the location.
%    * The size(N) row by 5 matrix P for a vector N with length(N)as well
%      as printing the values of Timestamp, Id, X, Y, and Z on the
%      screen for each value of N.
%    * P is of the form [ID X Y Z Timestamp] where each: ID, X, Y,
%      Z, and Timestamp is a length(N) column vector
%
%  [ID,X,Y,Z,TS] = Create_ObjDetect(N) returns
%    * The list of retrieved id's ID
%    * The x position of each object retrieved X
%    * The y position of each object retrieved Y
%    * The z position of each object retrieved Z
%    * The time the location was sent TS
%
%  NOTE:
%    * Output is put in numerical order according to Id number
%    * If robot is not seen in the field then values of zero are
%      substituted for real data.  This can be checked by making sure
%      Timestamp is not zero.  If a robot was detected Timestamp for that
%      robot will not be zero.
%    * A Warning Mesage stating "Warning: The specified amount of data was
%      not returned within the Timeout period." will appear at the
%      beginning of the output.  This is perfectly normal and will not
%      affect the functionality of the program.  

%   Author(s): C. Yang
%   $Date: 2011/02/21  


if nargin == 0
    error('Camera:NoIds','Too few arguments.  See help Create_ObjDetect'); 
elseif nargin > 2
    error('Camera:TooManyArguments','Too many arguments.  See help Create_ObjDetect');
end

%remote port (integer value)
port=45454;

%get UDP packet from the remote host
u=udp(host,port,'LocalPort',port);

%open the packet and
fopen(u);
%count = length(N)*19;

%read in packet and get size
[packet count] = fread(u);

%if statement used to prevent breakdown when no object detected in the
%view
msg_struct_len = 32;
if count>=msg_struct_len
    num_obj = count/msg_struct_len;
    %initialize parsing variables
    Id(1:num_obj)= 0; 
    X(1:num_obj) = 0;
    Y(1:num_obj) = 0;
    Z(1:num_obj) = 0;
    Timestamp(1:num_obj) = 0;
    %iterate for multiple objects
    for i=1:1:num_obj
        ind = (i-1)*msg_struct_len;
        Id(i) = typecast(uint8(packet(ind+9:ind+12,1)),'int32');
        X(i) = typecast(uint8(packet(ind+13:ind+16,1)),'single');
        Y(i) = typecast(uint8(packet(ind+17:ind+20,1)),'single');
        Z(i) = typecast(uint8(packet(ind+21:ind+24,1)),'single');
        Timestamp(i) = typecast(uint8(packet(ind+25:ind+32,1)),'double');
    end
    
%     %reorder data in asending order
%     N=sort(N);
    % Create matrix of the ID(col.1),Pose(2:4), and Timestamp(5)
    L(1:length(N),1:5)=0;
    for i=1:length(N)
        L(i,1)=N(i);
        for k=1:size(Id,2)
            %Search for desired Id
            if Id(k)==N(i)
                L(i,2)=X(k);
                L(i,3)=Y(k);
                L(i,4)=Z(k);
                L(i,5)=Timestamp(k);
                
                %quit searching when Id is found
                break
                
            end
        end
        %Notify user if a desired Id is not within the packet
        if L(i,5)==0
            fprintf(1,'\nCannot see ID %d\n',N(i));
        end
    end
else
    L(1:length(N),1:5)=0;
    fprintf(1,'\nThere is no object on the view!\n');
end

%Notify user of the Index corresponding to each Id
fprintf(1,'\nID to Index Conversion:\n ID  Index\n')
for i=1:length(N)
    fprintf(1,'  %d   %d\n',N(i),i)
end

%Organize output
no=nargout;
%ie Create_ObjDetect(host,N)=>notify user that they did not provide an output
if no==0
    for i=1:length(N)
        fprintf(1,'Timestamp: %0.3f, ID: %d, X: %0.3f, Y: %0.3f, Z: %0.3f',L(i,5), L(i,1), L(i,2), L(i,3), L(i,4))
    end
    fprintf(1,'\nNo data output to workspace \nPlease provide output variable if you want to use location information \n(see: help Create_ObjDetect)\n')
    %ie P=Create_ObjDetect(host,N)
elseif no==1
    P=L;
else
    P=L(:,1);
    varargout=num2cell(L(:,2:no),1);
end

fclose(u)
delete(u)
clear u