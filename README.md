# Rainbow

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Rainbow is a tool that lets you easily run repetitive or tedious batch jobs across your cluster.

## Installing from source code

Rainbow has almost no dependencies. It should build without issues on most POSIX compliant systems that have gcc and the pthread library. There is a makefile included in the repository that has been tested on Linux and Mac OS X. The default target builds rainbow in the `bin` directory within the repository.

```
make
```

Super user privileges are not required for building rainbow but may be required for running it on some systems based on the privileges your ssh client needs.

## Usage

You can automate your workload by writing a **rain** script tht rainbow executes for you. Here is an example of a rain script.

```
connect user@server.com -i rsa_key
connect user@server2.com -i rsa_key
task echo Hello > out.txt
task echo World >> out.txt
run 0 0
run 0 1
run 1 0
run 1 1
```

The first two lines set up connections. The syntax of the `connect` command is as follows:
```
connect <host> [parameters]
```

You can specify a host. Optionally, you can add any additional parameters you want rainbow to relay to your local ssh client when attempting to connect with the host you specified. The example script creates to connections, to server and server2, using `rsa_key` as the authentication key.


The next two lines register tasks you want to execute. Anything that follows the term `task` (and a space) is relayed directly to your servers. Note that the `task` command only registers a task, it does not execute it.

The final four lines run task 0 on server, task 1 on server, task 0 on server2 and task 1 on server2 (in that order). These are non-blocking calls, rainbow does not wait for a run's task to finish before initiating the next run. Rainbow informs you when each run completes. 

The `run` command has the following syntax:
```
run <connection index> <task index>
```

Connections and tasks are indexed in the order they are declared in a rain script. In our example, server will have a connection index of `0` and server2 will have a connection index of `1`. Similarly, the task that outputs "Hello" will have a task index of `0` and the task that outputs "World" will have a task index of `1`.

If you save the above script as `example.rain`, you can execute it using rainbow:
```
rainbow example.rain
```

## Questions, patches and bug reports
Github is the best place to contact regarding questions and issues. Rainbow is actively maintained.
