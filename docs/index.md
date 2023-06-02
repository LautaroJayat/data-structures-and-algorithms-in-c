# An overview of data structures and algorithms in C

When starting out in programming, it is common to perceive certain aspects or behaviors as 'magical'.

For example, if the first language you have learned was JavaScript or Python, dynamic arrays and dictionaries/objects are often taken for granted. They work as intended, requiring only the reading of the API documentation to utilize them. There are no memory allocations, resizing, hashing functions, or other low-level implementation details to worry about.

Exploring the reasoning behind various data structures and how one can use them to implement algorithms may reduce the amount of magic involved in programming. It empowers newcomers in the field by building critical thinking skills and allowing them to make informed decisions.

I hope these examples end up being useful to someone

## How to start

To be able to play with the source without dealing with OS and build tools I would suggest to follow these steps:

1. build and tag a docker container with everything needed. There is already a Dockerfile for this.
2. Then run the container attaching an interactive terminal to it.

You can also map this repo to a folder inside the container to code in your host but use the build tools inside the container.

For example:

```bash
docker build -t clab .

docker run -v $(pwd)/:/app -it clab
```

Then, inside our container we can start working:

```bash
cd ./app/02_linked_list

make build

make run-tests
```
