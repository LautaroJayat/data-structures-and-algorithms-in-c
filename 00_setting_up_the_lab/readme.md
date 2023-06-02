[![LinkedIn](https://img.shields.io/badge/linkedin-%230077B5.svg?style=sociale&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/lautaro-jayat/)
[![GitHub watchers](https://img.shields.io/github/watchers/lautarojayat/data-structures-and-algorithms-in-c.svg?style=social&label=Watch)](https://GitHub.com/lautarojayat/data-structures-and-algorithms-in-c/watchers/)
[![GitHub forks](https://img.shields.io/github/forks/lautarojayat/data-structures-and-algorithms-in-c.svg?style=social&label=Fork)](https://GitHub.com/lautarojayat/data-structures-and-algorithms-in-c/network/)
[![GitHub stars](https://img.shields.io/github/stars/lautarojayat/data-structures-and-algorithms-in-c.svg?style=social&label=Star)](https://GitHub.com/lautarojayat/data-structures-and-algorithms-in-c/stargazers/)

[<- go back](../README.md)

# Setting up the lab

## GCC

There are lot of ways one can start developing and compiling C.
On one hand, one could try using an IDE like Vistual Studio or Visual Studio Code plus some extensions.
On the other hand, one could just go and use the good old GCC and some other free and open sourced tools that works super well. The only drawback of this last approach is that GCC migth not work in all operative systems because they could lack some tools or libraries.
Nonetheless, there are some super simple solutions for this.

GCC stands for GNU Compiler Collection it helps to deal with compiling several languages. As they put in [their official website](https://gcc.gnu.org/):

> The GNU Compiler Collection includes front ends for C, C++, Objective-C, Fortran, Ada, Go, and D, as well as libraries for these languages (libstdc++,...).

The easiest approach to start using it in a isolated environment, without breaking anything or having to install lot of things that one may not trust is to make use of Docker and virtualize the whole thing.

## Installing Docker

We usually say "Docker" to refer a tool or a set of tools that let us build, run, and manage containers.

Also, we usually say "containers" to refeer to an implementation of some kind of virtualization technology that allows to isolate environments and processes as if they were separate machines with their own operative systems, programs, configurations, and so on.

To install Docker the best guides can be found in [their official website](https://www.docker.com/).

I would suggest to instal only the CLI tools, but Docker Desktop is fine too. Here are some helpful resources to do it.

- [Mac installation](https://docs.docker.com/desktop/install/mac-install/)
- [Windows installation](https://docs.docker.com/desktop/install/windows-install/)
- [Linux installation](https://docs.docker.com/desktop/install/linux-install/)

## Creating a container that has all what we need

Using docker migth be tricky at the begining, but if you have seen [The Matrix](https://en.wikipedia.org/wiki/The_Matrix) you will get it quite quickly.

The main task here is to create this virtual world. It should look like a Debian machine, having all the tooling we need.

For this, lets firts create the so called "Dockerfile" to instruct docker what do we want.

```Dockerfile
# ./Dockerfile

# 1. start from debian
FROM debian:latest

# 2. update the package manager
RUN apt update -y

#3. install this package that contains GCC and everything we would need for this project
RUN apt install build-essential -y
```

This content can be placed inside a file named Docker.

Then we need to open a terminal in the same folder where we have previously placed our Dockerfile, so we can build this container image from the instructions above:

```bash
# hey docker, please build the Dockerfile in this folder and tag it with "clab"
docker build -t clab .
```

Now we have a docker image that we can make reference to by the tag "clab", our lab to perform experiments with the C language.

To run this container we can type:

```bash
docker run -v $(pwd)/:/app -it clab 
```

The `-it` flag will instruct docker to attach a interactive terminal to the container, which will allow us to continue working inside the container. This means, in the container context (inside the matrix).

The `-v $(pwd)/:/app` part is instructing docker to map your current working directory of your host (the real world) inside a folder in the container that should be placed at `/app`.

You will notice that your terminal has changed it's colors, and now you are logged as `root`.

To check we can start playing with GCC, just ask it to provide it's version.

```bash
gcc --version
# gcc (Debian 10.2.1-6) 10.2.1 20210110
# Copyright (C) 2020 Free Software Foundation, Inc.
# This is free software; see the source for copying conditions.  There is NO
# warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

## How to use this container

I wll suggest to create a folder for each example, and start the docker container there.

Then, just move to the `/app` folder in the container by typing `cd /app`.

Keep in mind that you can use your text editor of preference to create or modify files in your project folder, in the real world, in your host. All changes will be visible inside your host.

This way you can write as usual but perform the compilations in your isolated environment.

:)
