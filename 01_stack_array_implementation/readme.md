[![LinkedIn](https://img.shields.io/badge/linkedin-%230077B5.svg?style=sociale&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/lautaro-jayat/)
[![GitHub watchers](https://img.shields.io/github/watchers/lautarojayat/data-structures-and-algorithms-in-c.svg?style=social&label=Watch)](https://GitHub.com/lautarojayat/data-structures-and-algorithms-in-c/watchers/)
[![GitHub forks](https://img.shields.io/github/forks/lautarojayat/data-structures-and-algorithms-in-c.svg?style=social&label=Fork)](https://GitHub.com/lautarojayat/data-structures-and-algorithms-in-c/network/)
[![GitHub stars](https://img.shields.io/github/stars/lautarojayat/data-structures-and-algorithms-in-c.svg?style=social&label=Star)](https://GitHub.com/lautarojayat/data-structures-and-algorithms-in-c/stargazers/)

[<- go back](../README.md)

# Implementing a stack using a simple array

## The stack as an abstract data structure

The stack is a relatively simple data structure.

Thinking abstract, it is a container where every time you put in something the order is preserved in chronologically order.

Then, to remove an element one can only take the last element that was placed there (the element at the top of the stack).

The fundamental operations are usually called **Push** for the action of adding something, and **Pop** to get the last element.

The order in which an element is placed or removed from the stack is usually called "**last in first out**" or **LIFO**.

To ilustrate the idea:

```sh
# 1. the stack is empty
[]

# 1. I want to put X in the stack

[ X ]

# 2. I want to put Y in the stack

[ X , Y ]

# 3. I want to put Z in the stack

[ X , Y , Z ]

# 4. Now I want to remove an element

[ X , Y ] #--> and you got Z because it was the latest element placed in there.
```

There are other operations one could do with a stack, these are called non essential operations.

For example we could mention **Peek**-ing an element, which allows to see the last element added without removing it. As this behavior could be also achieved by removing an element and then adding it again at the top of the stack, it is not considered as essential.

In the same way, we could check if the stack is empty of full to avoid performing operations that could not have sense in abstract, or crashing our system in a real implementation.

## Now yes, on implementation details
