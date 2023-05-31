# data-structures-in-c

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

gcc -o test linked_list.c test.c

./test
```