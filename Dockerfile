FROM debian:stable

RUN apt-get update

RUN apt-get install -y nasm grub2

RUN apt-get install -y qemu-user-static xauth

RUN apt-get install -y python3

RUN apt-get install -y mpc
