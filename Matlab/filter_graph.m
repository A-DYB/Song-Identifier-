%Graph the filtered output for the 5s segment of the song and the clip
%close all
overlap = 0.5;

song_out =csvread('song_filter_out.csv');
song_out = song_out(645/overlap:710/overlap,1:end);
y =size(song_out);

figure;
hold on;
title('song filtered fft');
for i =1:y(2)
    for j =1:y(1)
        if song_out(j,i) ~=0
            scatter(j*overlap*1024/11025,11025*song_out(j,i)/1024);
        end
    end
end
hold off;

%%%%%%CLIP
clip_fft =csvread('out.csv');
y =size(clip_fft);

figure;
hold on;
title('clip fft');
plot((clip_fft));
hold off;

%%%%
clip_out =csvread('clip_filter_out.csv');
y =size(clip_out);

figure;
hold on;
title('clip filtered fft');
for i =1:y(2)
    for j =1:y(1)
        if clip_out(j,i) ~=0
            scatter(j*overlap*1024/11025,11025*clip_out(j,i)/1024);
        end
    end
end
hold off;