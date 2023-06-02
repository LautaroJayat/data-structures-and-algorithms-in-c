[![LinkedIn](https://img.shields.io/badge/linkedin-%230077B5.svg?style=sociale&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/lautaro-jayat/)
[![GitHub watchers](https://img.shields.io/github/watchers/lautarojayat/data-structures-and-algorithms-in-c.svg?style=social&label=Watch)](https://GitHub.com/lautarojayat/data-structures-and-algorithms-in-c/watchers/)
[![GitHub forks](https://img.shields.io/github/forks/lautarojayat/data-structures-and-algorithms-in-c.svg?style=social&label=Fork)](https://GitHub.com/lautarojayat/data-structures-and-algorithms-in-c/network/)
[![GitHub stars](https://img.shields.io/github/stars/lautarojayat/data-structures-and-algorithms-in-c.svg?style=social&label=Star)](https://GitHub.com/lautarojayat/data-structures-and-algorithms-in-c/stargazers/)

# An overview of data structures and algorithms in C

When starting out in programming, it is common to perceive certain aspects or behaviors as 'magical'.

For example, if the first language you have learned was JavaScript or Python, dynamic arrays and dictionaries/objects are often taken for granted. They work as intended, requiring only the reading of the API documentation to utilize them. There are no memory allocations, resizing, hashing functions, or other low-level implementation details to worry about.

Exploring the reasoning behind various data structures and how one can use them to implement algorithms may reduce the amount of magic involved in programming. It empowers newcomers in the field by building critical thinking skills and allowing them to make informed decisions.

I hope these examples end up being useful to someone

## Contents

| Chapter |                                     Topic                                      |                                              Contents                                               | Source                                                                                                              |
| :-----: | :----------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------: | ------------------------------------------------------------------------------------------------------------------- |
|    0    |        [Setting up a lab to work with C](00_setting_up_the_lab/readme.md)        |               A quick tutorial on how to create a development environment with docker               |
|    1    | [Implementing a stack using an array](01_stack_array_implementation/readme.md) |                 Overview on how a stack works and how to implement it with an array                 | [link](https://github.com/LautaroJayat/data-structures-and-algorithms-in-c/tree/main/01_stack_array_implementation) |
|    2    |         [Implementing a simple linked list](02_linked_list/readme.md)          |                  An explanation on how a linked list works and how to implement it                  | [link](https://github.com/LautaroJayat/data-structures-and-algorithms-in-c/tree/main/02_linked_list)                |
|    3    |             [Creating a dynamic array](03_dynamic_array/readme.md)             |      The differences between an static array an a dynamicaly sized array, with implementations      | [link](https://github.com/LautaroJayat/data-structures-and-algorithms-in-c/tree/main/03_dynamic_array)              |
|    4    | [Checking for balanced braces in a string](04_check_balanced_braces/readme.md) | A hands-on example on how to check for unbalanced braces in a string using the stack from chapter 2 | [link](https://github.com/LautaroJayat/data-structures-and-algorithms-in-c/tree/main/04_check_balanced_braces)      |

## How to start playing with the source code

If you want to skip all of the above, if you are a person who loves learning by reading code, [you can clone the github repo](https://github.com/LautaroJayat/data-structures-and-algorithms-in-c).

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

<details>

**<summary> Note </summary>**

Keep in mind I'm not a C expert nor I know all the conventions for this language.
I will strive to keep all the examples and the source as _correct_ as I can.
Also, If you read something odd, you can [make a PR](https://github.com/LautaroJayat/data-structures-and-algorithms-in-c/pulls) or [fill an issue](https://github.com/LautaroJayat/data-structures-and-algorithms-in-c/issues) in the [github repo](https://github.com/LautaroJayat/data-structures-and-algorithms-in-c).

</details>
