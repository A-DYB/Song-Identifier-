close all
%song list: sad,epic,buddy
[y,Fs] = audioread('buddy.mp3'); 
%filter frequencies above 5 kHz to avoid aliasing when downsampling to
%11025 Hz
y = lowpass(y,5000,44100);
y_downsamp = downsample(y,4); 

%figure; plot(y_downsamp); % Plot the original waveform.

sz= size(y_downsamp)
%1:1048576
%661500:716625
AudioArray = int16( (y_downsamp(1:1323000,1))*10000); 
csvwrite('song_arr.csv',AudioArray');
figure;plot(AudioArray);title('song array');

%%%%cut a 5 second clip from the original song at 60 seconds
ClipArray = int16( (y_downsamp(661500:716625,1))*10000);
csvwrite('clip_arr.csv',ClipArray');
figure;plot(ClipArray);title('clip array');
%%%%
