# Rainbow

<img src="https://upload.wikimedia.org/wikipedia/commons/b/b8/Rainbow_Lorikeet_with_Red_Silk_Cotton_Flowers_-_AndrewMercer_IMG41411.jpg" width="30%"/>

Rainbow is a tool that lets you easily run repetitive or tedious batch jobs across your cluster.

## Usage

You can automate your workload by writing a **rain** script tht rainbow executes for you. Here is an example of a rain script.

```
connect user@server.com -i rsa_key
connect user@server2.com -i rsa_key
task echo "Hello" > out.txt
task echo "World" >> out.txt
run 1 1
run 1 2
run 2 1
run 2 2
```

The first two lines set up connections. The syntax of the `connect` command is as follows:
```
connect <host> [parameters]
```

You can specify a host. Optionally, you can add any additional parameters you want rainbow to relay to your local ssh client when attempting to connect with the host you specified. The example script creates to connections, to server and server2, using `rsa_key` as the authentication key.


The next two lines register tasks you want to execute. Anything that follows the term `task` (and a space) is relayed directly to your servers. Note that the `task` command only registers a task, it does not execute it.

The final four lines run task 1 on server, task 2 on server, task 1 on server2 and task2 on server2 (in that order). These are non-blocking calls, rainbow does not wait for a run's task to finish before initiating the next run. Rainbow informs you when each run completes. 

The `run` command has the following syntax:
```
run <connection index> <task index>
```

Connections and tasks are indexed in the order they are declared in a rain script. In our example, server will have a connection index of `0` and server2 will have a connection index of `1`. Similarly, the task that outputs "Hello" will have a task index of `0` and the task that outputs "World" will have a task index of `1`.

If you save the above script as `example.rain`, you can execute it using rainbow:
```
rainbow example.rain
```

## Footnotes
Cover image: "Rainbow Lorikeet with Red Silk Cotton Flowers" By <a href="//commons.wikimedia.org/wiki/User:Bald_white_guy" title="User:Bald white guy">Andrew Mercer</a> <a rel="nofollow" class="external text" href="http://www.baldwhiteguy.co.nz">(www.baldwhiteguy.co.nz)</a> - <span class="int-own-work" lang="en">Own work</span>, <a href="https://creativecommons.org/licenses/by-sa/4.0" title="Creative Commons Attribution-Share Alike 4.0">CC BY-SA 4.0</a>, <a href="https://commons.wikimedia.org/w/index.php?curid=81752198">Link</a>


