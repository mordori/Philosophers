# Philosophers
## 📖 Introduction
**Dining philopsophers with threads and mutex.**

- Philosophers think, eat, and sleep

- Each philosopher is represented by a thread

- There is one fork per philosopher, but in order to eat a philosopher needs two so they have to take turns in eating

- None should die from starvation, if the provided parameters consider the number of philosophers, the combined time of the actions, and some overhead for computing time

- The philosophers are not allowed to communicate with each other

<p align="center">
  <img src="https://github.com/mordori/mordori/blob/main/doc/asdasd.gif" alt="Demo" />
</p>

## ✨ Features
- Resources shared between threads are locked with mutex to avoid data races

- Main thread handles monitoring the philosophers' status, controlling the simulation

- Dedicated logging thread decoupled from status monitoring handles the printing of the philosophers' states. It constructs batches from a queue of logs and writes them out periodically

> [!IMPORTANT]
> Outcome may depend on the performance of the computer used to run the program.

> [!NOTE]
> ## 🔡 Input
The program expects four arguments, with an optional fifth one.

The time inputs are in milliseconds.
``` bash
./philo `number_of_philos` `time_to_die` `time_to_eat` `time_to_sleep` (optional)`number_of_meals`
```
For example:

| Input					               | Outcome							     	   |
|------------------------------|-------------------------------|
|  `./philo 1 100 100 100`     | The philosopher dies          |
|  `./philo 3 610 200 100`     | Nobody dies, inifinite        |
|  `./philo 199 185 60 60 5`   | Nobody dies, finite           |
|  `./philo 200 130 60 60 10`  | Nobody dies, finite           |

The program internally calculates time in microseconds for accuracy.

The timestamps of the printed states are in milliseconds.

> [!TIP]
> ## 🚀 How to use
Run the following commands to clone the repository and create `philo` program
``` git
git clone https://github.com/mordori/Philosophers.git Philosophers
cd Philosophers
make
```
Execute the program with proper input, for example
``` bash
./philo 3 610 200 100 3
```
A lot of logs will be produced and queued per millisecond with a large `number_of_philos` and small `time_to_eat` or `time_to_sleep`.

The default 1024 logs per millisecond might not be enough for example to `./philo 999 185 60 60 10`, so the program will print an error and end the simulation.

You can increase the queue size by recompiling the program with a define
``` git
make QUEUE_SIZE=2048
```
To delete all of the compiled files, use
``` Makefile
make fclean
```
