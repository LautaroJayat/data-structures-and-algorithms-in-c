# Setting up the lab

Table of contents

- [GCC](#gcc)
- [Installing Docker](#installing-docker)
- [Creating a container that has all what we need](#creating-a-container-that-has-all-what-we-need)

- [How to use this containers](#how-to-use-this-container)

## GCC

There are numerous ways to start developing and compiling C. On one hand, you could consider using an IDE like Visual Studio or Visual Studio Code, along with some helpful extensions. On the other hand, you can opt for the reliable combination of GCC and other free and open-source tools, which perform exceptionally well. However, it's worth noting that this latter approach may not be compatible with all operating systems due to potential missing tools or libraries. Nevertheless, there are simple and effective solutions available for this issue

GCC, short for GNU Compiler Collection, is a versatile tool for compiling various programming languages. According to [the official website](https://gcc.gnu.org/):

> The GNU Compiler Collection includes front ends for C, C++, Objective-C, Fortran, Ada, Go, and D, as well as libraries for these languages (libstdc++,...).

The easiest way to get started with GCC in a controlled environment, without risking any conflicts or having to install multiple components that may raise concerns, is to utilize Docker and virtualize the entire setup.

## Installing Docker

We typically use the term "Docker" to refer to a tool or a collection of tools that enable us to build, run, and manage containers.

Likewise, we commonly use the term "containers" to denote an implementation of virtualization technology that allows us to isolate environments and processes, simulating separate machines with their own operating systems, programs, configurations, and more.

For installing Docker, the best guides can be found on [their official website](https://www.docker.com/).

I would recommend installing only the CLI tools, although Docker Desktop is also a suitable option. Here are some useful resources to assist you with the installation process:

- [Mac installation](https://docs.docker.com/desktop/install/mac-install/)
- [Windows installation](https://docs.docker.com/desktop/install/windows-install/)
- [Linux installation](https://docs.docker.com/desktop/install/linux-install/)

## Creating a container that has all what we need

Using Docker might be tricky at the beginning, but if you've seen [The Matrix](https://en.wikipedia.org/wiki/The_Matrix), you'll grasp it quite quickly.

The main task here is to create this virtual world. It should look like a Debian machine, having all the tooling we need.

For this, lets firts create the so called "Dockerfile" to instruct docker what do we want.

```Dockerfile
# ./Dockerfile

# 1. start from debian
FROM debian:latest

# 2. update the package manager
RUN apt update -y

# 3. install this package that contains GCC
#    and everything we would need for this project
RUN apt install build-essential -y
```

This content can be placed inside a file named "Docker".

Next, we should open a terminal in the same folder where we have previously placed our Dockerfile. This will allow us to build the container image based on the instructions provided above:

```bash
# hey docker, please build the Dockerfile in this folder and tag it with "clab"
docker build -t clab .
```

Now we have a Docker image that we can refer to using the tag "clab," which represents our lab for conducting experiments with the C language.

To run this container, simply type:

```bash
docker run -v $(pwd)/:/app -it clab
```

The `-it` flag will instruct Docker to attach an interactive terminal to the container, enabling us to continue working inside the container. This allows us to operate within the container context, akin to being inside the matrix.

The `-v $(pwd)/:/app` portion instructs Docker to map the current working directory of your host (the real world) to a folder inside the container located at `/app`.

You will observe a change in the terminal's color scheme, indicating that you are now logged in as `root`.

To verify, let's start experimenting with GCC by requesting its version.

```bash
gcc --version
# gcc (Debian 10.2.1-6) 10.2.1 20210110
# Copyright (C) 2020 Free Software Foundation, Inc.
# This is free software; see the source for copying conditions.  There is NO
# warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

## How to use this container

I suggest creating a folder for each example and starting the Docker container from there.

Then, navigate to the `/app` folder inside the container by using the command `cd /app`.

Keep in mind that you can use your preferred text editor to create or modify files in your project folder on your host machine. All changes made in this "real world" context will be visible and reflected inside the container.

This way, you can write code as usual, while performing compilations within the isolated environment provided by Docker.

:)
