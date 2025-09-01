/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:01:50 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/31 20:28:26 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "errors.h"
#include "simulation.h"
#include "simulation_utils.h"

int	main(int argc, char *argv[])
{
	t_sim	sim;

	if (argc < 5 || argc > 6)
	{
		ft_perror("Invalid amount of arguments.");
		return (EXIT_FAILURE);
	}
	if (!init_sim(&sim, argc, argv))
		return (EXIT_FAILURE);
	simulate(&sim);
	clean_sim(&sim, &sim.mutex_print);
	return (EXIT_SUCCESS);
}


// #include <stdio.h>
// #include <pthread.h>
// #include <stdatomic.h>
// #include <unistd.h>

// #define NUM_THREADS 10
// #define ITERATIONS 100000

// // Test with atomic_int using direct operations
// atomic_int atomic_counter = 0;

// // Test with regular int (will have race conditions)
// int regular_counter = 0;
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// void* atomic_worker(void* arg) {
// 	(void)arg;
//     for (int i = 0; i < ITERATIONS; i++) {
//         atomic_counter++;  // Direct atomic operation - NO load/store needed
//     }
//     return NULL;
// }

// void* regular_worker(void* arg) {
// 	(void)arg;
//     for (int i = 0; i < ITERATIONS; i++) {
//         pthread_mutex_lock(&mutex);
//         regular_counter++;  // Protected by mutex
//         pthread_mutex_unlock(&mutex);
//     }
//     return NULL;
// }

// void* unsafe_worker(void* arg) {
// 	(void)arg;
//     for (int i = 0; i < ITERATIONS; i++) {
//         regular_counter++;  // UNSAFE - will have race conditions
//     }
//     return NULL;
// }

// int main() {
//     pthread_t threads[NUM_THREADS];

//     printf("Testing atomic operations...\n");

//     // Test 1: Atomic operations with direct syntax
//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_create(&threads[i], NULL, atomic_worker, NULL);
//     }
//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_join(threads[i], NULL);
//     }
//     printf("Atomic counter (direct operations): %d\n", atomic_counter);
//     printf("Expected: %d\n", NUM_THREADS * ITERATIONS);
//     printf("Match: %s\n\n", (atomic_counter == NUM_THREADS * ITERATIONS) ? "YES" : "NO");

//     // Test 2: Regular int with mutex protection
//     regular_counter = 0;
//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_create(&threads[i], NULL, regular_worker, NULL);
//     }
//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_join(threads[i], NULL);
//     }
//     printf("Regular counter (mutex protected): %d\n", regular_counter);
//     printf("Expected: %d\n", NUM_THREADS * ITERATIONS);
//     printf("Match: %s\n\n", (regular_counter == NUM_THREADS * ITERATIONS) ? "YES" : "NO");

//     // Test 3: Unsafe operations (will show race conditions)
//     regular_counter = 0;
//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_create(&threads[i], NULL, unsafe_worker, NULL);
//     }
//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_join(threads[i], NULL);
//     }
//     printf("Unsafe counter (no protection): %d\n", regular_counter);
//     printf("Expected: %d\n", NUM_THREADS * ITERATIONS);
//     printf("Match: %s\n", (regular_counter == NUM_THREADS * ITERATIONS) ? "YES" : "NO");

//     return 0;
// }
