use std::sync::{Arc, Mutex};
use std::thread;
use rand::Rng;
use serde_json::json;
use std::fs::File;
use std::io::Write;

pub struct MultithreadedSimulations;

impl MultithreadedSimulations {
    pub fn run_simulations(total_games: usize, num_threads: usize) {
        let games_per_thread = total_games / num_threads;
        let results = Arc::new(Mutex::new(Vec::new()));
        let games_completed = Arc::new(Mutex::new(0));

        let mut handles = vec![];

        for i in 0..num_threads {
            let results = Arc::clone(&results);
            let games_completed = Arc::clone(&games_completed);
            let handle = thread::spawn(move || {
                let mut rng = rand::thread_rng();
                for _ in 0..games_per_thread {
                    let mut game = FastBaseGame::new(rng.gen());
                    let game_result = game.play_and_get_result();
                    
                    {
                        let mut results = results.lock().unwrap();
                        results.push(game_result);
                    }
                    
                    {
                        let mut completed = games_completed.lock().unwrap();
                        *completed += 1;
                    }
                }
            });
            handles.push(handle);
        }

        for handle in handles {
            handle.join().unwrap();
        }

        let final_results = results.lock().unwrap();
        let json_output = json!({
            "games": final_results,
        });

        let mut file = File::create("simulation_results.json").unwrap();
        file.write_all(serde_json::to_string_pretty(&json_output).unwrap().as_bytes()).unwrap();
    }


    pub fn get_games_completed(games_completed: &Arc<Mutex<usize>>) -> usize {
        *games_completed.lock().unwrap()
    }
}

// Add this to your main.rs or lib.rs file:
pub fn run_multithreaded_simulations() {
    let total_games = 10000;
    let num_threads = 4;
    
    println!("Starting {} games across {} threads...", total_games, num_threads);
    
    let games_completed = Arc::new(Mutex::new(0));
    let games_completed_clone = Arc::clone(&games_completed);
    
    let simulation_thread = thread::spawn(move || {
        MultithreadedSimulations::run_simulations(total_games, num_threads);
    });
    
    // Progress reporting
    while MultithreadedSimulations::get_games_completed(&games_completed_clone) < total_games {
        let completed = MultithreadedSimulations::get_games_completed(&games_completed_clone);
        println!("Progress: {}/{} games completed", completed, total_games);
        thread::sleep(std::time::Duration::from_secs(1));
    }
    
    simulation_thread.join().unwrap();
    println!("All simulations completed. Results written to simulation_results.json");
}