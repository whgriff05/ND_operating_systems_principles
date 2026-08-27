# OS Taxonomy, History, and Themes

## OS Taxonomy

Operating Systems can differ by __purpose__
- __Mainframe__ - process many jobs or tasks at once (*time sharing, transaction processing, batch*)
    - optimize for throughput / volume
- __Server__ - Provide services to multiple users (*print, filesharing, web, etc.*)
    - optimize for throughput / volume
- __Personal__ - Support a single user (like a laptop or desktop)
    - optimize for latency
- __Handheld__ - Usually found in PDAs or mobile phones
    - optimize for power / energy
- __Embedded__ - Run on resource constrained devices that are not usually used for general purpose computing (*TVs, cars, microwaves, etc.*)
    - optimize for reliability
- __Real-Time__ - Provide absolute guarantees that a certain action will occur by a certain time
    - optimize for consistency

Operating Systems can differ by __structure__
- __Monolithic Kernel__ - A single program running in kernel mode
- __Microkernel__ - Multiple programs working together with the help of a *privileged intermediary*

*Note: Linux is monolithic, Windows and MacOS are historically microkernels but have moved to a hybrid kernel approach*

## OS History

At the beginning of computers, the operating system __did not exist__

Then, on the onset of batch computing, operating systems (*usually some sort of library*) were charged with processing __batches__ of tasks (*compile, load, run, output*)

Next comes multiprogramming with three hallmarks
1. multiprogramming - Enable multiple tasks to run concurrently
2. memory protection - Disallow one task from manipulating data of another task
3. timesharing - Split processing time among multiple users

## OS Themes

An operating system is a __body of software__ that enables other programs to interact with each other and the __physical hardware resources__ in an efficient manner. To do this, it utilizes
- __Virtualization__
- __Concurrency__
- __Persistence__

Computer hardware provides both __limitations__ and __opportunities__ for the operating system, which is in charge of managing these resources efficiently on behalf of the __applications__

