use clap::{App, Arg};
use dice_game_simulator::{GameMode, OptimizationLevel, Strategy};
use crate::strategies::create_strategies;
use crate::simulation::{run_simulation, SimulationConfig, SimulatorType};
use std::sync::Arc;

fn main() {
    let matches = App::new("Dice Game Simulator")
        .version("1.0")
        .arg(Arg::with_name("mode")
            .short("m")
            .long("mode")
            .value_name("MODE")
            .help("Sets the game mode (base or extended)")
            .takes_value(true)
            .default_value("base"))
        .arg(Arg::with_name("optimization")
            .short("o")
            .long("optimization")
            .value_name("LEVEL")
            .help("Sets the optimization level (standard, fast, or ultra)")
            .takes_value(true)
            .default_value("standard"))
        .arg(Arg::with_name("strategies")
            .short("s")
            .long("strategies")
            .value_name("STRATEGIES")
            .help("Specifies which strategies to use in the simulation (comma-separated)")
            .takes_value(true))
        .arg(Arg::with_name("num_games")
            .short("n")
            .long("num-games")
            .value_name("NUM")
            .help("Sets the number of games to simulate")
            .takes_value(true)
            .default_value("10000"))
        .arg(Arg::with_name("simulator")
            .short("sim")
            .long("simulator")
            .value_name("TYPE")
            .help("Sets the simulator type (multithreaded, parallel, or distributed)")
            .takes_value(true)
            .default_value("parallel"))
        .get_matches();

    let mode = match matches.value_of("mode").unwrap() {
        "base" => GameMode::Base,
        "extended" => GameMode::Extended,
        _ => panic!("Invalid game mode"),
    };

    let optimization = match matches.value_of("optimization").unwrap() {
        "standard" => OptimizationLevel::Standard,
        "fast" => OptimizationLevel::Fast,
        "ultra" => OptimizationLevel::UltraOptimized,
        _ => panic!("Invalid optimization level"),
    };

    let simulator_type = match matches.value_of("simulator").unwrap() {
        "multithreaded" => SimulatorType::Multithreaded,
        "parallel" => SimulatorType::Parallel,
        "distributed" => SimulatorType::Distributed,
        _ => panic!("Invalid simulator type"),
    };

    let all_strategies: Vec<Arc<dyn Strategy>> = create_strategies().into_iter().map(Arc::from).collect();
    let selected_strategies = if let Some(strategy_names) = matches.value_of("strategies") {
        let names: Vec<&str> = strategy_names.split(',').collect();
        all_strategies.into_iter()
            .filter(|s| names.contains(&s.name()))
            .collect()
    } else {
        all_strategies
    };

    let num_games: usize = matches.value_of("num_games").unwrap().parse().expect("Invalid number of games");

    let config = SimulationConfig {
        mode,
        optimization,
        strategies: selected_strategies,
        num_games,
        simulator_type,
    };

    let results = run_simulation(config);

    // Output results
    println!("Simulation Results:");
    for (strategy_name, win_rate) in results {
        println!("{}: {:.2}% win rate", strategy_name, win_rate * 100.0);
    }
}