# Philosophers
## ✨ Features
**Dining philopsophers with threads and mutex.**

Description:

Philosophers think, eat, and sleep. Each philosopher is represented by a thread. There is one fork per philosopher, but in order to eat a philosopher needs two so they have to take turns in eating. None should die from starvation, if the provided parameters consider the combined time of the actions and some overhead for computing time. The philosophers are not allowed to communicate with each other.

- Resources shared between threads are locked with mutex to avoid data races

- Main thread handles monitoring the philosophers living status, and if they have eaten the optionally provided number of meals. In those cases the simulation will be flagged to be over

- A dedicated logging thread decoupled from philosopher status monitoring handles the printing of the states of the philosophers. It constructs batches from a queue of logs and writes them out periodically

> [!IMPORTANT]
> Outcome may depend on the performance of the computer used to run the program.

> [!NOTE]
> ## 🔡 Input
The program expects four arguments, with optional fifth one in the form of
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

> [!TIP]
> ## 🚀 How to use
Run the following commands to clone the repository and create `philo` program
``` git
git clone https://github.com/mordori/Philosophers.git Philosophers
cd Philosophers
make -j4
```
Execute the program with numbers in a random order, for example
``` bash
./philo 3 610 200 100 3
```
To delete all of the compiled files, use
``` Makefile
make fclean
```
