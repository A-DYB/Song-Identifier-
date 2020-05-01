[y,Fs] = audioread('buddy_clip.m4a');
%Resample phone recording
y = lowpass(y,5000,44100);
y_downsamp = downsample(y,4); 

ClipArray = int16( (y_downsamp(1:55125,1))*10000);
csvwrite('clip_arr.csv',ClipArray');
figure;plot(ClipArray);title('clip array');