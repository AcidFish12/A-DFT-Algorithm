clc
close all
clear
%读入原信号并绘制
data = load('input.txt');
subplot(3,1,1);
plot(data);
%利用MATLAB计算fft并绘制
A_data=abs(fft(data,1024));
subplot(3,1,2);
plot(A_data(1:512));
title("输入信号");
xlabel("采样点数");
%读入C++多线程计算的fft数据并绘出
outputdata=load("output.txt");
outputdata_real=outputdata(:,1);
outputdata_img=outputdata(:,2);
A_out=complex(outputdata_real,outputdata_img);
A_out=abs(A_out);
title('matlab计算结果');
xlabel("f/hz");
A_out=flipud(A_out);
subplot(3,1,3);
plot(A_out(1:512));
title("C++winAPI计算结果")
xlabel("f/hz");
