% Record your voice for 5 seconds.
recObj = audiorecorder(11025,8,1);
disp('Play Song.')
recordblocking(recObj, 5);
disp('End of Recording.');

% Play back the recording.
play(recObj);

% Store data in double-precision array.
myRecording = getaudiodata(recObj);
figure; plot(myRecording); % Plot the original waveform.

% cut the area you want and convert it into integers
MyAudioArray = int16(myRecording(1:end)*10000); 
csvwrite('clip_arr.csv',MyAudioArray');
% Plot the modified waveform.
figure;plot(MyAudioArray);
