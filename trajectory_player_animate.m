Copyright (c) 2009, Stephen Walkerh
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are 
met:

    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in 
      the documentation and/or other materials provided with the distribution
      
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.

%this code is to play back the solar system simulated data
% Stephen Walker 2009 (stephen.walker@student.uts.edu.au)

clear

fid=fopen('test.bin','r');
[fname,permission,machineformat,encoding]=fopen(fid)
% open the binary file for reading
planet_out_2=reshape(fread(fid,'*double'),75,[])';
fclose(fid);
%read planet values out of file to array
whos planet_out*
%  return

planet_out = planet_out_2;
%unnecesscary, but it allowed two different pieces of code to be easily joinedspliced together
[zzzz,count] = size(planet_out)
%gets dimensions of planet data array


figure(3)
clf
hold on
set(gcf,'Color',[1 1 1]*0.2)
set(gca,'Color',[1 1 1]*0.2)


i = 1:10:zzzz;
%i = 1:10:min(43200,zzzz);
inc = 3:3:(count);

a = plot3(planet_out(i,(inc-2)),planet_out(i,(inc-1)), planet_out(i,inc));
axis auto
view(3)
axis manual
grid on
axis equal


for l = 1:100:zzzz%min(43200,zzzz)
    k = 1:100:l;
    for j = 1:length(a);
        inc = j*3;
        set(a(j),'XData',planet_out(k,(inc-2)),'YData',planet_out(k,(inc-1)),'ZData',planet_out(k,inc));
    end
    drawnow;
    pause(.001)
    
end

